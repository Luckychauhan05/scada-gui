#ifndef ALUMCLARIFLOCCULATORVIEW_H
#define ALUMCLARIFLOCCULATORVIEW_H

#include <QGraphicsView>
#include <QGraphicsItemGroup>
#include <QList>
#include <QRectF>

class QGraphicsScene;
class QGraphicsEllipseItem;
class FlowArrow;
class Pipeline;
class QTimer;

class AlumClariflocculatorView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit AlumClariflocculatorView(QWidget *parent = nullptr);

private slots:
    void animateArrows();
    void animateRotation();
    void animateWater();
    void animateFlowDots();
    void onPump1Started();
    void onPump1Stopped();
    void onPump2Started();
    void onPump2Stopped();

private:
    void buildScene();

 protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    QGraphicsScene *m_scene;
    QList<Pipeline *> m_pipelines;
    struct ControlledPipeline {
        Pipeline *item = nullptr;
        int pumpMask = 0; // bit0: pump1, bit1: pump2
    };
    QList<ControlledPipeline> m_controlledPipelines;
    QList<FlowArrow *> m_arrows;
    QTimer *m_arrowTimer;
    bool m_arrowPulse;

    // Rotating agitator groups
    QList<QGraphicsItemGroup *> m_agitatorGroups;
    QList<QPointF> m_agitatorCenters;
    QTimer *m_rotationTimer;
    qreal m_rotationAngle;

    // Water ripple animation
    QList<QGraphicsEllipseItem *> m_ripples;
    QTimer *m_waterTimer;
    qreal m_ripplePhase;

    // Flow dot animation
    // pumpId: 0 = always running, 1 = gated by pump1, 2 = gated by pump2
    struct FlowDot {
        QGraphicsEllipseItem *item;
        QList<QPointF> path;
        qreal t;
        qreal speed;
        int pumpId = 0;
        QColor baseColor;
    };
    QList<FlowDot> m_flowDots;
    QTimer *m_dotTimer;

    // Dedicated agitator pointers for alum tanks (used to conditionally stop them)
    QGraphicsItemGroup *m_alumAgitatorOne = nullptr;
    QGraphicsItemGroup *m_alumAgitatorTwo = nullptr;

    // Clickable alum tank rects (stored from buildScene)
    QRectF m_alumTankOneRect;
    QRectF m_alumTankTwoRect;

    // Pump running state
    bool m_pump1Running = true;
    bool m_pump2Running = true;

    bool isPumpMaskRunning(int pumpMask) const;
};

#endif // ALUMCLARIFLOCCULATORVIEW_H
