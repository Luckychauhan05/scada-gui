#ifndef FILTRATIONDASHBOARD_H
#define FILTRATIONDASHBOARD_H

#include <QWidget>

class QLabel;
class QTimer;
class ProcessFlowView;

class FiltrationDashboardPage : public QWidget
{
    Q_OBJECT
public:
    explicit FiltrationDashboardPage(QWidget *parent = nullptr);

private slots:
    void updateDateTime();

private:
    QLabel *m_dateTimeLabel;
    ProcessFlowView *m_flowView;
    QTimer *m_clock;
};

#endif // FILTRATIONDASHBOARD_H
