#include "pages/controlpanel.h"

#include "src/datasimulator.h"
#include "widgets/pumpcontrolfaceplate.h"
#include "widgets/statusindicator.h"

#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

ControlPanelPage::ControlPanelPage(QWidget *parent)
    : QWidget(parent),
            m_startPumpBtn(nullptr),
            m_stopPumpBtn(nullptr),
            m_openValveBtn(nullptr),
            m_closeValveBtn(nullptr),
            m_estopBtn(nullptr),
            m_alumPump1FaceplateBtn(nullptr),
            m_alumPump2FaceplateBtn(nullptr),
            m_permissionLabel(new QLabel(this)),
      m_pump1Indicator(new StatusIndicator("Pump 1 - Stopped", this)),
            m_valveAIndicator(new StatusIndicator("Valve A - Closed", this)),
            m_simulator(nullptr)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(14);

    auto *controlsBox = new QGroupBox("Equipment Controls", this);
    controlsBox->setObjectName("card");
    auto *controlsLayout = new QGridLayout(controlsBox);
    controlsLayout->setHorizontalSpacing(12);
    controlsLayout->setVerticalSpacing(12);

    m_startPumpBtn = new QPushButton("START Pump 1", controlsBox);
    m_stopPumpBtn = new QPushButton("STOP Pump 1", controlsBox);
    m_openValveBtn = new QPushButton("OPEN Valve A", controlsBox);
    m_closeValveBtn = new QPushButton("CLOSE Valve A", controlsBox);
    m_estopBtn = new QPushButton("EMERGENCY STOP", controlsBox);

    m_startPumpBtn->setObjectName("primaryButton");
    m_openValveBtn->setObjectName("primaryButton");
    m_estopBtn->setObjectName("dangerButton");

    controlsLayout->addWidget(m_startPumpBtn, 0, 0);
    controlsLayout->addWidget(m_stopPumpBtn, 0, 1);
    controlsLayout->addWidget(m_openValveBtn, 1, 0);
    controlsLayout->addWidget(m_closeValveBtn, 1, 1);
    controlsLayout->addWidget(m_estopBtn, 2, 0, 1, 2);

    m_permissionLabel->setStyleSheet("color: #B0BEC5; font-size: 12px;");
    controlsLayout->addWidget(m_permissionLabel, 3, 0, 1, 2);

    m_alumPump1FaceplateBtn = new QPushButton("ALUM PUMP-1 FACEPLATE", controlsBox);
    m_alumPump2FaceplateBtn = new QPushButton("ALUM PUMP-2 FACEPLATE", controlsBox);
    m_alumPump1FaceplateBtn->setObjectName("primaryButton");
    m_alumPump2FaceplateBtn->setObjectName("primaryButton");
    controlsLayout->addWidget(m_alumPump1FaceplateBtn, 4, 0);
    controlsLayout->addWidget(m_alumPump2FaceplateBtn, 4, 1);

    auto *statusBox = new QGroupBox("Equipment Status", this);
    statusBox->setObjectName("card");
    auto *statusLayout = new QVBoxLayout(statusBox);
    statusLayout->setSpacing(10);
    statusLayout->addWidget(m_pump1Indicator);
    statusLayout->addWidget(m_valveAIndicator);
    statusLayout->addStretch();

    m_pump1Indicator->setOnline(false);
    m_valveAIndicator->setOnline(false);

    mainLayout->addWidget(controlsBox);
    mainLayout->addWidget(statusBox);
    mainLayout->addStretch();

    connect(m_startPumpBtn, &QPushButton::clicked, this, &ControlPanelPage::startPump1);
    connect(m_stopPumpBtn, &QPushButton::clicked, this, &ControlPanelPage::stopPump1);
    connect(m_openValveBtn, &QPushButton::clicked, this, &ControlPanelPage::openValveA);
    connect(m_closeValveBtn, &QPushButton::clicked, this, &ControlPanelPage::closeValveA);
    connect(m_estopBtn, &QPushButton::clicked, this, &ControlPanelPage::emergencyStop);
    connect(m_alumPump1FaceplateBtn, &QPushButton::clicked, this, &ControlPanelPage::openAlumPump1Faceplate);
    connect(m_alumPump2FaceplateBtn, &QPushButton::clicked, this, &ControlPanelPage::openAlumPump2Faceplate);

    setControlPermissions(false, false, "Guest");
}

void ControlPanelPage::setControlPermissions(bool canControl, bool canEmergency, const QString &roleName)
{
    m_startPumpBtn->setEnabled(canControl);
    m_stopPumpBtn->setEnabled(canControl);
    m_openValveBtn->setEnabled(canControl);
    m_closeValveBtn->setEnabled(canControl);
    m_estopBtn->setEnabled(canEmergency);

    m_permissionLabel->setText(
        QString("Role: %1 | Commands: %2 | Emergency Stop: %3")
            .arg(roleName)
            .arg(canControl ? "Enabled" : "Locked")
            .arg(canEmergency ? "Enabled" : "Locked"));
}

void ControlPanelPage::setDataSimulator(DataSimulator *simulator)
{
    if (m_simulator == simulator) {
        return;
    }

    if (m_simulator) {
        disconnect(m_simulator, &DataSimulator::dataUpdated, this, &ControlPanelPage::refreshFromSimulator);
    }

    m_simulator = simulator;

    if (m_simulator) {
        connect(m_simulator, &DataSimulator::dataUpdated, this, &ControlPanelPage::refreshFromSimulator);
        refreshFromSimulator();
    }
}

void ControlPanelPage::startPump1()
{
    if (!confirmAction("Start Pump", "Do you want to START Pump 1?")) {
        return;
    }

    if (m_simulator) {
        m_simulator->setPump1Running(true);
    }

    emit eventOccurred("Control Panel", "Pump 1 START command issued", "Command");
}

void ControlPanelPage::stopPump1()
{
    if (!confirmAction("Stop Pump", "Do you want to STOP Pump 1?")) {
        return;
    }

    if (m_simulator) {
        m_simulator->setPump1Running(false);
    }

    emit eventOccurred("Control Panel", "Pump 1 STOP command issued", "Command");
}

void ControlPanelPage::openValveA()
{
    if (!confirmAction("Open Valve", "Do you want to OPEN Valve A?")) {
        return;
    }

    if (m_simulator) {
        m_simulator->setValveAOpen(true);
    }

    emit eventOccurred("Control Panel", "Valve A OPEN command issued", "Command");
}

void ControlPanelPage::closeValveA()
{
    if (!confirmAction("Close Valve", "Do you want to CLOSE Valve A?")) {
        return;
    }

    if (m_simulator) {
        m_simulator->setValveAOpen(false);
    }

    emit eventOccurred("Control Panel", "Valve A CLOSE command issued", "Command");
}

void ControlPanelPage::emergencyStop()
{
    if (!confirmAction("Emergency Stop", "Execute EMERGENCY STOP for all equipment?")) {
        return;
    }

    if (m_simulator) {
        m_simulator->emergencyStop();
    }

    emit eventOccurred("Control Panel", "Emergency stop executed", "Critical");

    QMessageBox::critical(this, "Emergency Stop", "All controllable equipment has been halted.");
}

void ControlPanelPage::openAlumPump1Faceplate()
{
    PumpControlFaceplate dlg("ALUM PUMP-1", this);
    dlg.exec();
}

void ControlPanelPage::openAlumPump2Faceplate()
{
    PumpControlFaceplate dlg("ALUM PUMP-2", this);
    dlg.exec();
}

void ControlPanelPage::refreshFromSimulator()
{
    if (!m_simulator) {
        return;
    }

    const bool pumpRunning = m_simulator->pump1Running();
    const bool valveOpen = m_simulator->valveAOpen();

    m_pump1Indicator->setOnline(pumpRunning);
    m_pump1Indicator->setText(pumpRunning ? "Pump 1 - Running" : "Pump 1 - Stopped");

    m_valveAIndicator->setOnline(valveOpen);
    m_valveAIndicator->setText(valveOpen ? "Valve A - Open" : "Valve A - Closed");
}

bool ControlPanelPage::confirmAction(const QString &title, const QString &message)
{
    return QMessageBox::question(this, title, message, QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes;
}
