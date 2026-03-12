#ifndef PROCESSMIMICWIDGET_H
#define PROCESSMIMICWIDGET_H

#include <QWidget>

class QTimer;
class DataSimulator;

class ProcessMimicWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ProcessMimicWidget(QWidget *parent = nullptr);

    void setDataSimulator(DataSimulator *simulator);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private slots:
    void refreshData();
    void advanceAnimation();

private:
    DataSimulator *m_simulator;
    QTimer *m_animationTimer;
    double m_tankA;
    double m_tankB;
    double m_flowRate;
    double m_temperature;
    double m_pressure;
    bool m_pump1Running;
    bool m_valveAOpen;
    int m_flowPhase;
    bool m_alarmFlashOn;
    int m_healthState;
};

#endif // PROCESSMIMICWIDGET_H
