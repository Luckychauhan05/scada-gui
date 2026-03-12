#ifndef MIMIC_H
#define MIMIC_H

#include <QWidget>

class QLabel;
class ProcessMimicWidget;
class DataSimulator;

class MimicPage : public QWidget
{
    Q_OBJECT
public:
    explicit MimicPage(QWidget *parent = nullptr);

    void setDataSimulator(DataSimulator *simulator);

private slots:
    void refreshSummary();

private:
    ProcessMimicWidget *m_mimicWidget;
    QLabel *m_summaryLabel;
    DataSimulator *m_simulator;
};

#endif // MIMIC_H
