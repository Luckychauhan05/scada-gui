#include "widgets/blowercontrolpanel.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QLabel>
#include <QFont>
#include <QDateTime>

BlowerControlPanel::BlowerControlPanel(const QString &blowerName, QWidget *parent)
    : QDialog(parent),
      m_blowerName(blowerName)
{
    setWindowTitle(blowerName + " - Control Panel");
    setWindowIcon(QIcon());
    setStyleSheet(R"(
        QDialog {
            background-color: #E8F1F5;
        }
        QPushButton {
            background-color: #4A90E2;
            color: white;
            border: none;
            border-radius: 5px;
            padding: 8px;
            font-weight: bold;
            font-size: 11px;
        }
        QPushButton:hover {
            background-color: #357ABD;
        }
        QPushButton:pressed {
            background-color: #1F4D7D;
        }
        QPushButton#stopBtn {
            background-color: #E74C3C;
        }
        QPushButton#stopBtn:hover {
            background-color: #C0392B;
        }
        QLabel {
            color: #0D2A38;
            font-weight: bold;
        }
        QLabel#titleLabel {
            font-size: 14px;
            font-weight: bold;
            color: #1A3A4A;
        }
        QLabel#statusLabel {
            font-size: 11px;
            color: #27AE60;
            border: 1px solid #27AE60;
            border-radius: 3px;
            padding: 4px;
            background-color: #E8F8F0;
        }
    )");

    setupUI();
    setModal(true);
    resize(550, 350);
}

void BlowerControlPanel::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(15);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // Title
    QLabel *titleLabel = new QLabel(m_blowerName + " Control Panel");
    titleLabel->setObjectName("titleLabel");
    mainLayout->addWidget(titleLabel);

    // Status Label
    m_statusLabel = new QLabel("Status: IDLE");
    m_statusLabel->setObjectName("statusLabel");
    mainLayout->addWidget(m_statusLabel);

    // Control Buttons Grid
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(10);

    // Row 1: Run buttons
    m_runBtn = new QPushButton("RUN");
    m_runFBFailBtn = new QPushButton("RUN FB FAIL");
    gridLayout->addWidget(m_runBtn, 0, 0);
    gridLayout->addWidget(m_runFBFailBtn, 0, 1);

    // Row 2: Trip and Cmd
    m_tripBtn = new QPushButton("TRIP");
    m_runCmdBtn = new QPushButton("RUN CMD");
    gridLayout->addWidget(m_tripBtn, 1, 0);
    gridLayout->addWidget(m_runCmdBtn, 1, 1);

    // Row 3: Auto and Manual
    m_autoBtn = new QPushButton("AUTO");
    m_manualBtn = new QPushButton("MANUAL");
    gridLayout->addWidget(m_autoBtn, 2, 0);
    gridLayout->addWidget(m_manualBtn, 2, 1);

    // Row 4: Start and Stop
    m_startBtn = new QPushButton("START");
    m_stopBtn = new QPushButton("STOP");
    m_stopBtn->setObjectName("stopBtn");
    gridLayout->addWidget(m_startBtn, 3, 0);
    gridLayout->addWidget(m_stopBtn, 3, 1);

    mainLayout->addLayout(gridLayout);

    // Spacer
    mainLayout->addStretch();

    // Close Button
    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    m_closeBtn = new QPushButton("CLOSE");
    m_closeBtn->setFixedWidth(100);
    bottomLayout->addWidget(m_closeBtn);
    mainLayout->addLayout(bottomLayout);

    // Connect signals
    connect(m_runBtn, &QPushButton::clicked, this, &BlowerControlPanel::onRunClicked);
    connect(m_runFBFailBtn, &QPushButton::clicked, this, &BlowerControlPanel::onRunFBFailClicked);
    connect(m_tripBtn, &QPushButton::clicked, this, &BlowerControlPanel::onTripClicked);
    connect(m_runCmdBtn, &QPushButton::clicked, this, &BlowerControlPanel::onRunCmdClicked);
    connect(m_autoBtn, &QPushButton::clicked, this, &BlowerControlPanel::onAutoClicked);
    connect(m_manualBtn, &QPushButton::clicked, this, &BlowerControlPanel::onManualClicked);
    connect(m_startBtn, &QPushButton::clicked, this, &BlowerControlPanel::onStartClicked);
    connect(m_stopBtn, &QPushButton::clicked, this, &BlowerControlPanel::onStopClicked);
    connect(m_closeBtn, &QPushButton::clicked, this, &QDialog::accept);
}

void BlowerControlPanel::onRunClicked()
{
    m_statusLabel->setText("Status: RUN - " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    emit runClicked();
}

void BlowerControlPanel::onRunFBFailClicked()
{
    m_statusLabel->setText("Status: RUN FB FAIL - " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    m_statusLabel->setStyleSheet("QLabel#statusLabel { color: #E67E22; border: 1px solid #E67E22; background-color: #FEF5E7; }");
    emit runFBFailClicked();
}

void BlowerControlPanel::onTripClicked()
{
    m_statusLabel->setText("Status: TRIPPED - " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    m_statusLabel->setStyleSheet("QLabel#statusLabel { color: #E74C3C; border: 1px solid #E74C3C; background-color: #FADBD8; }");
    emit tripClicked();
}

void BlowerControlPanel::onRunCmdClicked()
{
    m_statusLabel->setText("Status: RUN CMD - " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    emit runCmdClicked();
}

void BlowerControlPanel::onAutoClicked()
{
    m_statusLabel->setText("Status: AUTO MODE - " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    m_statusLabel->setStyleSheet("QLabel#statusLabel { color: #3498DB; border: 1px solid #3498DB; background-color: #EBF5FB; }");
    emit autoClicked();
}

void BlowerControlPanel::onManualClicked()
{
    m_statusLabel->setText("Status: MANUAL MODE - " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    m_statusLabel->setStyleSheet("QLabel#statusLabel { color: #F39C12; border: 1px solid #F39C12; background-color: #FEF9E7; }");
    emit manualClicked();
}

void BlowerControlPanel::onStartClicked()
{
    m_statusLabel->setText("Status: STARTING... - " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    m_statusLabel->setStyleSheet("QLabel#statusLabel { color: #27AE60; border: 1px solid #27AE60; background-color: #E8F8F0; }");
    emit startClicked();
}

void BlowerControlPanel::onStopClicked()
{
    m_statusLabel->setText("Status: STOPPED - " + QDateTime::currentDateTime().toString("hh:mm:ss"));
    m_statusLabel->setStyleSheet("QLabel#statusLabel { color: #E74C3C; border: 1px solid #E74C3C; background-color: #FADBD8; }");
    emit stopClicked();
}
