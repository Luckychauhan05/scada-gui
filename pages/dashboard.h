#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>

class QGridLayout;
class QLabel;
class SensorCard;
class StatusIndicator;
class TankWidget;
class DataSimulator;

class DashboardPage : public QWidget
{
    Q_OBJECT
public:
    explicit DashboardPage(QWidget *parent = nullptr);
    void setDataSimulator(DataSimulator *simulator);

public slots:
    void updateAlarmSummary(int activeAlarms, int criticalAlarms);

private slots:
    void refreshFromSimulator();

private:
    QWidget *createCard(const QString &title, QWidget *content);

    TankWidget *m_tankA;
    TankWidget *m_tankB;

    StatusIndicator *m_pump1Status;
    StatusIndicator *m_pump2Status;

    SensorCard *m_flowCard;
    SensorCard *m_tempCard;
    SensorCard *m_pressureCard;

    QLabel *m_systemHealthLabel;
    QLabel *m_alarmSummaryLabel;

    DataSimulator *m_simulator;
};

#endif // DASHBOARD_H
