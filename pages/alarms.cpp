#include "pages/alarms.h"

#include <QDateTime>
#include <QHeaderView>
#include <QPushButton>
#include <QRandomGenerator>
#include <QTableWidget>
#include <QTimer>
#include <QVBoxLayout>

AlarmManagerPage::AlarmManagerPage(QWidget *parent)
    : QWidget(parent),
      m_alarmTable(new QTableWidget(this)),
      m_ackButton(new QPushButton("Acknowledge Selected Alarm", this)),
      m_alarmTimer(new QTimer(this))
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    m_alarmTable->setColumnCount(4);
    m_alarmTable->setHorizontalHeaderLabels({"Timestamp", "Alarm Message", "Severity", "Status"});
    m_alarmTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_alarmTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_alarmTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    layout->addWidget(m_alarmTable);
    layout->addWidget(m_ackButton, 0, Qt::AlignRight);

    connect(m_ackButton, &QPushButton::clicked, this, &AlarmManagerPage::acknowledgeSelectedAlarm);
    connect(m_alarmTimer, &QTimer::timeout, this, &AlarmManagerPage::generateAlarm);

    m_alarmTimer->start(4500);
}

void AlarmManagerPage::generateAlarm()
{
    const QStringList messages = {
        "Tank A level high-high limit exceeded",
        "Inlet pressure low warning",
        "Pump 2 overload tripped",
        "Flow sensor communication timeout",
        "Chlorination dosing deviation detected"
    };

    const int messageIndex = QRandomGenerator::global()->bounded(messages.size());
    const int severityRoll = QRandomGenerator::global()->bounded(100);
    const QString severity = severityRoll > 78 ? "Critical" : (severityRoll > 45 ? "Medium" : "Low");
    const QString journalSeverity = severity == "Critical" ? "Critical" : (severity == "Medium" ? "Warning" : "Info");

    const int row = m_alarmTable->rowCount();
    m_alarmTable->insertRow(row);

    m_alarmTable->setItem(row, 0, new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")));
    m_alarmTable->setItem(row, 1, new QTableWidgetItem(messages.at(messageIndex)));
    m_alarmTable->setItem(row, 2, new QTableWidgetItem(severity));
    m_alarmTable->setItem(row, 3, new QTableWidgetItem("Active"));

    if (severity == "Critical") {
        for (int col = 0; col < 4; ++col) {
            if (auto *item = m_alarmTable->item(row, col)) {
                item->setBackground(QColor("#5A1E1E"));
                item->setForeground(QColor("#FFCDD2"));
            }
        }
        emit criticalAlarmRaised(messages.at(messageIndex));
    } else if (severity == "Medium") {
        for (int col = 0; col < 4; ++col) {
            if (auto *item = m_alarmTable->item(row, col)) {
                item->setBackground(QColor("#4A3A1B"));
                item->setForeground(QColor("#FFE0B2"));
            }
        }
    }

    emit eventOccurred("Alarm Manager", messages.at(messageIndex), journalSeverity);

    refreshStats();
}

void AlarmManagerPage::acknowledgeSelectedAlarm()
{
    const auto rows = m_alarmTable->selectionModel()->selectedRows();
    for (const QModelIndex &index : rows) {
        auto *messageItem = m_alarmTable->item(index.row(), 1);
        auto *statusItem = m_alarmTable->item(index.row(), 3);
        if (statusItem && statusItem->text() == "Active") {
            statusItem->setText("Acknowledged");
            statusItem->setForeground(QColor("#B0BEC5"));
            emit eventOccurred("Alarm Manager",
                               QString("Alarm acknowledged: %1").arg(messageItem ? messageItem->text() : "Unknown"),
                               "Info");
        }
    }

    refreshStats();
}

void AlarmManagerPage::refreshStats()
{
    int active = 0;
    int critical = 0;

    for (int row = 0; row < m_alarmTable->rowCount(); ++row) {
        const auto *statusItem = m_alarmTable->item(row, 3);
        const auto *severityItem = m_alarmTable->item(row, 2);
        if (!statusItem || !severityItem) {
            continue;
        }

        if (statusItem->text() == "Active") {
            ++active;
            if (severityItem->text() == "Critical") {
                ++critical;
            }
        }
    }

    emit alarmStatsChanged(active, critical);
}
