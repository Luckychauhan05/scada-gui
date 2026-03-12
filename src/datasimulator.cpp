#include "src/datasimulator.h"

#include <QRandomGenerator>
#include <QTimer>

DataSimulator::DataSimulator(QObject *parent)
    : QObject(parent),
      m_timer(new QTimer(this)),
      m_tankA(58.0),
      m_tankB(42.0),
      m_flowRate(120.0),
      m_temperature(26.5),
      m_pressure(4.8),
      m_pump1Running(false),
      m_pump2Running(true),
      m_valveAOpen(false),
      m_levelAOnline(true),
      m_levelBOnline(true),
      m_flowOnline(true),
      m_pressureOnline(true),
      m_pump1Online(true),
      m_pump2Online(true),
      m_valveAOnline(true)
{
    connect(m_timer, &QTimer::timeout, this, &DataSimulator::simulateStep);
}

void DataSimulator::start(int intervalMs)
{
    m_timer->start(intervalMs);
    emit dataUpdated();
}

double DataSimulator::tankALevel() const
{
    return m_tankA;
}

double DataSimulator::tankBLevel() const
{
    return m_tankB;
}

double DataSimulator::flowRate() const
{
    return m_flowRate;
}

double DataSimulator::temperature() const
{
    return m_temperature;
}

double DataSimulator::pressure() const
{
    return m_pressure;
}

bool DataSimulator::pump1Running() const
{
    return m_pump1Running;
}

bool DataSimulator::pump2Running() const
{
    return m_pump2Running;
}

bool DataSimulator::valveAOpen() const
{
    return m_valveAOpen;
}

void DataSimulator::setPump1Running(bool running)
{
    m_pump1Running = running;
    emit dataUpdated();
}

void DataSimulator::setValveAOpen(bool open)
{
    m_valveAOpen = open;
    emit dataUpdated();
}

void DataSimulator::emergencyStop()
{
    m_pump1Running = false;
    m_valveAOpen = false;
    emit dataUpdated();
}

QVector<DataSimulator::DeviceSnapshot> DataSimulator::deviceSnapshots() const
{
    const QDateTime now = QDateTime::currentDateTime();

    return {
        {"Tank Level Sensor A", "Level Sensor", QString::number(m_tankA, 'f', 1) + " %", m_levelAOnline, now},
        {"Tank Level Sensor B", "Level Sensor", QString::number(m_tankB, 'f', 1) + " %", m_levelBOnline, now},
        {"Inlet Flow Meter", "Flow Sensor", QString::number(m_flowRate, 'f', 1) + " m³/h", m_flowOnline, now},
        {"Pressure Transmitter", "Pressure Sensor", QString::number(m_pressure, 'f', 2) + " bar", m_pressureOnline, now},
        {"Pump 1", "Pump", m_pump1Running ? "Running" : "Stopped", m_pump1Online, now},
        {"Pump 2", "Pump", m_pump2Running ? "Running" : "Stopped", m_pump2Online, now},
        {"Valve A", "Valve", m_valveAOpen ? "Open" : "Closed", m_valveAOnline, now}
    };
}

DataSimulator::HealthState DataSimulator::healthState() const
{
    const bool anyOffline = !m_levelAOnline || !m_levelBOnline || !m_flowOnline || !m_pressureOnline || !m_pump1Online || !m_pump2Online || !m_valveAOnline;
    const bool criticalProcess = m_pressure < 2.8 || m_pressure > 7.2 || m_tankA > 95.0 || m_tankA < 10.0 || m_tankB > 95.0 || m_tankB < 10.0;
    const bool warningProcess = m_temperature > 30.0 || m_flowRate < 80.0 || m_flowRate > 165.0 || m_pressure < 3.5 || m_pressure > 6.5;

    if (criticalProcess) {
        return HealthState::Critical;
    }

    if (anyOffline || warningProcess) {
        return HealthState::Warning;
    }

    return HealthState::Healthy;
}

QString DataSimulator::healthMessage() const
{
    switch (healthState()) {
    case HealthState::Healthy:
        return "Plant Stable";
    case HealthState::Warning:
        return "Process Warning";
    case HealthState::Critical:
        return "Critical Condition";
    }

    return "Unknown";
}

void DataSimulator::simulateStep()
{
    auto randRange = [](double min, double max) {
        return min + (QRandomGenerator::global()->generateDouble() * (max - min));
    };

    m_tankA = qBound(0.0, m_tankA + randRange(-2.5, 2.5), 100.0);
    m_tankB = qBound(0.0, m_tankB + randRange(-2.2, 2.2), 100.0);

    const double pumpInfluence = (m_pump1Running ? 8.0 : -5.0) + (m_valveAOpen ? -2.0 : 2.0);
    m_flowRate = qBound(60.0, m_flowRate + randRange(-6.0, 6.0) + pumpInfluence * 0.15, 180.0);
    m_temperature = qBound(10.0, m_temperature + randRange(-0.4, 0.4), 45.0);
    m_pressure = qBound(2.0, m_pressure + randRange(-0.25, 0.25) + (m_pump1Running ? 0.05 : -0.03), 8.0);

    m_pump2Running = QRandomGenerator::global()->bounded(100) > 20;

    m_levelAOnline = QRandomGenerator::global()->bounded(100) > 5;
    m_levelBOnline = QRandomGenerator::global()->bounded(100) > 5;
    m_flowOnline = QRandomGenerator::global()->bounded(100) > 6;
    m_pressureOnline = QRandomGenerator::global()->bounded(100) > 6;
    m_pump1Online = QRandomGenerator::global()->bounded(100) > 3;
    m_pump2Online = QRandomGenerator::global()->bounded(100) > 4;
    m_valveAOnline = QRandomGenerator::global()->bounded(100) > 4;

    emit dataUpdated();
}
