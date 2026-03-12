#include "widgets/pumpcontrolfaceplate.h"

#include <QFrame>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

namespace {

QString ledStyle(const QColor &inner, const QColor &outer)
{
    return QString(
               "QLabel {"
           " border: 1.4px solid #3C4651;"
           " border-radius: 11px;"
           " min-width: 22px;"
           " max-width: 22px;"
           " min-height: 22px;"
           " max-height: 22px;"
           " background-color: qradialgradient(cx:0.38, cy:0.35, radius:0.82, fx:0.32, fy:0.30, stop:0 %1, stop:0.62 %2, stop:1 #1B2229);"
               "}")
    .arg(inner.name(), outer.name());
}

} // namespace

PumpControlFaceplate::PumpControlFaceplate(const QString &pumpName, QWidget *parent)
    : QDialog(parent),
      m_pumpName(pumpName),
      m_runLed(nullptr),
      m_runFbFailLed(nullptr),
      m_tripLed(nullptr),
      m_runCmdLed(nullptr),
      m_autoButton(nullptr),
      m_manualButton(nullptr),
      m_startButton(nullptr),
      m_stopButton(nullptr),
      m_autoMode(true),
      m_run(false),
      m_runCmd(false),
      m_trip(false),
      m_runFbFail(false)
{
    setObjectName("PumpControlFaceplate");
    const QString headerText = QString("PUMP CONTROL - %1").arg(pumpName);
    setWindowTitle(headerText);
    setWindowModality(Qt::ApplicationModal);
    setModal(true);
    setMinimumSize(440, 340);
    setMaximumWidth(460);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(14, 14, 14, 14);
    mainLayout->setSpacing(12);

    auto *titleFrame = new QFrame(this);
    titleFrame->setStyleSheet("QFrame { background: #D6E2EC; border: 1px solid #8EA2B3; border-radius: 7px; }");
    auto *titleLayout = new QHBoxLayout(titleFrame);
    titleLayout->setContentsMargins(12, 7, 7, 7);

    auto *titleLabel = new QLabel(headerText, titleFrame);
    titleLabel->setStyleSheet("QLabel { color: #17384E; font-weight: 800; font-size: 13px; letter-spacing: 0.4px; }");

    auto *closeButton = new QPushButton("X", titleFrame);
    closeButton->setFixedSize(26, 24);
    closeButton->setStyleSheet(
        "QPushButton { background: #E6EDF3; border: 1px solid #7F92A3; border-radius: 4px; color: #203648; font-weight: 700; }"
        "QPushButton:hover { background: #D8E2EA; }"
        "QPushButton:pressed { background: #C5D3DE; padding-top: 1px; }");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::close);

    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch(1);
    titleLayout->addWidget(closeButton);
    mainLayout->addWidget(titleFrame);

    auto *statusFrame = new QFrame(this);
    statusFrame->setStyleSheet("QFrame { background: #EEF3F7; border: 1px solid #A2B1BE; border-radius: 7px; }");
    auto *statusLayout = new QGridLayout(statusFrame);
    statusLayout->setContentsMargins(12, 12, 12, 12);
    statusLayout->setHorizontalSpacing(28);
    statusLayout->setVerticalSpacing(12);
    statusLayout->setColumnStretch(0, 1);
    statusLayout->setColumnStretch(1, 1);

    statusLayout->addWidget(createIndicatorCell("RUN", &m_runLed), 0, 0);
    statusLayout->addWidget(createIndicatorCell("RUN FB FAIL", &m_runFbFailLed), 0, 1);
    statusLayout->addWidget(createIndicatorCell("TRIP", &m_tripLed), 1, 0);
    statusLayout->addWidget(createIndicatorCell("RUN CMD", &m_runCmdLed), 1, 1);
    mainLayout->addWidget(statusFrame);

    auto *modeFrame = new QFrame(this);
    modeFrame->setStyleSheet("QFrame { background: #EDF2F7; border: 1px solid #A2B1BE; border-radius: 7px; }");
    auto *modeLayout = new QVBoxLayout(modeFrame);
    modeLayout->setContentsMargins(12, 10, 12, 10);
    modeLayout->setSpacing(8);

    auto *modeLabel = new QLabel("MODE", modeFrame);
    modeLabel->setStyleSheet("QLabel { color: #20384B; font-weight: 800; font-size: 12px; }");

    m_autoButton = new QPushButton("AUTO", modeFrame);
    m_manualButton = new QPushButton("MANUAL", modeFrame);
    m_autoButton->setCheckable(true);
    m_manualButton->setCheckable(true);
    m_autoButton->setMinimumHeight(32);
    m_manualButton->setMinimumHeight(32);

    auto *modeButtons = new QHBoxLayout();
    modeButtons->setContentsMargins(0, 0, 0, 0);
    modeButtons->setSpacing(10);
    modeButtons->addWidget(m_autoButton);
    modeButtons->addWidget(m_manualButton);

    connect(m_autoButton, &QPushButton::clicked, this, &PumpControlFaceplate::setAutoMode);
    connect(m_manualButton, &QPushButton::clicked, this, &PumpControlFaceplate::setManualMode);

    modeLayout->addWidget(modeLabel, 0, Qt::AlignLeft);
    modeLayout->addLayout(modeButtons);
    mainLayout->addWidget(modeFrame);

    auto *commandFrame = new QFrame(this);
    commandFrame->setStyleSheet("QFrame { background: #EEF3F7; border: 1px solid #A2B1BE; border-radius: 7px; }");
    auto *commandLayout = new QHBoxLayout(commandFrame);
    commandLayout->setContentsMargins(12, 12, 12, 12);
    commandLayout->setSpacing(12);

    m_startButton = new QPushButton("START", commandFrame);
    m_stopButton = new QPushButton("STOP", commandFrame);
    m_startButton->setMinimumHeight(46);
    m_stopButton->setMinimumHeight(50);
    m_stopButton->setMinimumWidth(170);

    m_startButton->setStyleSheet(
        "QPushButton { background: #3E4953; color: #EAF0F5; border: 1px solid #27313A; border-radius: 6px; font-weight: 800; padding: 10px 16px; }"
        "QPushButton:hover { background: #4B5762; }"
        "QPushButton:pressed { background: #2F3A43; padding-top: 11px; padding-bottom: 9px; }");
    m_stopButton->setStyleSheet(
        "QPushButton { background: #B62222; color: #FDECEC; border: 1px solid #7F1717; border-radius: 6px; font-weight: 800; padding: 11px 16px; font-size: 14px; }"
        "QPushButton:hover { background: #C72B2B; }"
        "QPushButton:pressed { background: #971A1A; padding-top: 12px; padding-bottom: 10px; }");

    connect(m_startButton, &QPushButton::clicked, this, &PumpControlFaceplate::handleStart);
    connect(m_stopButton, &QPushButton::clicked, this, &PumpControlFaceplate::handleStop);

    commandLayout->addWidget(m_startButton, 1);
    commandLayout->addWidget(m_stopButton, 1);
    mainLayout->addWidget(commandFrame);

    setStyleSheet("QDialog#PumpControlFaceplate { background: #D9E4EC; }");

    updateModeVisuals();
    updateIndicators();
}

QWidget *PumpControlFaceplate::createIndicatorCell(const QString &label, QLabel **ledOut)
{
    auto *cell = new QWidget(this);
    auto *layout = new QHBoxLayout(cell);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    auto *led = new QLabel(cell);
    led->setAlignment(Qt::AlignCenter);
    led->setStyleSheet(ledStyle(QColor("#C5CBD1"), QColor("#7F8993")));

    auto *text = new QLabel(label, cell);
    text->setStyleSheet("QLabel { color: #18384D; font-weight: 700; font-size: 12px; }");

    layout->addWidget(led, 0, Qt::AlignVCenter);
    layout->addWidget(text, 1, Qt::AlignVCenter);
    cell->setMinimumWidth(180);

    if (ledOut) {
        *ledOut = led;
    }

    return cell;
}

void PumpControlFaceplate::setTrip(bool tripped)
{
    m_trip = tripped;
    if (m_trip) {
        m_run = false;
        m_runCmd = false;
    }
    updateIndicators();
}

void PumpControlFaceplate::setRunFeedbackFail(bool failed)
{
    m_runFbFail = failed;
    if (m_runFbFail) {
        m_run = false;
    }
    updateIndicators();
}

void PumpControlFaceplate::setRunState(bool running)
{
    if (m_trip || m_runFbFail) {
        m_run = false;
    } else {
        m_run = running;
    }
    updateIndicators();
}

void PumpControlFaceplate::setAutoMode()
{
    m_autoMode = true;
    updateModeVisuals();
}

void PumpControlFaceplate::setManualMode()
{
    m_autoMode = false;
    updateModeVisuals();
}

void PumpControlFaceplate::handleStart()
{
    if (m_trip) {
        return;
    }

    m_runCmd = true;
    m_run = !m_runFbFail;
    updateIndicators();
    emit started();
}

void PumpControlFaceplate::handleStop()
{
    m_runCmd = false;
    m_run = false;
    updateIndicators();
    emit stopped();
}

void PumpControlFaceplate::updateModeVisuals()
{
    m_autoButton->setChecked(m_autoMode);
    m_manualButton->setChecked(!m_autoMode);

    const QString activeStyle =
        "QPushButton { background: #39B54A; color: #F1FFF2; border: 1px solid #2E7C39; border-radius: 5px; font-weight: 800; padding: 8px 12px; }"
        "QPushButton:pressed { background: #2F9A3E; padding-top: 9px; padding-bottom: 7px; }";
    const QString inactiveStyle =
        "QPushButton { background: #A9B4BE; color: #243746; border: 1px solid #7A8896; border-radius: 5px; font-weight: 700; padding: 8px 12px; }"
        "QPushButton:pressed { background: #96A2AE; padding-top: 9px; padding-bottom: 7px; }";
    const QString manualActiveStyle =
        "QPushButton { background: #5E7183; color: #F4F8FC; border: 1px solid #3D4B57; border-radius: 5px; font-weight: 800; padding: 8px 12px; }"
        "QPushButton:pressed { background: #4B5D6D; padding-top: 9px; padding-bottom: 7px; }";

    m_autoButton->setStyleSheet(m_autoMode ? activeStyle : inactiveStyle);
    m_manualButton->setStyleSheet(m_autoMode ? inactiveStyle : manualActiveStyle);
}

void PumpControlFaceplate::updateIndicators()
{
    setLedColor(m_runLed, m_run ? LedState::Active : LedState::Inactive);
    setLedColor(m_runCmdLed, m_runCmd ? LedState::Active : LedState::Inactive);
    setLedColor(m_tripLed, m_trip ? LedState::Fault : LedState::Inactive);
    setLedColor(m_runFbFailLed, m_runFbFail ? LedState::Fault : LedState::Inactive);
}

void PumpControlFaceplate::setLedColor(QLabel *led, LedState state)
{
    if (!led) {
        return;
    }

    QColor inner("#C5CBD1");
    QColor outer("#7F8993");
    if (state == LedState::Active) {
        inner = QColor("#A5F3A4");
        outer = QColor("#2FA446");
    } else if (state == LedState::Fault) {
        inner = QColor("#FF9B9B");
        outer = QColor("#C53A3A");
    }

    led->setStyleSheet(ledStyle(inner, outer));
}
