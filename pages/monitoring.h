#ifndef MONITORING_H
#define MONITORING_H

#include <QWidget>

class QTableWidget;
class DataSimulator;

class MonitoringPage : public QWidget
{
    Q_OBJECT
public:
    explicit MonitoringPage(QWidget *parent = nullptr);
    void setDataSimulator(DataSimulator *simulator);

private slots:
    void refreshDeviceData();

private:
    QTableWidget *m_table;
    DataSimulator *m_simulator;
};

#endif // MONITORING_H
