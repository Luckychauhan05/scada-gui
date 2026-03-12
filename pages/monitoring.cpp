#include "pages/monitoring.h"

#include "src/datasimulator.h"

#include <QDateTime>
#include <QHeaderView>
#include <QTableWidget>
#include <QVBoxLayout>

MonitoringPage::MonitoringPage(QWidget *parent)
    : QWidget(parent),
      m_table(new QTableWidget(this)),
    m_simulator(nullptr)
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->addWidget(m_table);

    m_table->setColumnCount(5);
    m_table->setHorizontalHeaderLabels({"Device Name", "Device Type", "Current Value", "Status", "Last Update Time"});
    m_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_table->setAlternatingRowColors(true);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);

    const QStringList devices = {
        "Tank Level Sensor A", "Tank Level Sensor B", "Inlet Flow Meter", "Pressure Transmitter", "Pump 1", "Pump 2", "Valve A"
    };
    const QStringList types = {
        "Level Sensor", "Level Sensor", "Flow Sensor", "Pressure Sensor", "Pump", "Pump", "Valve"
    };

    m_table->setRowCount(devices.size());
    for (int i = 0; i < devices.size(); ++i) {
        m_table->setItem(i, 0, new QTableWidgetItem(devices.at(i)));
        m_table->setItem(i, 1, new QTableWidgetItem(types.at(i)));
        m_table->setItem(i, 2, new QTableWidgetItem("0.0"));
        m_table->setItem(i, 3, new QTableWidgetItem("Online"));
        m_table->setItem(i, 4, new QTableWidgetItem(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")));
    }

}

void MonitoringPage::setDataSimulator(DataSimulator *simulator)
{
    if (m_simulator == simulator) {
        return;
    }

    if (m_simulator) {
        disconnect(m_simulator, &DataSimulator::dataUpdated, this, &MonitoringPage::refreshDeviceData);
    }

    m_simulator = simulator;

    if (m_simulator) {
        connect(m_simulator, &DataSimulator::dataUpdated, this, &MonitoringPage::refreshDeviceData);
        refreshDeviceData();
    }
}

void MonitoringPage::refreshDeviceData()
{
    if (!m_simulator) {
        return;
    }

    const auto snapshots = m_simulator->deviceSnapshots();

    if (m_table->rowCount() != snapshots.size()) {
        m_table->setRowCount(snapshots.size());
    }

    for (int row = 0; row < snapshots.size(); ++row) {
        const auto &snapshot = snapshots.at(row);

        if (!m_table->item(row, 0)) {
            m_table->setItem(row, 0, new QTableWidgetItem());
            m_table->setItem(row, 1, new QTableWidgetItem());
            m_table->setItem(row, 2, new QTableWidgetItem());
            m_table->setItem(row, 3, new QTableWidgetItem());
            m_table->setItem(row, 4, new QTableWidgetItem());
        }

        auto *nameItem = m_table->item(row, 0);
        auto *typeItem = m_table->item(row, 1);
        auto *valueItem = m_table->item(row, 2);
        auto *statusItem = m_table->item(row, 3);
        auto *timeItem = m_table->item(row, 4);

        nameItem->setText(snapshot.name);
        typeItem->setText(snapshot.type);
        valueItem->setText(snapshot.value);
        statusItem->setText(snapshot.online ? "Online" : "Offline");
        statusItem->setForeground(snapshot.online ? QColor("#81C784") : QColor("#EF5350"));
        timeItem->setText(snapshot.lastUpdate.toString("yyyy-MM-dd HH:mm:ss"));
    }
}
