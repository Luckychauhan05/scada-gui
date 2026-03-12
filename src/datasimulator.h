#ifndef DATASIMULATOR_H
#define DATASIMULATOR_H

#include <QObject>

#include <QDateTime>
#include <QVector>

class QTimer;

class DataSimulator : public QObject
{
    Q_OBJECT
public:
    enum class HealthState {
        Healthy,
        Warning,
        Critical
    };

    struct DeviceSnapshot {
        QString name;
        QString type;
        QString value;
        bool online;
        QDateTime lastUpdate;
    };

    explicit DataSimulator(QObject *parent = nullptr);

    void start(int intervalMs = 1000);

    double tankALevel() const;
    double tankBLevel() const;
    double flowRate() const;
    double temperature() const;
    double pressure() const;

    bool pump1Running() const;
    bool pump2Running() const;
    bool valveAOpen() const;

    void setPump1Running(bool running);
    void setValveAOpen(bool open);
    void emergencyStop();

    QVector<DeviceSnapshot> deviceSnapshots() const;
    HealthState healthState() const;
    QString healthMessage() const;

signals:
    void dataUpdated();

private slots:
    void simulateStep();

private:
    QTimer *m_timer;

    double m_tankA;
    double m_tankB;
    double m_flowRate;
    double m_temperature;
    double m_pressure;

    bool m_pump1Running;
    bool m_pump2Running;
    bool m_valveAOpen;

    bool m_levelAOnline;
    bool m_levelBOnline;
    bool m_flowOnline;
    bool m_pressureOnline;
    bool m_pump1Online;
    bool m_pump2Online;
    bool m_valveAOnline;
};

#endif // DATASIMULATOR_H
