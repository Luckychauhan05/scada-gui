#include "widgets/eventlogwidget.h"

#include <QDateTime>
#include <QHeaderView>
#include <QTableWidget>
#include <QVBoxLayout>

EventLogWidget::EventLogWidget(QWidget *parent)
    : QWidget(parent),
      m_table(new QTableWidget(this))
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);
    layout->addWidget(m_table);

    m_table->setColumnCount(4);
    m_table->setHorizontalHeaderLabels({"Timestamp", "Source", "Event", "Severity"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setAlternatingRowColors(true);
}

void EventLogWidget::addEvent(const QString &source, const QString &message, const QString &severity)
{
    const int row = 0;
    m_table->insertRow(row);

    auto *timeItem = new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
    auto *sourceItem = new QTableWidgetItem(source);
    auto *messageItem = new QTableWidgetItem(message);
    auto *severityItem = new QTableWidgetItem(severity);

    QColor foreground("#DDE3EA");
    QColor background("#1E2A35");

    if (severity == "Critical") {
        foreground = QColor("#FFCDD2");
        background = QColor("#5A1E1E");
    } else if (severity == "Warning") {
        foreground = QColor("#FFE0B2");
        background = QColor("#4A3A1B");
    } else if (severity == "Command") {
        foreground = QColor("#BBDEFB");
        background = QColor("#16324A");
    }

    const QList<QTableWidgetItem *> items = {timeItem, sourceItem, messageItem, severityItem};
    for (auto *item : items) {
        item->setForeground(foreground);
        item->setBackground(background);
    }

    m_table->setItem(row, 0, timeItem);
    m_table->setItem(row, 1, sourceItem);
    m_table->setItem(row, 2, messageItem);
    m_table->setItem(row, 3, severityItem);

    while (m_table->rowCount() > 200) {
        m_table->removeRow(m_table->rowCount() - 1);
    }
}
