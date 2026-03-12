#ifndef ALARMS_H
#define ALARMS_H

#include <QWidget>

class QPushButton;
class QTableWidget;
class QTimer;

class AlarmManagerPage : public QWidget
{
    Q_OBJECT
public:
    explicit AlarmManagerPage(QWidget *parent = nullptr);

signals:
    void alarmStatsChanged(int activeAlarms, int criticalAlarms);
    void criticalAlarmRaised(const QString &message);
    void eventOccurred(const QString &source, const QString &message, const QString &severity);

private slots:
    void generateAlarm();
    void acknowledgeSelectedAlarm();

private:
    void refreshStats();

    QTableWidget *m_alarmTable;
    QPushButton *m_ackButton;
    QTimer *m_alarmTimer;
};

#endif // ALARMS_H
