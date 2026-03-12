#include "widgets/alumclariflocculatorview.h"

#include "widgets/flowcomponents.h"
#include "widgets/pumpcontrolfaceplate.h"

#include <QBrush>
#include <QGraphicsEllipseItem>
#include <QGraphicsItemGroup>
#include <QGraphicsPathItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QLinearGradient>
#include <QMouseEvent>
#include <QPainterPath>
#include <QPen>
#include <QTimer>
#include <QtMath>

namespace {

enum SceneLayer {
    PipeLayer = 1,
    TankLayer = 2,
    AgitatorLayer = 3,
    MotorLayer = 4,
    LabelLayer = 5
};

void setLayer(QGraphicsItem *item, qreal layer)
{
    if (item) {
        item->setZValue(layer);
    }
}

QGraphicsTextItem *addText(QGraphicsScene *scene,
                           const QString &text,
                           const QFont &font,
                           const QColor &color,
                           const QPointF &pos,
                           qreal layer)
{
    auto *item = scene->addText(text, font);
    item->setDefaultTextColor(color);
    item->setPos(pos);
    setLayer(item, layer);
    return item;
}

QGraphicsTextItem *addCenteredText(QGraphicsScene *scene,
                                   const QString &text,
                                   const QFont &font,
                                   const QColor &color,
                                   const QRectF &rect,
                                   qreal layer,
                                   qreal yOffset = 0.0)
{
    auto *item = scene->addText(text, font);
    item->setDefaultTextColor(color);
    const QRectF bounds = item->boundingRect();
    item->setPos(rect.center().x() - bounds.width() / 2.0,
                 rect.center().y() - bounds.height() / 2.0 + yOffset);
    setLayer(item, layer);
    return item;
}

void addRoundedTank(QGraphicsScene *scene, const QRectF &rect, const QColor &fill)
{
    QPainterPath path;
    path.addRoundedRect(rect, 22, 22);
    auto *body = scene->addPath(path, QPen(QColor("#2D5F78"), 2.2), QBrush(fill));
    setLayer(body, TankLayer);

    auto *shine = scene->addRect(QRectF(rect.left() + 12, rect.top() + 12, 18, rect.height() - 24),
                                 QPen(Qt::NoPen),
                                 QBrush(QColor(255, 255, 255, 55)));
    setLayer(shine, TankLayer);
}

void addCylindricalTank(QGraphicsScene *scene, const QRectF &rect, const QColor &fill)
{
    QPainterPath shell;
    shell.addRoundedRect(rect, rect.height() / 2.0, rect.height() / 2.0);
    auto *body = scene->addPath(shell, QPen(QColor("#2C617A"), 2.2), QBrush(fill));
    setLayer(body, TankLayer);

    auto *leftCap = scene->addEllipse(QRectF(rect.left() + 10, rect.top() + 10, 28, rect.height() - 20),
                                      QPen(QColor("#8DB9C8"), 1.1),
                                      QBrush(QColor(255, 255, 255, 40)));
    auto *rightCap = scene->addEllipse(QRectF(rect.right() - 38, rect.top() + 10, 28, rect.height() - 20),
                                       QPen(QColor("#8DB9C8"), 1.1),
                                       QBrush(QColor(255, 255, 255, 30)));
    setLayer(leftCap, TankLayer);
    setLayer(rightCap, TankLayer);
}

void addJunctionDot(QGraphicsScene *scene, const QPointF &center, const QColor &color)
{
    auto *dot = scene->addEllipse(QRectF(center.x() - 5, center.y() - 5, 10, 10),
                                  QPen(Qt::NoPen),
                                  QBrush(color));
    setLayer(dot, PipeLayer + 0.1);
}

void addMotor(QGraphicsScene *scene, const QPointF &center, const QString &label)
{
    const QRectF body(center.x() - 42, center.y(), 84, 28);
    QPainterPath housing;
    housing.addRoundedRect(body, 6, 6);
    auto *motorBody = scene->addPath(housing, QPen(QColor("#274C61"), 1.8), QBrush(QColor("#DDEBF3")));
    setLayer(motorBody, MotorLayer);

    auto *cap = scene->addRect(QRectF(center.x() - 16, center.y() - 6, 32, 6),
                               QPen(QColor("#274C61"), 1.2),
                               QBrush(QColor("#C7D7E0")));
    auto *outputShaft = scene->addRect(QRectF(center.x() - 5, center.y() + 28, 10, 12),
                                       QPen(QColor("#274C61"), 1.2),
                                       QBrush(QColor("#BACDD8")));
    setLayer(cap, MotorLayer);
    setLayer(outputShaft, MotorLayer);

    addCenteredText(scene,
                    label,
                    QFont("Segoe UI", 8, QFont::Bold),
                    QColor("#12384D"),
                    QRectF(center.x() - 70, center.y() - 28, 140, 18),
                    LabelLayer);
}

void addShaft(QGraphicsScene *scene, const QPointF &start, const QPointF &end)
{
    auto *shaft = scene->addLine(QLineF(start, end), QPen(QColor("#1F4B63"), 4.2, Qt::SolidLine, Qt::RoundCap));
    setLayer(shaft, AgitatorLayer);
}

// Returns a group that can be rotated around (center)
QGraphicsItemGroup *addPropeller(QGraphicsScene *scene, const QPointF &center, qreal radius)
{
    QList<QGraphicsItem *> items;

    auto *mixZone = scene->addEllipse(QRectF(center.x() - radius - 2,
                                             center.y() - radius - 2,
                                             (radius + 2) * 2,
                                             (radius + 2) * 2),
                                      QPen(Qt::NoPen),
                                      QBrush(QColor(120, 178, 202, 85)));
    setLayer(mixZone, AgitatorLayer - 0.05);
    items << mixZone;

    auto *swirl = scene->addEllipse(QRectF(center.x() - radius - 10,
                                           center.y() - radius - 10,
                                           (radius + 10) * 2,
                                           (radius + 10) * 2),
                                    QPen(QColor("#2D6E88"), 1.8, Qt::DashLine),
                                    Qt::NoBrush);
    setLayer(swirl, AgitatorLayer);
    items << swirl;

    auto *innerRing = scene->addEllipse(QRectF(center.x() - radius + 4,
                                               center.y() - radius + 4,
                                               (radius - 4) * 2,
                                               (radius - 4) * 2),
                                        QPen(QColor("#2D6E88"), 1.3),
                                        Qt::NoBrush);
    setLayer(innerRing, AgitatorLayer);
    items << innerRing;

    const QPolygonF bladeShape {
        QPointF(4, 0),
        QPointF(radius - 6, -11),
        QPointF(radius + 12, 0),
        QPointF(radius - 6, 11)
    };

    for (int index = 0; index < 3; ++index) {
        QTransform transform;
        transform.translate(center.x(), center.y());
        transform.rotate(index * 120.0 + 18.0);
        auto *blade = scene->addPolygon(transform.map(bladeShape),
                                        QPen(QColor("#0E3549"), 2.6),
                                        QBrush(QColor("#63A3BE")));
        setLayer(blade, AgitatorLayer);
        items << blade;
    }

    auto *hub = scene->addEllipse(QRectF(center.x() - 10, center.y() - 10, 20, 20),
                                  QPen(QColor("#0E3549"), 2.0),
                                  QBrush(QColor("#E8F2F6")));
    setLayer(hub, AgitatorLayer);
    items << hub;

    auto *spokeA = scene->addLine(QLineF(center.x() - 7, center.y(), center.x() + 7, center.y()),
                                  QPen(QColor("#0E3549"), 1.6));
    auto *spokeB = scene->addLine(QLineF(center.x(), center.y() - 7, center.x(), center.y() + 7),
                                  QPen(QColor("#0E3549"), 1.6));
    setLayer(spokeA, AgitatorLayer);
    setLayer(spokeB, AgitatorLayer);
    items << spokeA << spokeB;

    auto *group = scene->createItemGroup(items);
    group->setZValue(AgitatorLayer);
    // Set transform origin so rotation is around center
    group->setTransformOriginPoint(center);
    return group;
}

QGraphicsItemGroup *addRakeArms(QGraphicsScene *scene, const QPointF &center)
{
    QList<QGraphicsItem *> items;

    auto *ring = scene->addEllipse(QRectF(center.x() - 34, center.y() - 34, 68, 68),
                                   QPen(QColor("#1F5A73"), 2.0),
                                   Qt::NoBrush);
    auto *armH = scene->addLine(QLineF(center.x() - 34, center.y(), center.x() + 34, center.y()),
                                QPen(QColor("#1F5A73"), 2.0));
    auto *armV = scene->addLine(QLineF(center.x(), center.y() - 34, center.x(), center.y() + 34),
                                QPen(QColor("#1F5A73"), 2.0));
    auto *armD1 = scene->addLine(QLineF(center.x() - 24, center.y() - 24, center.x() + 24, center.y() + 24),
                                 QPen(QColor("#1F5A73"), 1.6));
    auto *armD2 = scene->addLine(QLineF(center.x() - 24, center.y() + 24, center.x() + 24, center.y() - 24),
                                 QPen(QColor("#1F5A73"), 1.6));
    auto *hub = scene->addEllipse(QRectF(center.x() - 8, center.y() - 8, 16, 16),
                                  QPen(QColor("#0E3549"), 1.8),
                                  QBrush(QColor("#E7F1F6")));

    setLayer(ring, AgitatorLayer);
    setLayer(armH, AgitatorLayer);
    setLayer(armV, AgitatorLayer);
    setLayer(armD1, AgitatorLayer);
    setLayer(armD2, AgitatorLayer);
    setLayer(hub, AgitatorLayer);

    items << ring << armH << armV << armD1 << armD2 << hub;

    auto *group = scene->createItemGroup(items);
    group->setZValue(AgitatorLayer);
    group->setTransformOriginPoint(center);
    return group;
}

// Interpolate along a polyline (t 0..1 => position on path)
static QPointF polylinePoint(const QList<QPointF> &pts, qreal t)
{
    if (pts.size() < 2) return pts.isEmpty() ? QPointF() : pts[0];
    // compute total length
    qreal total = 0;
    for (int i = 1; i < pts.size(); ++i) {
        total += QLineF(pts[i-1], pts[i]).length();
    }
    qreal target = qBound(0.0, t, 1.0) * total;
    qreal acc = 0;
    for (int i = 1; i < pts.size(); ++i) {
        qreal seg = QLineF(pts[i-1], pts[i]).length();
        if (acc + seg >= target) {
            qreal frac = (seg > 0) ? (target - acc) / seg : 0.0;
            return QPointF(pts[i-1].x() + frac * (pts[i].x() - pts[i-1].x()),
                           pts[i-1].y() + frac * (pts[i].y() - pts[i-1].y()));
        }
        acc += seg;
    }
    return pts.last();
}

} // namespace

AlumClariflocculatorView::AlumClariflocculatorView(QWidget *parent)
    : QGraphicsView(parent),
      m_scene(new QGraphicsScene(this)),
      m_arrowTimer(new QTimer(this)),
      m_arrowPulse(false),
      m_rotationTimer(new QTimer(this)),
      m_rotationAngle(0.0),
      m_waterTimer(new QTimer(this)),
      m_ripplePhase(0.0),
      m_dotTimer(new QTimer(this))
{
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing, true);
    setFrameShape(QFrame::NoFrame);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setSceneRect(0, 0, 1620, 860);
    setMouseTracking(true);

    buildScene();

    connect(m_arrowTimer, &QTimer::timeout, this, &AlumClariflocculatorView::animateArrows);
    m_arrowTimer->start(450);

    // Agitator rotation: tick every 50ms → 7.2°/tick → 1 revolution ~2.5 s
    connect(m_rotationTimer, &QTimer::timeout, this, &AlumClariflocculatorView::animateRotation);
    m_rotationTimer->start(50);

    // Water ripple: tick every 80ms
    connect(m_waterTimer, &QTimer::timeout, this, &AlumClariflocculatorView::animateWater);
    m_waterTimer->start(80);

    // Flow dots: tick every 40ms
    connect(m_dotTimer, &QTimer::timeout, this, &AlumClariflocculatorView::animateFlowDots);
    m_dotTimer->start(40);
}

void AlumClariflocculatorView::animateArrows()
{
    m_arrowPulse = !m_arrowPulse;
    for (Pipeline *pipe : m_pipelines) {
        if (pipe) {
            pipe->setPulse(m_arrowPulse);
        }
    }
    for (const ControlledPipeline &entry : m_controlledPipelines) {
        if (entry.item) {
            const bool running = isPumpMaskRunning(entry.pumpMask);
            // Freeze at light state when related pump path is stopped.
            entry.item->setPulse(running ? m_arrowPulse : true);
        }
    }
    for (FlowArrow *arrow : m_arrows) {
        if (arrow) {
            arrow->setPulse(m_arrowPulse);
        }
    }
}

bool AlumClariflocculatorView::isPumpMaskRunning(int pumpMask) const
{
    if (pumpMask == 0) {
        return true;
    }
    bool running = false;
    if (pumpMask & 0x1) {
        running = running || m_pump1Running;
    }
    if (pumpMask & 0x2) {
        running = running || m_pump2Running;
    }
    return running;
}

void AlumClariflocculatorView::animateRotation()
{
    // 7.2° per 50ms tick = one revolution in 2.5s
    m_rotationAngle += 7.2;
    if (m_rotationAngle >= 360.0) {
        m_rotationAngle -= 360.0;
    }
    for (QGraphicsItemGroup *grp : m_agitatorGroups) {
        if (grp) {
            grp->setRotation(m_rotationAngle);
        }
    }
}

void AlumClariflocculatorView::animateWater()
{
    m_ripplePhase += 0.12;
    // Animate 3 concentric ripple ellipses with oscillating opacity and scale
    for (int i = 0; i < m_ripples.size(); ++i) {
        QGraphicsEllipseItem *r = m_ripples[i];
        if (!r) continue;
        // Each ripple has a phase offset
        qreal phase = m_ripplePhase + i * (M_PI * 2.0 / m_ripples.size());
        qreal alpha = 30.0 + 50.0 * (0.5 + 0.5 * qSin(phase));
        QColor c(100, 190, 215, static_cast<int>(alpha));
        r->setBrush(QBrush(c));
        // Gently animate stroke
        QPen p(QColor(80, 160, 200, static_cast<int>(alpha * 1.5)), 1.5);
        r->setPen(p);
    }
}

void AlumClariflocculatorView::animateFlowDots()
{
    for (FlowDot &dot : m_flowDots) {
        if (!dot.item) continue;
        bool running = true;
        if (dot.pumpId == 1) running = m_pump1Running;
        else if (dot.pumpId == 2) running = m_pump2Running;
        const QColor displayColor = dot.baseColor.isValid()
            ? (running ? dot.baseColor.lighter(140) : dot.baseColor.darker(180))
            : QColor(Qt::white);
        dot.item->setBrush(QBrush(displayColor));
        if (!running) continue;  // freeze position when stopped
        dot.t += dot.speed;
        if (dot.t > 1.0) dot.t -= 1.0;
        QPointF pos = polylinePoint(dot.path, dot.t);
        dot.item->setPos(pos.x() - 5, pos.y() - 5);
    }
}

void AlumClariflocculatorView::onPump1Started() { m_pump1Running = true; }
void AlumClariflocculatorView::onPump1Stopped() { m_pump1Running = false; }
void AlumClariflocculatorView::onPump2Started() { m_pump2Running = true; }
void AlumClariflocculatorView::onPump2Stopped() { m_pump2Running = false; }

void AlumClariflocculatorView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        const QPointF scenePos = mapToScene(event->pos());
        if (m_alumTankOneRect.contains(scenePos)) {
            PumpControlFaceplate dlg("ALUM PUMP-1", this);
            connect(&dlg, &PumpControlFaceplate::started, this, &AlumClariflocculatorView::onPump1Started);
            connect(&dlg, &PumpControlFaceplate::stopped, this, &AlumClariflocculatorView::onPump1Stopped);
            dlg.exec();
            return;
        }
        if (m_alumTankTwoRect.contains(scenePos)) {
            PumpControlFaceplate dlg("ALUM PUMP-2", this);
            connect(&dlg, &PumpControlFaceplate::started, this, &AlumClariflocculatorView::onPump2Started);
            connect(&dlg, &PumpControlFaceplate::stopped, this, &AlumClariflocculatorView::onPump2Stopped);
            dlg.exec();
            return;
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void AlumClariflocculatorView::mouseMoveEvent(QMouseEvent *event)
{
    const QPointF scenePos = mapToScene(event->pos());
    if (m_alumTankOneRect.contains(scenePos) || m_alumTankTwoRect.contains(scenePos))
        setCursor(Qt::PointingHandCursor);
    else
        unsetCursor();
    QGraphicsView::mouseMoveEvent(event);
}

void AlumClariflocculatorView::buildScene()
{
    m_scene->clear();
    m_pipelines.clear();
    m_controlledPipelines.clear();
    m_arrows.clear();
    m_agitatorGroups.clear();
    m_agitatorCenters.clear();
    m_ripples.clear();
    m_flowDots.clear();

    QLinearGradient bg(0, 0, 1620, 860);
    bg.setColorAt(0.0, QColor("#D8F3F7"));
    bg.setColorAt(1.0, QColor("#A7E0E7"));
    m_scene->setBackgroundBrush(bg);

    // ── Geometry ─────────────────────────────────────────────────────────────
    const QRectF backwashRect(580, 40, 440, 96);
    // Alum tanks moved down slightly so MOTOR labels have clear space above
    const QRectF alumTankOneRect(170, 240, 330, 96);
    const QRectF alumTankTwoRect(170, 390, 330, 96);
    m_alumTankOneRect = alumTankOneRect;
    m_alumTankTwoRect = alumTankTwoRect;
    const QRectF inletRect(700, 285, 160, 195);
    const QRectF flashRect(930, 285, 190, 195);
    const QRectF clarRect(1220, 235, 300, 355);

    // ── Background tanks ─────────────────────────────────────────────────────
    addRoundedTank(m_scene, backwashRect, QColor("#CBEAF2"));
    addCenteredText(m_scene,
            "BACKWASH WATER TANK",
            QFont("Segoe UI", 12, QFont::Bold),
            QColor("#10384C"),
            backwashRect,
            LabelLayer);

    // ALUM SOLUTION TANKS heading — shifted left so it doesn't crowd tank graphics
    addText(m_scene,
        "ALUM SOLUTION TANKS",
        QFont("Segoe UI", 11, QFont::Bold),
        QColor("#0E3449"),
        QPointF(78, 200),           // left-aligned, above the tanks
        LabelLayer);

    addCylindricalTank(m_scene, alumTankOneRect, QColor("#CFEAF3"));
    addCylindricalTank(m_scene, alumTankTwoRect, QColor("#CFEAF3"));

    // Tank labels placed below each tank, left-aligned to avoid overlapping caps
    addText(m_scene,
        "ALUM SOLUTION TANK 1",
        QFont("Segoe UI", 9, QFont::Bold),
        QColor("#12394D"),
        QPointF(alumTankOneRect.left() - 8, alumTankOneRect.bottom() + 8),
        LabelLayer);
    addText(m_scene,
        "ALUM SOLUTION TANK 2",
        QFont("Segoe UI", 9, QFont::Bold),
        QColor("#12394D"),
        QPointF(alumTankTwoRect.left() - 8, alumTankTwoRect.bottom() + 8),
        LabelLayer);

    addRoundedTank(m_scene, inletRect, QColor("#D5EBF2"));
    addCenteredText(m_scene,
            "INLET\nCHAMBER",
            QFont("Segoe UI", 11, QFont::Bold),
            QColor("#13374A"),
            inletRect,
            LabelLayer);

    addRoundedTank(m_scene, flashRect, QColor("#D2EEF5"));
    addCenteredText(m_scene,
            "FLASH MIXER",
            QFont("Segoe UI", 11, QFont::Bold),
            QColor("#12384D"),
            QRectF(flashRect.left(), flashRect.bottom() + 10, flashRect.width(), 20),
            LabelLayer);

    addRoundedTank(m_scene, clarRect, QColor("#CFDCE5"));

    // Water fill inside clariflocculator (upper half)
    auto *clarWater = m_scene->addRect(QRectF(clarRect.left() + 12,
                          clarRect.top() + 12,
                          clarRect.width() - 24,
                          clarRect.height() * 0.55),
                       QPen(Qt::NoPen),
                       QBrush(QColor("#9DD1DC")));
    setLayer(clarWater, TankLayer);

    // Water ripple ellipses inside clariflocculator
    const QPointF clarCenter(clarRect.center().x(), clarRect.top() + clarRect.height() * 0.3);
    for (int i = 0; i < 3; ++i) {
        qreal rw = 40 + i * 28;
        qreal rh = 12 + i * 8;
        auto *ripple = m_scene->addEllipse(QRectF(clarCenter.x() - rw, clarCenter.y() - rh, rw * 2, rh * 2),
                                           QPen(QColor(80, 160, 200, 60), 1.5),
                                           QBrush(QColor(100, 190, 215, 30)));
        setLayer(ripple, TankLayer + 0.1);
        m_ripples.append(ripple);
    }

    addCenteredText(m_scene,
            "CLARIFLOCCULATOR",
            QFont("Segoe UI", 12, QFont::Bold),
            QColor("#12384D"),
            QRectF(clarRect.left(), clarRect.bottom() - 44, clarRect.width(), 20),
            LabelLayer);

    // ── Motor row y-coords ────────────────────────────────────────────────────
    // motorY is well above the tanks to avoid crowding
    const qreal motorY     = 125.0;          // alum / clariflocculator motors
    const qreal flashMotorY = 210.0;         // flash mixer motor (separate, lower)

    const QPointF alumTankOneCenter = alumTankOneRect.center();
    const QPointF alumTankTwoCenter = alumTankTwoRect.center();

    // Flash agitator: upper portion of flash mixer (1/3 from top)
    const QPointF flashCenter(flashRect.center().x(),
                              flashRect.top() + flashRect.height() * 0.32);

    // Clariflocculator agitators: near the water surface (upper quarter)
    const QPointF clarBridgeCenter(clarRect.center().x(),
                                   clarRect.top() + clarRect.height() * 0.20);
    const QPointF clarAg5Center(clarRect.center().x() - 90,
                                clarRect.top() + clarRect.height() * 0.20);
    const QPointF clarAg6Center(clarRect.center().x() + 90,
                                clarRect.top() + clarRect.height() * 0.20);

    // ── Motors ───────────────────────────────────────────────────────────────
    addMotor(m_scene, QPointF(alumTankOneCenter.x(), motorY), "MOTOR 1");
    addMotor(m_scene, QPointF(alumTankTwoCenter.x(), motorY), "MOTOR 2");
    addMotor(m_scene, QPointF(flashCenter.x(), flashMotorY), "MIXER MOTOR");
    addMotor(m_scene, QPointF(clarAg5Center.x(), motorY), "AGITATOR 1");
    addMotor(m_scene, QPointF(clarBridgeCenter.x(), motorY), "BRIDGE MOTOR");
    addMotor(m_scene, QPointF(clarAg6Center.x(), motorY), "AGITATOR 2");

    // ── Shafts ───────────────────────────────────────────────────────────────
    addShaft(m_scene, QPointF(alumTankOneCenter.x(), motorY + 40), alumTankOneCenter);
    addShaft(m_scene, QPointF(alumTankTwoCenter.x(), motorY + 40), alumTankTwoCenter);
    addShaft(m_scene, QPointF(flashCenter.x(), flashMotorY + 40), flashCenter);
    addShaft(m_scene, QPointF(clarAg5Center.x(), motorY + 40), clarAg5Center);
    addShaft(m_scene, QPointF(clarBridgeCenter.x(), motorY + 40), clarBridgeCenter);
    addShaft(m_scene, QPointF(clarAg6Center.x(), motorY + 40), clarAg6Center);

    // ── Agitators (rotating groups) ──────────────────────────────────────────
    m_agitatorGroups << addPropeller(m_scene, alumTankOneCenter, 34);
    m_agitatorGroups << addPropeller(m_scene, alumTankTwoCenter, 34);
    m_agitatorGroups << addPropeller(m_scene, flashCenter, 40);
    m_agitatorGroups << addPropeller(m_scene, clarAg5Center, 32);
    m_agitatorGroups << addRakeArms(m_scene, clarBridgeCenter);
    m_agitatorGroups << addPropeller(m_scene, clarAg6Center, 32);

    // ── Pipes ────────────────────────────────────────────────────────────────
    auto addPipe = [this](const QVector<QPointF> &points, const QColor &color, qreal width = 7.0) {
        auto *pipe = new Pipeline(points, color, width);
        pipe->setZValue(PipeLayer);
        m_scene->addItem(pipe);
        m_pipelines.append(pipe);
        return pipe;
    };
    auto addControlledPipe = [this, &addPipe](const QVector<QPointF> &points,
                                              const QColor &color,
                                              int pumpMask,
                                              qreal width = 7.0) {
        Pipeline *pipe = addPipe(points, color, width);
        m_controlledPipelines.append({pipe, pumpMask});
    };
    auto addArrow = [this](const QPointF &tip, qreal angle) {
        auto *arrow = new FlowArrow(tip, angle);
        arrow->setZValue(PipeLayer + 0.2);
        m_scene->addItem(arrow);
        m_arrows.append(arrow);
    };

    // Helper to register a flow dot on a polyline path
    auto addFlowDot = [this](const QList<QPointF> &path, const QColor &color, qreal startT, qreal speed = 0.0035, int pumpId = 0) {
        auto *dot = m_scene->addEllipse(QRectF(0, 0, 10, 10),
                                        QPen(Qt::NoPen),
                                        QBrush(color.lighter(140)));
        dot->setZValue(PipeLayer + 0.5);
        FlowDot fd;
        fd.item      = dot;
        fd.path      = path;
        fd.t         = startT;
        fd.speed     = speed;
        fd.pumpId    = pumpId;
        fd.baseColor = color;
        m_flowDots.append(fd);
    };

    const QColor feedColor("#2E7D32");
    const QColor dosingColor("#8E44AD");
    const QColor transferColor("#1E88E5");
    const QColor filterOutletColor("#F4C542");
    const QColor wasteOutletColor("#E53970");

    // ── Green backwash feed: vertical drop then horizontal to tank1, trunk to tank2 ──
    const qreal feedDropY = 172;
    const qreal feedTrunkX = alumTankOneCenter.x();

    QList<QPointF> feedPath1 = {
        QPointF(backwashRect.left() + 20, backwashRect.bottom()),
        QPointF(backwashRect.left() + 20, feedDropY),
        QPointF(feedTrunkX, feedDropY)
    };
    addPipe(QVector<QPointF>(feedPath1.begin(), feedPath1.end()), feedColor);

    QList<QPointF> feedPath2 = {
        QPointF(feedTrunkX, feedDropY),
        QPointF(feedTrunkX, alumTankOneRect.top()),
        QPointF(feedTrunkX, alumTankOneRect.bottom() + 10),
        QPointF(feedTrunkX, alumTankTwoRect.top())
    };
    addPipe(QVector<QPointF>(feedPath2.begin(), feedPath2.end()), feedColor);

    addJunctionDot(m_scene, QPointF(feedTrunkX, feedDropY), feedColor);
    addJunctionDot(m_scene, QPointF(feedTrunkX, alumTankOneRect.top()), feedColor);
    addJunctionDot(m_scene, QPointF(feedTrunkX, alumTankTwoRect.top()), feedColor);

    addText(m_scene,
        "Backwash water distribution",
        QFont("Segoe UI", 8, QFont::Bold),
        QColor("#1D5C2A"),
        QPointF(560, 184),
        LabelLayer);

    // Flow dots on backwash feed
    QList<QPointF> feedFull = {
        QPointF(backwashRect.left() + 20, backwashRect.bottom()),
        QPointF(backwashRect.left() + 20, feedDropY),
        QPointF(feedTrunkX, feedDropY),
        QPointF(feedTrunkX, alumTankTwoRect.top())
    };
    addFlowDot(feedFull, feedColor, 0.0);
    addFlowDot(feedFull, feedColor, 0.5);

    // ── Purple dosing pipe ─────────────────────────────────────────────────────
    // Clean T-junction layout: both tanks share a vertical trunk, then a single
    // horizontal run enters the inlet chamber — no zig-zag bends.
    const qreal dosingTrunkX = alumTankOneRect.right() + 22; // shared vertical trunk
    const qreal dosingDropY  = alumTankTwoRect.bottom() + 28; // below tank 2
    const qreal dosingRunY   = inletRect.bottom() - 30;

    // Tank 1 branch: right edge -> trunk (pure horizontal)
    QList<QPointF> dosing1 = {
        QPointF(alumTankOneRect.right(), alumTankOneCenter.y()),
        QPointF(dosingTrunkX,           alumTankOneCenter.y())
    };
    addControlledPipe(QVector<QPointF>(dosing1.begin(), dosing1.end()), dosingColor, 0x1);

    // Tank 2 branch: right edge -> trunk (pure horizontal)
    QList<QPointF> dosing2 = {
        QPointF(alumTankTwoRect.right(), alumTankTwoCenter.y()),
        QPointF(dosingTrunkX,           alumTankTwoCenter.y())
    };
    addControlledPipe(QVector<QPointF>(dosing2.begin(), dosing2.end()), dosingColor, 0x2);

    // Vertical trunk: down from tank1 level through tank2 level, then across to inlet
    QList<QPointF> dosingTrunk = {
        QPointF(dosingTrunkX, alumTankOneCenter.y()),
        QPointF(dosingTrunkX, dosingDropY),
        QPointF(inletRect.left(), dosingDropY),
        QPointF(inletRect.left(), dosingRunY)
    };
    addControlledPipe(QVector<QPointF>(dosingTrunk.begin(), dosingTrunk.end()), dosingColor, 0x3);

    addJunctionDot(m_scene, QPointF(dosingTrunkX, alumTankOneCenter.y()), dosingColor);
    addJunctionDot(m_scene, QPointF(dosingTrunkX, alumTankTwoCenter.y()), dosingColor);
    addText(m_scene,
        "Chemical dosing line",
        QFont("Segoe UI", 8, QFont::Bold),
        QColor("#5D2B73"),
        QPointF(dosingTrunkX + 10, dosingDropY + 6),
        LabelLayer);

    // Flow dot follows the natural trunk path
    // Pump-1: dot starts from Tank 1 right edge, along trunk to inlet
    QList<QPointF> dosingFull1 = {
        QPointF(alumTankOneRect.right(), alumTankOneCenter.y()),
        QPointF(dosingTrunkX,           alumTankOneCenter.y()),
        QPointF(dosingTrunkX,           dosingDropY),
        QPointF(inletRect.left(),       dosingDropY),
        QPointF(inletRect.left(),       dosingRunY)
    };
    addFlowDot(dosingFull1, dosingColor, 0.1, 0.004, 1);

    // Pump-2: dot starts from Tank 2 right edge, merges into trunk, to inlet
    QList<QPointF> dosingFull2 = {
        QPointF(alumTankTwoRect.right(), alumTankTwoCenter.y()),
        QPointF(dosingTrunkX,           alumTankTwoCenter.y()),
        QPointF(dosingTrunkX,           dosingDropY),
        QPointF(inletRect.left(),       dosingDropY),
        QPointF(inletRect.left(),       dosingRunY)
    };
    addFlowDot(dosingFull2, dosingColor, 0.6, 0.004, 2);

    // ── Blue transfer: Inlet -> Flash -> Clariflocculator ─────────────────────
    QList<QPointF> inletToFlash = {
        QPointF(inletRect.right(), inletRect.center().y()),
        QPointF(flashRect.left(), inletRect.center().y())
    };
    addPipe(QVector<QPointF>(inletToFlash.begin(), inletToFlash.end()), transferColor);
    addFlowDot(inletToFlash, transferColor, 0.0, 0.006);

    QList<QPointF> flashToClar = {
        QPointF(flashRect.right(), flashRect.center().y()),
        QPointF(clarRect.left(), flashRect.center().y())
    };
    addPipe(QVector<QPointF>(flashToClar.begin(), flashToClar.end()), transferColor);
    addFlowDot(flashToClar, transferColor, 0.3, 0.006);

    // ── Filter outlet (yellow) ────────────────────────────────────────────────
    QList<QPointF> filterPath = {
        QPointF(clarRect.center().x() + 28, clarRect.bottom()),
        QPointF(clarRect.center().x() + 28, 690),
        QPointF(1545, 690)
    };
    addPipe(QVector<QPointF>(filterPath.begin(), filterPath.end()), filterOutletColor);
    addArrow(QPointF(1545, 690), 0);
    addFlowDot(filterPath, filterOutletColor, 0.0, 0.005);

    addText(m_scene,
        "TO FILTER BEDS",
        QFont("Segoe UI", 9, QFont::Bold),
        QColor("#6B5712"),
        QPointF(1416, 662),
        LabelLayer);

    // ── Waste outlet (pink) ───────────────────────────────────────────────────
    QList<QPointF> wastePath = {
        QPointF(clarRect.center().x() - 56, clarRect.bottom()),
        QPointF(clarRect.center().x() - 56, 765),
        QPointF(1545, 765)
    };
    addPipe(QVector<QPointF>(wastePath.begin(), wastePath.end()), wasteOutletColor);
    addArrow(QPointF(1545, 765), 0);
    addFlowDot(wastePath, wasteOutletColor, 0.5, 0.005);

    addText(m_scene,
        "TO WASTE WATER TANK",
        QFont("Segoe UI", 9, QFont::Bold),
        QColor("#8E2550"),
        QPointF(1328, 737),
        LabelLayer);
}
