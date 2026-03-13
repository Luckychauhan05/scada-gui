#include "widgets/processflowview.h"

#include "widgets/flowcomponents.h"
#include "widgets/valveindicator.h"
#include "widgets/blowercontrolpanel.h"

#include <QBrush>
#include <QFont>
#include <QGraphicsEllipseItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QLinearGradient>
#include <QPainterPath>
#include <QPen>
#include <QMouseEvent>
#include <QTimer>

namespace {

enum SceneLayer {
    PipeLayer = 1,
    EquipmentLayer = 2,
    LabelLayer = 3,
    ValveLayer = 4
};

void setLayer(QGraphicsItem *item, qreal layer)
{
    if (item) {
        item->setZValue(layer);
    }
}

void addJunctionDot(QGraphicsScene *scene, const QPointF &center, const QColor &color)
{
    auto *dot = scene->addEllipse(QRectF(center.x() - 5, center.y() - 5, 10, 10),
                                  QPen(Qt::NoPen),
                                  QBrush(color));
    setLayer(dot, PipeLayer + 0.1);
}

QPointF polylinePoint(const QList<QPointF> &points, qreal t)
{
    if (points.size() < 2) {
        return points.isEmpty() ? QPointF() : points.first();
    }

    qreal total = 0.0;
    for (int index = 1; index < points.size(); ++index) {
        total += QLineF(points[index - 1], points[index]).length();
    }

    const qreal target = qBound(0.0, t, 1.0) * total;
    qreal traversed = 0.0;
    for (int index = 1; index < points.size(); ++index) {
        const QLineF segment(points[index - 1], points[index]);
        const qreal length = segment.length();
        if (traversed + length >= target) {
            const qreal fraction = length > 0.0 ? (target - traversed) / length : 0.0;
            return QPointF(points[index - 1].x() + fraction * (points[index].x() - points[index - 1].x()),
                           points[index - 1].y() + fraction * (points[index].y() - points[index - 1].y()));
        }
        traversed += length;
    }

    return points.last();
}

}

ProcessFlowView::ProcessFlowView(QWidget *parent)
    : QGraphicsView(parent),
      m_scene(new QGraphicsScene(this)),
      m_flowTimer(new QTimer(this)),
      m_dotTimer(new QTimer(this)),
      m_alarmState(false),
      m_flowPulse(false)
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing, true);
    setFrameShape(QFrame::NoFrame);
    setSceneRect(0, 0, 1580, 900);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    buildScene();

    connect(m_flowTimer, &QTimer::timeout, this, &ProcessFlowView::animateFlow);
    m_flowTimer->start(500);

    connect(m_dotTimer, &QTimer::timeout, this, &ProcessFlowView::animateFlowDots);
    m_dotTimer->start(40);
}

void ProcessFlowView::animateFlow()
{
    m_flowPulse = !m_flowPulse;

    for (Pipeline *pipeline : m_pipelines) {
        if (pipeline) {
            pipeline->setPulse(m_flowPulse);
        }
    }

    // Controlled pipes should stop pulsing when their gate is blocked and remain in light state.
    for (const ControlledPipeline &entry : m_controlledPipelines) {
        if (!entry.item) {
            continue;
        }
        entry.item->setPulse(isGateOpen(entry.gate) ? m_flowPulse : true);
    }

    for (FlowArrow *arrow : m_arrows) {
        if (arrow) {
            arrow->setPulse(m_flowPulse);
        }
    }
}

void ProcessFlowView::animateFlowDots()
{
    for (FlowDot &dot : m_flowDots) {
        if (!dot.item) {
            continue;
        }

        const bool pathOpen = isGateOpen({dot.controllingValves, dot.controllingBlowers});

        // Darker dot indicates stopped flow segment.
        dot.item->setBrush(QBrush(pathOpen ? dot.baseColor.lighter(140) : dot.baseColor.darker(180)));

        // If any controlling MV is closed, freeze this flow dot in-place.
        if (!pathOpen) {
            continue;
        }

        dot.t += dot.speed;
        if (dot.t > 1.0) {
            dot.t -= 1.0;
        }

        const QPointF pos = polylinePoint(dot.path, dot.t);
        dot.item->setRect(pos.x() - 4, pos.y() - 4, 8, 8);
    }

    // Rotate blower internals only while each blower is running.
    for (auto it = m_blowers.begin(); it != m_blowers.end(); ++it) {
        Blower *blower = it.value();
        if (!blower) {
            continue;
        }
        const bool running = m_blowerRunning.value(it.key(), true);
        blower->setRunning(running);
        blower->stepRotation(9.0);
    }
}

void ProcessFlowView::toggleAlarmState()
{
    m_alarmState = !m_alarmState;
    if (m_mvValves.contains("MV6")) {
        m_mvValves.value("MV6")->setOpen(!m_alarmState);
    }
}

void ProcessFlowView::resetState()
{
    m_alarmState = false;

    for (auto it = m_blowerRunning.begin(); it != m_blowerRunning.end(); ++it) {
        it.value() = true;
    }
    for (auto it = m_blowers.begin(); it != m_blowers.end(); ++it) {
        if (it.value()) {
            it.value()->setRunning(true);
        }
    }

    for (auto it = m_mvValves.begin(); it != m_mvValves.end(); ++it) {
        if (it.value()) {
            it.value()->setOpen(true);
        }
    }
    for (auto it = m_valves.begin(); it != m_valves.end(); ++it) {
        const bool closed = (it.key() == "MV6");
        it.value()->setOpen(!closed);
    }
}

void ProcessFlowView::buildScene()
{
    m_scene->clear();
    m_pipelines.clear();
    m_controlledPipelines.clear();
    m_arrows.clear();
    m_valves.clear();
    m_mvValves.clear();
    m_blowers.clear();
    m_blowerPositions.clear();
    m_blowerRunning.clear();
    m_flowDots.clear();

    QLinearGradient bg(0, 0, 1580, 900);
    bg.setColorAt(0.0, QColor("#D9F3F7"));
    bg.setColorAt(1.0, QColor("#A9DEE2"));
    m_scene->setBackgroundBrush(bg);

    // ============ EQUIPMENT POSITIONING ============

    addEquipment("OVERHEAD TANK", QRectF(620, 0, 320, 80), QColor("#E6DFC4"), 10);

    addEquipment("RAPID GRAVITY\nFILTER - 1", QRectF(280, 180, 360, 130), QColor("#D7E5EA"), 11);
    addEquipment("RAPID GRAVITY\nFILTER - 2", QRectF(850, 180, 360, 130), QColor("#D7E5EA"), 11);

    addEquipment("CLEAR WATER TANK", QRectF(620, 430, 320, 100), QColor("#D4E5ED"), 10);
    addEquipment("BACKWASH TANK\nFILLING PUMPS", QRectF(620, 600, 320, 90), QColor("#C8DCE4"), 10);

    addEquipment("CHLORINE TONNER", QRectF(1040, 430, 240, 90), QColor("#EED5DF"), 10);
    addEquipment("DISTRIBUTION\nTANK", QRectF(1340, 430, 200, 100), QColor("#CDE3CE"), 10);

    // Add new Blower components
    addBlower("BLOWER 1", QPointF(60, 190));
    addBlower("BLOWER 2", QPointF(60, 275));
    addBlower("BLOWER 3", QPointF(60, 360));

    auto addFlowDot = [this](const QList<QPointF> &path,
                             const QColor &color,
                             qreal startT,
                             qreal speed = 0.005,
                             const QList<QString> &controllingValves = {},
                             const QList<QString> &controllingBlowers = {}) {
        auto *dot = m_scene->addEllipse(QRectF(0, 0, 8, 8), QPen(Qt::NoPen), QBrush(color.lighter(140)));
        setLayer(dot, PipeLayer + 0.4);
        m_flowDots.append({dot, path, startT, speed, controllingValves, controllingBlowers, color});
    };

    // ============ YELLOW: CLARIFIED WATER INLET (FROM CLARIFLOCCULATOR) ============
    const qreal clarHeaderY = 112.0;
    const qreal clarFilterHeaderY = 145.0;
    const qreal clarFilter1DropX = 410.0;
    const qreal clarFilter2DropX = 1050.0;

    addPipeline({QPointF(0, clarHeaderY), QPointF(clarFilter1DropX, clarHeaderY), QPointF(clarFilter1DropX, clarFilterHeaderY),
                 QPointF(clarFilter2DropX, clarFilterHeaderY)},
                QColor("#F4C542"), 8.0, {"MV9"});
    addPipeline({QPointF(clarFilter1DropX, clarFilterHeaderY), QPointF(clarFilter1DropX, 180)}, QColor("#F4C542"), 8.0, {"MV9"});
    addPipeline({QPointF(clarFilter2DropX, clarFilterHeaderY), QPointF(clarFilter2DropX, 180)}, QColor("#F4C542"), 8.0, {"MV9"});

    addJunctionDot(m_scene, QPointF(clarFilter1DropX, clarHeaderY), QColor("#F4C542"));
    addJunctionDot(m_scene, QPointF(clarFilter1DropX, clarFilterHeaderY), QColor("#F4C542"));
    addJunctionDot(m_scene, QPointF(clarFilter2DropX, clarFilterHeaderY), QColor("#F4C542"));
    addArrow(QPointF(180, clarHeaderY), 0, QColor("#F4C542"));
    addArrow(QPointF(720, clarHeaderY), 0, QColor("#F4C542"));
    addArrow(QPointF(clarFilter1DropX, 170), 90, QColor("#F4C542"));
    addArrow(QPointF(clarFilter2DropX, 170), 90, QColor("#F4C542"));

    addFlowDot({QPointF(0, clarHeaderY), QPointF(clarFilter1DropX, clarHeaderY), QPointF(clarFilter1DropX, clarFilterHeaderY),
                QPointF(clarFilter1DropX, 180)},
               QColor("#F4C542"), 0.18, 0.006, {"MV9"});
    addFlowDot({QPointF(0, clarHeaderY), QPointF(clarFilter1DropX, clarHeaderY), QPointF(clarFilter1DropX, clarFilterHeaderY),
                QPointF(clarFilter2DropX, clarFilterHeaderY), QPointF(clarFilter2DropX, 180)},
               QColor("#F4C542"), 0.62, 0.006, {"MV9"});

    QFont clarifiedLabelFont("Segoe UI", 8, QFont::Bold);
    clarifiedLabelFont.setWordSpacing(1.6);
    clarifiedLabelFont.setLetterSpacing(QFont::PercentageSpacing, 102.0);
    auto *clarifiedLabel = m_scene->addText("CLARIFIED WATER FROM CLARIFLOCCULATOR",
                                            clarifiedLabelFont);
    clarifiedLabel->setDefaultTextColor(QColor("#6B5712"));
    clarifiedLabel->setPos(20, 60);
    setLayer(clarifiedLabel, LabelLayer);

    // ============ ORANGE: RAW WATER (OVERHEAD -> FILTERS) ============
    const QColor overheadFeedColor("#E67E22");
    addPipeline({QPointF(780, 60), QPointF(780, 120)}, overheadFeedColor, 8.0, {"MV1"});
    addPipeline({QPointF(780, 120), QPointF(430, 120), QPointF(430, 180)}, overheadFeedColor, 8.0, {"MV1", "MV2"});
    addPipeline({QPointF(780, 120), QPointF(1030, 120), QPointF(1030, 180)}, overheadFeedColor, 8.0, {"MV1", "MV3"});

    addJunctionDot(m_scene, QPointF(780, 120), overheadFeedColor);
    addArrow(QPointF(780, 105), 90, overheadFeedColor);
    addArrow(QPointF(430, 172), 90, overheadFeedColor);
    addArrow(QPointF(1030, 172), 90, overheadFeedColor);

    addFlowDot({QPointF(780, 60), QPointF(780, 120), QPointF(430, 120), QPointF(430, 180)},
               overheadFeedColor, 0.08, 0.005, {"MV1", "MV2"});
    addFlowDot({QPointF(780, 60), QPointF(780, 120), QPointF(1030, 120), QPointF(1030, 180)},
               overheadFeedColor, 0.58, 0.005, {"MV1", "MV3"});

    // ============ GREEN: AIR HEADER (BLOWERS -> COMMON HEADER -> FILTERS) ============
    addPipeline({QPointF(140, 220), QPointF(250, 220)}, QColor("#2ECC71"), 6.0, {}, {"BLOWER 1"});
    addPipeline({QPointF(140, 305), QPointF(250, 305)}, QColor("#2ECC71"), 6.0, {}, {"BLOWER 2"});
    addPipeline({QPointF(140, 390), QPointF(250, 390)}, QColor("#2ECC71"), 6.0, {}, {"BLOWER 3"});

    addPipeline({QPointF(250, 220), QPointF(250, 390)}, QColor("#2ECC71"), 6.0);
    addPipeline({QPointF(250, 340), QPointF(1050, 340)}, QColor("#2ECC71"), 6.0);
    addPipeline({QPointF(410, 340), QPointF(410, 310)}, QColor("#2ECC71"), 6.0);
    addPipeline({QPointF(1050, 340), QPointF(1050, 310)}, QColor("#2ECC71"), 6.0);

    addJunctionDot(m_scene, QPointF(250, 220), QColor("#2ECC71"));
    addJunctionDot(m_scene, QPointF(250, 305), QColor("#2ECC71"));
    addJunctionDot(m_scene, QPointF(250, 390), QColor("#2ECC71"));
    addJunctionDot(m_scene, QPointF(410, 340), QColor("#2ECC71"));
    addJunctionDot(m_scene, QPointF(1050, 340), QColor("#2ECC71"));

    addArrow(QPointF(242, 220), 0, QColor("#2ECC71"));
    addArrow(QPointF(242, 305), 0, QColor("#2ECC71"));
    addArrow(QPointF(242, 390), 0, QColor("#2ECC71"));
    addArrow(QPointF(250, 320), 90, QColor("#2ECC71"));
    addArrow(QPointF(520, 340), 0, QColor("#2ECC71"));
    addArrow(QPointF(900, 340), 0, QColor("#2ECC71"));
    addArrow(QPointF(410, 322), -90, QColor("#2ECC71"));
    addArrow(QPointF(1050, 322), -90, QColor("#2ECC71"));

    addFlowDot({QPointF(140, 220), QPointF(250, 220), QPointF(250, 340), QPointF(1050, 340)},
               QColor("#2ECC71"), 0.10, 0.006, {}, {"BLOWER 1"});
    addFlowDot({QPointF(140, 305), QPointF(250, 305), QPointF(250, 340), QPointF(1050, 340)},
               QColor("#2ECC71"), 0.40, 0.006, {}, {"BLOWER 2"});
    addFlowDot({QPointF(140, 390), QPointF(250, 390), QPointF(250, 340), QPointF(1050, 340)},
               QColor("#2ECC71"), 0.70, 0.006, {}, {"BLOWER 3"});

    // ============ BLUE: FILTER OUTLET MERGE (FILTERS -> CWT) ============
    addPipeline({QPointF(450, 310), QPointF(450, 365), QPointF(780, 365)}, QColor("#4A90E2"), 8.0, {"MV4"});
    addPipeline({QPointF(1010, 310), QPointF(1010, 365), QPointF(780, 365)}, QColor("#4A90E2"), 8.0, {"MV5"});
    addPipeline({QPointF(780, 365), QPointF(780, 430)}, QColor("#4A90E2"));

    addJunctionDot(m_scene, QPointF(780, 365), QColor("#4A90E2"));
    addArrow(QPointF(450, 345), 90, QColor("#4A90E2"));
    addArrow(QPointF(1010, 345), 90, QColor("#4A90E2"));
    addArrow(QPointF(780, 410), 90, QColor("#4A90E2"));

    addFlowDot({QPointF(450, 310), QPointF(450, 365), QPointF(780, 365)},
               QColor("#4A90E2"), 0.16, 0.005, {"MV4"});
    addFlowDot({QPointF(1010, 310), QPointF(1010, 365), QPointF(780, 365)},
               QColor("#4A90E2"), 0.66, 0.005, {"MV5"});

    // ============ CYAN: BACKWASH (CWT -> PUMPS -> FILTERS) ============
    addPipeline({QPointF(780, 530), QPointF(780, 600), QPointF(780, 645)}, QColor("#17A2B8"), 8.0, {"MV6"});
    addPipeline({QPointF(780, 645), QPointF(370, 645), QPointF(370, 310)}, QColor("#17A2B8"), 8.0, {"MV6"});
    addPipeline({QPointF(780, 645), QPointF(1090, 645), QPointF(1090, 310)}, QColor("#17A2B8"), 8.0, {"MV6"});

    addJunctionDot(m_scene, QPointF(780, 645), QColor("#17A2B8"));
    addJunctionDot(m_scene, QPointF(370, 310), QColor("#17A2B8"));
    addJunctionDot(m_scene, QPointF(1090, 310), QColor("#17A2B8"));
    addArrow(QPointF(780, 570), 90, QColor("#17A2B8"));
    addArrow(QPointF(780, 635), 90, QColor("#17A2B8"));
    addArrow(QPointF(560, 645), 180, QColor("#17A2B8"));
    addArrow(QPointF(940, 645), 0, QColor("#17A2B8"));
    addArrow(QPointF(370, 500), -90, QColor("#17A2B8"));
    addArrow(QPointF(1090, 500), -90, QColor("#17A2B8"));

    addFlowDot({QPointF(780, 530), QPointF(780, 600), QPointF(780, 645)},
               QColor("#17A2B8"), 0.20, 0.007, {"MV6"});
    addFlowDot({QPointF(780, 645), QPointF(370, 645), QPointF(370, 310)},
               QColor("#17A2B8"), 0.35, 0.006, {"MV6"});
    addFlowDot({QPointF(780, 645), QPointF(1090, 645), QPointF(1090, 310)},
               QColor("#17A2B8"), 0.70, 0.006, {"MV6"});

    // ============ RED: CHLORINATION (CWT -> TONNER -> DISTRIBUTION) ============
    addPipeline({QPointF(940, 475), QPointF(1040, 475)}, QColor("#E74C3C"), 8.0, {"MV7"});
    addPipeline({QPointF(1280, 475), QPointF(1340, 475)}, QColor("#E74C3C"), 8.0, {"MV8"});

    addArrow(QPointF(990, 475), 0, QColor("#E74C3C"));
    addArrow(QPointF(1310, 475), 0, QColor("#E74C3C"));

    addFlowDot({QPointF(940, 475), QPointF(1040, 475)}, QColor("#E74C3C"), 0.15, 0.008, {"MV7"});
    addFlowDot({QPointF(1280, 475), QPointF(1340, 475)}, QColor("#E74C3C"), 0.65, 0.008, {"MV8"});

    // ============ VALVES (P&ID motor-valve symbol on pipelines) ============
    // Each QPointF is the exact pipe-centre point; addValve centres the bowtie there.
    addValve("MV1", QPointF(780, 90), true);   // Overhead tank outlet (vertical pipe x=780)
    addValve("MV2", QPointF(430, 168), true);   // Filter-1 inlet (vertical pipe x=430)
    addValve("MV3", QPointF(1030, 168), true);  // Filter-2 inlet (vertical pipe x=1030)
    addValve("MV4", QPointF(450, 350), true);   // Filter-1 outlet (vertical pipe x=450)
    addValve("MV5", QPointF(1010, 350), true);  // Filter-2 outlet (vertical pipe x=1010)
    addValve("MV6", QPointF(780, 565), true);   // Backwash isolation (vertical pipe x=780)
    addValve("MV7", QPointF(990, 475), true);   // Chlorine inlet (horizontal pipe y=475)
    addValve("MV8", QPointF(1310, 475), true);  // Distribution outlet (horizontal pipe y=475)
    addValve("MV9", QPointF(220, 112), true);   // Clarified-water inlet header (just ahead of first arrow at x=180)
}

void ProcessFlowView::addEquipment(const QString &title, const QRectF &rect, const QColor &fill, int pointSize)
{
    QPainterPath panel;
    panel.addRoundedRect(rect, 14, 14);
    auto *item = m_scene->addPath(panel, QPen(QColor("#28627A"), 2), QBrush(fill));
    setLayer(item, EquipmentLayer);

    auto *shine = m_scene->addRect(QRectF(rect.left() + 10, rect.top() + 10, 14, rect.height() - 20),
                                   QPen(Qt::NoPen),
                                   QBrush(QColor(255, 255, 255, 38)));
    setLayer(shine, EquipmentLayer);

    auto *text = m_scene->addText(title, QFont("Segoe UI", pointSize, QFont::Bold));
    text->setDefaultTextColor(QColor("#0E3348"));
    const QRectF tr = text->boundingRect();
    text->setPos(rect.center().x() - tr.width() / 2.0, rect.center().y() - tr.height() / 2.0);
    setLayer(text, LabelLayer);
}

void ProcessFlowView::addPipeline(const QVector<QPointF> &points,
                                  const QColor &color,
                                  qreal width,
                                  const QList<QString> &controllingValves,
                                  const QList<QString> &controllingBlowers)
{
    if (points.size() < 2) {
        return;
    }

    auto *pipeline = new Pipeline(points, color, width);
    setLayer(pipeline, PipeLayer);
    m_scene->addItem(pipeline);
    m_pipelines.append(pipeline);

    if (!controllingValves.isEmpty() || !controllingBlowers.isEmpty()) {
        m_controlledPipelines.append({pipeline, {controllingValves, controllingBlowers}});
    }
}

bool ProcessFlowView::isGateOpen(const ControlGate &gate) const
{
    for (const QString &valveName : gate.valves) {
        if (m_mvValves.contains(valveName) && !m_mvValves.value(valveName)->isOpen()) {
            return false;
        }
    }

    for (const QString &blowerName : gate.blowers) {
        if (m_blowerRunning.contains(blowerName) && !m_blowerRunning.value(blowerName)) {
            return false;
        }
    }

    return true;
}

void ProcessFlowView::addArrow(const QPointF &tip, qreal angleDeg, const QColor &color)
{
    Q_UNUSED(color)

    auto *arrow = new FlowArrow(tip, angleDeg);
    setLayer(arrow, PipeLayer + 0.2);
    m_scene->addItem(arrow);
    m_arrows.append(arrow);
}

void ProcessFlowView::addValve(const QString &name, const QPointF &scenePos, bool open)
{
    auto *mvValve = new ManualValveIndicator(name, open);
    // Local bow-tie centre is at (10, 23); offset so it sits centred on scenePos.
    mvValve->setPos(scenePos.x() - 10, scenePos.y() - 23);
    setLayer(mvValve, ValveLayer);
    m_scene->addItem(mvValve);
    m_mvValves.insert(name, mvValve);
}

void ProcessFlowView::addBlower(const QString &name, const QPointF &pos)
{
    auto *blower = new Blower(name);
    blower->setPos(pos);
    blower->setRunning(true);
    setLayer(blower, EquipmentLayer + 1);
    m_scene->addItem(blower);
    m_blowers.insert(name, blower);
    m_blowerPositions.insert(name, pos);
    m_blowerRunning.insert(name, true);
}

void ProcessFlowView::mouseReleaseEvent(QMouseEvent *event)
{
    QPointF scenePos = mapToScene(event->pos());

    // Check if any blower was clicked
    for (auto it = m_blowerPositions.begin(); it != m_blowerPositions.end(); ++it) {
        QString blowerName = it.key();
        QPointF blowerPos = it.value();

        // Blower bounds for compact symbol + text label area.
        QRectF blowerBounds(blowerPos.x(), blowerPos.y(), 84, 80);

        if (blowerBounds.contains(scenePos)) {
            onBlowerClicked(blowerName);
            return;
        }
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void ProcessFlowView::onBlowerClicked(const QString &blowerName)
{
    BlowerControlPanel *controlPanel = new BlowerControlPanel(blowerName, this);

    connect(controlPanel, &BlowerControlPanel::stopClicked, this, [this, blowerName]() {
        m_blowerRunning[blowerName] = false;
        if (m_blowers.contains(blowerName) && m_blowers.value(blowerName)) {
            m_blowers.value(blowerName)->setRunning(false);
        }
    });
    connect(controlPanel, &BlowerControlPanel::tripClicked, this, [this, blowerName]() {
        m_blowerRunning[blowerName] = false;
        if (m_blowers.contains(blowerName) && m_blowers.value(blowerName)) {
            m_blowers.value(blowerName)->setRunning(false);
        }
    });
    connect(controlPanel, &BlowerControlPanel::runFBFailClicked, this, [this, blowerName]() {
        m_blowerRunning[blowerName] = false;
        if (m_blowers.contains(blowerName) && m_blowers.value(blowerName)) {
            m_blowers.value(blowerName)->setRunning(false);
        }
    });

    connect(controlPanel, &BlowerControlPanel::startClicked, this, [this, blowerName]() {
        m_blowerRunning[blowerName] = true;
        if (m_blowers.contains(blowerName) && m_blowers.value(blowerName)) {
            m_blowers.value(blowerName)->setRunning(true);
        }
    });
    connect(controlPanel, &BlowerControlPanel::runClicked, this, [this, blowerName]() {
        m_blowerRunning[blowerName] = true;
        if (m_blowers.contains(blowerName) && m_blowers.value(blowerName)) {
            m_blowers.value(blowerName)->setRunning(true);
        }
    });
    connect(controlPanel, &BlowerControlPanel::runCmdClicked, this, [this, blowerName]() {
        m_blowerRunning[blowerName] = true;
        if (m_blowers.contains(blowerName) && m_blowers.value(blowerName)) {
            m_blowers.value(blowerName)->setRunning(true);
        }
    });

    controlPanel->exec();
    delete controlPanel;
}
