#include "pages/mimic.h"

#include "src/datasimulator.h"
#include "widgets/processmimicwidget.h"

#include <QGroupBox>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

MimicPage::MimicPage(QWidget *parent)
    : QWidget(parent),
      m_mimicWidget(new ProcessMimicWidget(this)),
      m_summaryLabel(new QLabel("Live process overview unavailable", this)),
      m_simulator(nullptr)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(14);

    auto *headerBox = new QGroupBox("Process Mimic Overview", this);
    headerBox->setObjectName("card");
    auto *headerLayout = new QHBoxLayout(headerBox);
    m_summaryLabel->setStyleSheet("font-size: 14px; color: #C5D4E2; font-weight: 600;");
    headerLayout->addWidget(m_summaryLabel);
    headerLayout->addStretch();

    auto *mimicBox = new QGroupBox("Water Treatment Flow Path", this);
    mimicBox->setObjectName("card");
    auto *mimicLayout = new QVBoxLayout(mimicBox);
    mimicLayout->addWidget(m_mimicWidget);

    mainLayout->addWidget(headerBox);
    mainLayout->addWidget(mimicBox, 1);
}

void MimicPage::setDataSimulator(DataSimulator *simulator)
{
    if (m_simulator == simulator) {
        return;
    }

    if (m_simulator) {
        disconnect(m_simulator, &DataSimulator::dataUpdated, this, &MimicPage::refreshSummary);
    }

    m_simulator = simulator;
    m_mimicWidget->setDataSimulator(simulator);

    if (m_simulator) {
        connect(m_simulator, &DataSimulator::dataUpdated, this, &MimicPage::refreshSummary);
        refreshSummary();
    }
}

void MimicPage::refreshSummary()
{
    if (!m_simulator) {
        return;
    }

    const QString pumpText = m_simulator->pump1Running() ? "Pump 1 running" : "Pump 1 stopped";
    const QString valveText = m_simulator->valveAOpen() ? "Valve A open" : "Valve A closed";
    m_summaryLabel->setText(QString("%1 | %2 | Flow %3 m³/h | Pressure %4 bar | Temp %5 °C | %6")
                                .arg(pumpText)
                                .arg(valveText)
                                .arg(QString::number(m_simulator->flowRate(), 'f', 1))
                                .arg(QString::number(m_simulator->pressure(), 'f', 2))
                                .arg(QString::number(m_simulator->temperature(), 'f', 1))
                                .arg(m_simulator->healthMessage()));
}
