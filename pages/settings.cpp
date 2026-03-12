#include "pages/settings.h"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

SettingsPage::SettingsPage(QWidget *parent)
    : QWidget(parent)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(14);

    auto *commBox = new QGroupBox("Communication Settings", this);
    commBox->setObjectName("card");
    auto *commForm = new QFormLayout(commBox);
    auto *protocolCombo = new QComboBox(commBox);
    protocolCombo->addItems({"Modbus TCP", "OPC UA", "MQTT"});
    commForm->addRow("Protocol:", protocolCombo);
    commForm->addRow("Server IP:", new QLineEdit("192.168.1.100", commBox));
    auto *portSpin = new QSpinBox(commBox);
    portSpin->setRange(1, 65535);
    portSpin->setValue(502);
    commForm->addRow("Port:", portSpin);

    auto *alarmBox = new QGroupBox("Alarm Threshold Configuration", this);
    alarmBox->setObjectName("card");
    auto *alarmForm = new QFormLayout(alarmBox);
    auto *levelHigh = new QSpinBox(alarmBox);
    levelHigh->setRange(0, 100);
    levelHigh->setValue(90);
    auto *levelLow = new QSpinBox(alarmBox);
    levelLow->setRange(0, 100);
    levelLow->setValue(20);
    alarmForm->addRow("Tank Level High (%):", levelHigh);
    alarmForm->addRow("Tank Level Low (%):", levelLow);
    alarmForm->addRow("Enable SMS Alerts:", new QCheckBox(alarmBox));

    auto *userBox = new QGroupBox("User Role Management", this);
    userBox->setObjectName("card");
    auto *userForm = new QFormLayout(userBox);
    auto *role = new QComboBox(userBox);
    role->addItems({"Operator", "Supervisor", "Engineer", "Administrator"});
    userForm->addRow("Default Role:", role);
    userForm->addRow("Require 2FA:", new QCheckBox(userBox));

    auto *dbBox = new QGroupBox("Database Configuration", this);
    dbBox->setObjectName("card");
    auto *dbForm = new QFormLayout(dbBox);
    dbForm->addRow("Database Host:", new QLineEdit("localhost", dbBox));
    dbForm->addRow("Database Name:", new QLineEdit("scada_historian", dbBox));
    auto *retentionSpin = new QSpinBox(dbBox);
    retentionSpin->setRange(7, 3650);
    retentionSpin->setValue(365);
    dbForm->addRow("Retention Days:", retentionSpin);

    auto *buttonsRow = new QHBoxLayout();
    buttonsRow->addStretch();
    auto *saveBtn = new QPushButton("Save Configuration", this);
    saveBtn->setObjectName("primaryButton");
    buttonsRow->addWidget(saveBtn);

    mainLayout->addWidget(commBox);
    mainLayout->addWidget(alarmBox);
    mainLayout->addWidget(userBox);
    mainLayout->addWidget(dbBox);
    mainLayout->addLayout(buttonsRow);
    mainLayout->addStretch();
}
