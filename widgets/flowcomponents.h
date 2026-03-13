#ifndef FLOWCOMPONENTS_H
#define FLOWCOMPONENTS_H

#include <QGraphicsItemGroup>
#include <QGraphicsPathItem>
#include <QGraphicsPolygonItem>

class Pipeline : public QGraphicsPathItem
{
public:
    Pipeline(const QVector<QPointF> &points, const QColor &color, qreal width = 7.0, QGraphicsItem *parent = nullptr);

    void setPulse(bool on);

private:
    QColor m_baseColor;
    qreal m_width;
};

class FlowArrow : public QGraphicsPolygonItem
{
public:
    FlowArrow(const QPointF &tip, qreal angleDeg, QGraphicsItem *parent = nullptr);
    void setPulse(bool on);
};

class AgitatorMotor : public QGraphicsItemGroup
{
public:
    AgitatorMotor(const QString &label, qreal shaftLength, QGraphicsItem *parent = nullptr);
};

class MixerMotor : public QGraphicsItemGroup
{
public:
    explicit MixerMotor(const QString &label, QGraphicsItem *parent = nullptr);
};

class Blower : public QGraphicsItemGroup
{
public:
    explicit Blower(const QString &label, QGraphicsItem *parent = nullptr);
    void setRunning(bool running);
    void stepRotation(qreal stepDeg = 10.0);

private:
    QGraphicsItemGroup *m_rotorGroup = nullptr;
    bool m_running = true;
    qreal m_rotorAngleDeg = 0.0;
};

class ManualValveIndicator : public QGraphicsItemGroup
{
public:
    explicit ManualValveIndicator(const QString &name, bool open = true, QGraphicsItem *parent = nullptr);
    void setOpen(bool open);
    bool isOpen() const { return m_open; }

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    QString m_name;
    bool m_open;
};

#endif // FLOWCOMPONENTS_H
