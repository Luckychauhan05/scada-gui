#ifndef PROCESSFLOWVIEW_H
#define PROCESSFLOWVIEW_H

#include <QGraphicsView>
#include <QList>
#include <QMap>

class QGraphicsScene;
class QGraphicsEllipseItem;
class FlowArrow;
class Pipeline;
class Blower;
class QTimer;
class ValveIndicator;
class ManualValveIndicator;

class ProcessFlowView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit ProcessFlowView(QWidget *parent = nullptr);

public slots:
    void toggleAlarmState();
    void resetState();

private slots:
    void animateFlow();
    void animateFlowDots();

protected:
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    struct ControlGate {
        QList<QString> valves;
        QList<QString> blowers;
    };

    struct ControlledPipeline {
        Pipeline *item;
        ControlGate gate;
    };

    void buildScene();
    void addEquipment(const QString &title, const QRectF &rect, const QColor &fill, int pointSize = 9);
    void addPipeline(const QVector<QPointF> &points,
                     const QColor &color,
                     qreal width = 8.0,
                     const QList<QString> &controllingValves = {},
                     const QList<QString> &controllingBlowers = {});
    void addArrow(const QPointF &tip, qreal angleDeg, const QColor &color);
    void addValve(const QString &name, const QPointF &scenePos, bool open);
    void addBlower(const QString &name, const QPointF &pos);
    void onBlowerClicked(const QString &blowerName);
    bool isGateOpen(const ControlGate &gate) const;

    QGraphicsScene *m_scene;
    QList<Pipeline *> m_pipelines;
    QList<ControlledPipeline> m_controlledPipelines;
    QList<FlowArrow *> m_arrows;
    QMap<QString, ValveIndicator *> m_valves;
    QMap<QString, ManualValveIndicator *> m_mvValves;
    QMap<QString, Blower *> m_blowers;
    QMap<QString, QPointF> m_blowerPositions;
    QMap<QString, bool> m_blowerRunning;
    QTimer *m_flowTimer;
    struct FlowDot {
        QGraphicsEllipseItem *item;
        QList<QPointF> path;
        qreal t;
        qreal speed;
        QList<QString> controllingValves;
        QList<QString> controllingBlowers;
        QColor baseColor;
    };
    QList<FlowDot> m_flowDots;
    QTimer *m_dotTimer;
    bool m_alarmState;
    bool m_flowPulse;
};

#endif // PROCESSFLOWVIEW_H
