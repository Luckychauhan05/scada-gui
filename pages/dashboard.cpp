#include "pages/dashboard.h"

#include "src/datasimulator.h"

#include "widgets/sensorcard.h"
#include "widgets/statusindicator.h"
#include "widgets/tankwidget.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

DashboardPage::DashboardPage(QWidget *parent)
    : QWidget(parent),
      m_tankA(new TankWidget("Tank A", this)),
      m_tankB(new TankWidget("Tank B", this)),
      m_pump1Status(new StatusIndicator("Pump 1 - Running", this)),
      m_pump2Status(new StatusIndicator("Pump 2 - Stopped", this)),
      m_flowCard(new SensorCard("Flow Rate", "m³/h", this)),
      m_tempCard(new SensorCard("Temperature", "°C", this)),
      m_pressureCard(new SensorCard("Pressure", "bar", this)),
      m_systemHealthLabel(new QLabel("System Health: Good", this)),
    m_alarmSummaryLabel(new QLabel("Active Alarms: 0 | Critical: 0", this)),
    m_simulator(nullptr)
{
    auto *mainLayout = new QGridLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setHorizontalSpacing(14);
    mainLayout->setVerticalSpacing(14);

    auto *tankContainer = new QWidget(this);
    auto *tankLayout = new QHBoxLayout(tankContainer);
    tankLayout->setContentsMargins(0, 0, 0, 0);
    tankLayout->setSpacing(14);
    tankLayout->addWidget(m_tankA);
    tankLayout->addWidget(m_tankB);

    auto *pumpContainer = new QWidget(this);
    auto *pumpLayout = new QVBoxLayout(pumpContainer);
    pumpLayout->setContentsMargins(0, 0, 0, 0);
    pumpLayout->setSpacing(10);
    pumpLayout->addWidget(m_pump1Status);
    pumpLayout->addWidget(m_pump2Status);
    pumpLayout->addStretch();

    m_systemHealthLabel->setStyleSheet("font-size: 18px; font-weight: 600; color: #66BB6A;");
    m_alarmSummaryLabel->setStyleSheet("font-size: 15px; color: #FFB74D;");

    auto *healthContainer = new QWidget(this);
    auto *healthLayout = new QVBoxLayout(healthContainer);
    healthLayout->setContentsMargins(0, 0, 0, 0);
    healthLayout->setSpacing(8);
    healthLayout->addWidget(m_systemHealthLabel);
    healthLayout->addWidget(m_alarmSummaryLabel);
    healthLayout->addStretch();

    mainLayout->addWidget(createCard("Tank Levels", tankContainer), 0, 0, 2, 2);
    mainLayout->addWidget(createCard("Pump Status", pumpContainer), 0, 2, 1, 1);
    mainLayout->addWidget(createCard("Flow Monitoring", m_flowCard), 1, 2, 1, 1);
    mainLayout->addWidget(createCard("Temperature", m_tempCard), 2, 0, 1, 1);
    mainLayout->addWidget(createCard("Pressure", m_pressureCard), 2, 1, 1, 1);
    mainLayout->addWidget(createCard("System Health", healthContainer), 2, 2, 1, 1);

    mainLayout->setRowStretch(3, 1);

    m_tankA->setLevel(58.0);
    m_tankB->setLevel(42.0);
    m_pump1Status->setOnline(true);
    m_pump2Status->setOnline(false);
}

void DashboardPage::setDataSimulator(DataSimulator *simulator)
{
    if (m_simulator == simulator) {
        return;
    }

    if (m_simulator) {
        disconnect(m_simulator, &DataSimulator::dataUpdated, this, &DashboardPage::refreshFromSimulator);
    }

    m_simulator = simulator;

    if (m_simulator) {
        connect(m_simulator, &DataSimulator::dataUpdated, this, &DashboardPage::refreshFromSimulator);
        refreshFromSimulator();
    }
}

void DashboardPage::updateAlarmSummary(int activeAlarms, int criticalAlarms)
{
    m_alarmSummaryLabel->setText(QString("Active Alarms: %1 | Critical: %2")
                                     .arg(activeAlarms)
                                     .arg(criticalAlarms));

    const bool critical = criticalAlarms > 0;
    m_alarmSummaryLabel->setStyleSheet(critical
                                           ? "font-size: 15px; color: #EF5350; font-weight: 600;"
                                           : "font-size: 15px; color: #FFB74D;");
}

void DashboardPage::refreshFromSimulator()
{
    if (!m_simulator) {
        return;
    }

    const double tank1 = m_simulator->tankALevel();
    const double tank2 = m_simulator->tankBLevel();

    m_tankA->animateToLevel(tank1);
    m_tankB->animateToLevel(tank2);

    const double flow = m_simulator->flowRate();
    const double temp = m_simulator->temperature();
    const double pressure = m_simulator->pressure();

    m_flowCard->setValue(flow);
    m_tempCard->setValue(temp);
    m_pressureCard->setValue(pressure);

    const bool healthy = (pressure > 3.8 && pressure < 5.8) && (temp < 30.0);
    m_flowCard->setHealthy(healthy);
    m_tempCard->setHealthy(temp < 29.5);
    m_pressureCard->setHealthy(pressure > 4.0);

    m_systemHealthLabel->setText(healthy ? "System Health: Good" : "System Health: Attention Required");
    m_systemHealthLabel->setStyleSheet(healthy
                                           ? "font-size: 18px; font-weight: 600; color: #66BB6A;"
                                           : "font-size: 18px; font-weight: 600; color: #FFA726;");

    const bool pump1Online = m_simulator->pump1Running();
    m_pump1Status->setOnline(pump1Online);
    m_pump1Status->setText(pump1Online ? "Pump 1 - Running" : "Pump 1 - Stopped");

    const bool pump2Online = m_simulator->pump2Running();
    m_pump2Status->setOnline(pump2Online);
    m_pump2Status->setText(pump2Online ? "Pump 2 - Running" : "Pump 2 - Stopped");
}

QWidget *DashboardPage::createCard(const QString &title, QWidget *content)
{
    auto *box = new QGroupBox(title, this);
    box->setObjectName("card");
    auto *layout = new QVBoxLayout(box);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->addWidget(content);
    return box;
}
