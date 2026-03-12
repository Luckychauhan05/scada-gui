#include "widgets/flowcomponents.h"

#include <QBrush>
#include <QCursor>
#include <QFont>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QPainterPath>
#include <QPen>
#include <cmath>

Pipeline::Pipeline(const QVector<QPointF> &points, const QColor &color, qreal width, QGraphicsItem *parent)
    : QGraphicsPathItem(parent),
      m_baseColor(color),
      m_width(width)
{
    QPainterPath path;
    if (!points.isEmpty()) {
        path.moveTo(points.first());
        for (int i = 1; i < points.size(); ++i) {
            path.lineTo(points[i]);
        }
    }

    setPath(path);
    setPen(QPen(color, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

void Pipeline::setPulse(bool on)
{
    QColor pulseColor = m_baseColor;
    pulseColor = on ? pulseColor.lighter(122) : pulseColor.darker(106);
    setPen(QPen(pulseColor, m_width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

FlowArrow::FlowArrow(const QPointF &tip, qreal angleDeg, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{
    const qreal s = 10.0;
    QPolygonF tri;
    tri << QPointF(0, 0) << QPointF(-s, -s / 2.0) << QPointF(-s, s / 2.0);

    QTransform t;
    t.translate(tip.x(), tip.y());
    t.rotate(angleDeg);

    setPolygon(t.map(tri));
    setPen(Qt::NoPen);
    setBrush(QColor("#111111"));
}

void FlowArrow::setPulse(bool on)
{
    setBrush(on ? QColor("#111111") : QColor("#616161"));
}

AgitatorMotor::AgitatorMotor(const QString &label, qreal shaftLength, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
{
    auto *body = new QGraphicsRectItem(QRectF(0, 0, 110, 32));
    body->setPen(QPen(QColor("#2B5067"), 1.6));
    body->setBrush(QBrush(QColor("#D9EAF3")));
    addToGroup(body);

    auto *drive = new QGraphicsRectItem(QRectF(110, 7, 20, 18));
    drive->setPen(QPen(QColor("#2B5067"), 1.2));
    drive->setBrush(QBrush(QColor("#C4D7E2")));
    addToGroup(drive);

    auto *text = new QGraphicsTextItem(label);
    text->setDefaultTextColor(QColor("#12394D"));
    text->setFont(QFont("Segoe UI", 8, QFont::Bold));
    text->setPos(8, -20);
    addToGroup(text);

    auto *shaft = new QGraphicsLineItem(QLineF(55, 32, 55, 32 + shaftLength));
    shaft->setPen(QPen(QColor("#31566C"), 3));
    addToGroup(shaft);

    const qreal y = 32 + shaftLength - 10;
    auto *hub = new QGraphicsEllipseItem(QRectF(47, y - 7, 16, 16));
    hub->setPen(QPen(QColor("#31566C"), 1.2));
    hub->setBrush(QBrush(QColor("#BDD3DF")));
    addToGroup(hub);

    auto *blade1 = new QGraphicsLineItem(QLineF(30, y, 80, y));
    auto *blade2 = new QGraphicsLineItem(QLineF(35, y - 10, 75, y + 10));
    auto *blade3 = new QGraphicsLineItem(QLineF(35, y + 10, 75, y - 10));
    blade1->setPen(QPen(QColor("#3B6278"), 2));
    blade2->setPen(QPen(QColor("#3B6278"), 2));
    blade3->setPen(QPen(QColor("#3B6278"), 2));
    addToGroup(blade1);
    addToGroup(blade2);
    addToGroup(blade3);
}

MixerMotor::MixerMotor(const QString &label, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
{
    auto *motor = new QGraphicsRectItem(QRectF(0, 0, 92, 30));
    motor->setPen(QPen(QColor("#2A4D63"), 1.4));
    motor->setBrush(QBrush(QColor("#DDECF4")));
    addToGroup(motor);

    auto *text = new QGraphicsTextItem(label);
    text->setFont(QFont("Segoe UI", 8, QFont::Bold));
    text->setDefaultTextColor(QColor("#12384C"));
    text->setPos(12, -20);
    addToGroup(text);

    auto *shaft = new QGraphicsLineItem(QLineF(46, 30, 46, 94));
    shaft->setPen(QPen(QColor("#31566C"), 3));
    addToGroup(shaft);

    auto *wheel = new QGraphicsEllipseItem(QRectF(32, 84, 28, 28));
    wheel->setPen(QPen(QColor("#32586D"), 1.5));
    wheel->setBrush(QBrush(QColor("#BCD2DF")));
    addToGroup(wheel);

    auto *cross1 = new QGraphicsLineItem(QLineF(26, 98, 66, 98));
    auto *cross2 = new QGraphicsLineItem(QLineF(46, 78, 46, 118));
    cross1->setPen(QPen(QColor("#3C637A"), 2));
    cross2->setPen(QPen(QColor("#3C637A"), 2));
    addToGroup(cross1);
    addToGroup(cross2);
}

// ============ BLOWER COMPONENT ============
Blower::Blower(const QString &label, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent)
{
    // Compact centrifugal blower so stacked units leave readable label gaps.
    auto *housing = new QGraphicsEllipseItem(QRectF(8, 4, 56, 56));
    housing->setPen(QPen(QColor("#4A6A8A"), 2));
    
    // Create gradient for 3D effect
    QRadialGradient grad(36, 32, 30);
    grad.setColorAt(0.0, QColor("#A8D5F7"));    // Light blue center
    grad.setColorAt(0.7, QColor("#5A8FC7"));    // Medium blue
    grad.setColorAt(1.0, QColor("#3A5A8A"));    // Dark blue edge
    housing->setBrush(QBrush(grad));
    addToGroup(housing);
    
    // Impeller blade pattern (6 curved blades like turbine wheel)
    for (int i = 0; i < 6; ++i) {
        qreal angle = (i * 60.0) * (M_PI / 180.0);
        qreal innerR = 8;
        qreal outerR = 22;
        
        qreal x1 = 36 + innerR * cos(angle);
        qreal y1 = 32 + innerR * sin(angle);
        qreal x2 = 36 + outerR * cos(angle);
        qreal y2 = 32 + outerR * sin(angle);
        
        auto *blade = new QGraphicsLineItem(QLineF(x1, y1, x2, y2));
        blade->setPen(QPen(QColor("#1A3A5A"), 1.7));
        addToGroup(blade);
        
        // Secondary line for blade depth
        qreal angle2 = angle + 15 * (M_PI / 180.0);
        qreal x3 = 36 + innerR * cos(angle2);
        qreal y3 = 32 + innerR * sin(angle2);
        qreal x4 = 36 + outerR * cos(angle2);
        qreal y4 = 32 + outerR * sin(angle2);
        
        auto *bladeShadow = new QGraphicsLineItem(QLineF(x3, y3, x4, y4));
        bladeShadow->setPen(QPen(QColor("#2A4A6A"), 1.2));
        addToGroup(bladeShadow);
    }
    
    // Center hub
    auto *hub = new QGraphicsEllipseItem(QRectF(29, 25, 14, 14));
    hub->setPen(QPen(QColor("#2A4A6A"), 1.5));
    hub->setBrush(QBrush(QColor("#6A8AC7")));
    addToGroup(hub);
    
    // Discharge outlet (right side), centered at y=30 for pipe alignment.
    auto *outlet = new QGraphicsRectItem(QRectF(64, 24, 16, 12));
    outlet->setPen(QPen(QColor("#4A6A8A"), 1.5));
    outlet->setBrush(QBrush(QColor("#7A9BC7")));
    addToGroup(outlet);
    
    // Outlet connection line
    auto *outletLine = new QGraphicsLineItem(QLineF(64, 30, 58, 30));
    outletLine->setPen(QPen(QColor("#4A6A8A"), 2));
    addToGroup(outletLine);
    
    // Motor/bearing housing (upper right)
    auto *motor = new QGraphicsRectItem(QRectF(64, 4, 16, 16));
    motor->setPen(QPen(QColor("#3A5A7A"), 1.5));
    motor->setBrush(QBrush(QColor("#8AABC7")));
    addToGroup(motor);
    
    // Motor coupling/connection
    auto *coupling = new QGraphicsLineItem(QLineF(56, 12, 64, 12));
    coupling->setPen(QPen(QColor("#2A4A6A"), 2));
    addToGroup(coupling);
    
    // Label below blower with compact footprint to avoid overlap in stack.
    auto *text = new QGraphicsTextItem(label);
    text->setFont(QFont("Segoe UI", 8, QFont::Bold));
    text->setDefaultTextColor(QColor("#0D2A38"));
    text->setPos(8, 58);
    addToGroup(text);
}

// ============ MANUAL VALVE INDICATOR ============
// P&ID symbol: motor actuator box [M] on top, bowtie valve body below.
ManualValveIndicator::ManualValveIndicator(const QString &name, bool open, QGraphicsItem *parent)
    : QGraphicsItemGroup(parent),
      m_name(name),
      m_open(open)
{
    setAcceptedMouseButtons(Qt::LeftButton);
    setFlags(QGraphicsItem::ItemIsSelectable);
    setCursor(QCursor(Qt::PointingHandCursor));
    // Open = light green fill, Closed = light red fill on valve triangles
    const QColor valveColor = m_open ? QColor("#90EE90") : QColor("#F08080");
    const QPen bodyPen(QColor("#1A3A5A"), 1.5);

    // ── Motor actuator box with "M" ──────────────────────────────────────
    auto *mbox = new QGraphicsRectItem(QRectF(2, 0, 16, 12));
    mbox->setPen(bodyPen);
    mbox->setBrush(QBrush(QColor("#D4EAF8")));
    addToGroup(mbox);

    auto *mtext = new QGraphicsTextItem("M");
    mtext->setFont(QFont("Segoe UI", 6, QFont::Bold));
    mtext->setDefaultTextColor(QColor("#0D2A38"));
    mtext->setPos(4, -2);
    addToGroup(mtext);

    // ── Short line: box bottom → valve centre ────────────────────────────
    auto *topLine = new QGraphicsLineItem(QLineF(10, 12, 10, 18));
    topLine->setPen(bodyPen);
    addToGroup(topLine);

    // ── Left triangle: base on left, tip pointing right to (10, 23) ─────
    QPolygonF leftTri;
    leftTri << QPointF(0, 18) << QPointF(0, 28) << QPointF(10, 23);
    auto *leftPoly = new QGraphicsPolygonItem(leftTri);
    leftPoly->setPen(bodyPen);
    leftPoly->setBrush(QBrush(valveColor));
    addToGroup(leftPoly);

    // ── Right triangle: base on right, tip pointing left to (10, 23) ────
    QPolygonF rightTri;
    rightTri << QPointF(20, 18) << QPointF(20, 28) << QPointF(10, 23);
    auto *rightPoly = new QGraphicsPolygonItem(rightTri);
    rightPoly->setPen(bodyPen);
    rightPoly->setBrush(QBrush(valveColor));
    addToGroup(rightPoly);

    // ── Short line: valve centre → bottom ───────────────────────────────
    auto *botLine = new QGraphicsLineItem(QLineF(10, 23, 10, 32));
    botLine->setPen(bodyPen);
    addToGroup(botLine);

    // ── Name label ───────────────────────────────────────────────────────
    auto *label = new QGraphicsTextItem(m_name);
    label->setFont(QFont("Segoe UI", 7, QFont::Bold));
    label->setDefaultTextColor(QColor("#0D2A38"));
    label->setPos(-3, 32);
    addToGroup(label);
}

void ManualValveIndicator::setOpen(bool open)
{
    if (m_open == open) return;
    m_open = open;

    const QColor valveColor = m_open ? QColor("#90EE90") : QColor("#F08080");

    // Item order: mbox=0, mtext=1, topLine=2, leftPoly=3, rightPoly=4, botLine=5, label=6
    QList<QGraphicsItem *> items = this->childItems();
    for (int idx : {3, 4}) {
        if (idx < items.size()) {
            if (auto *poly = dynamic_cast<QGraphicsPolygonItem *>(items[idx]))
                poly->setBrush(QBrush(valveColor));
        }
    }
}

void ManualValveIndicator::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event)
    setOpen(!m_open);          // toggle: green → red or red → green
    update();
}
