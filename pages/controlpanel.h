#ifndef CONTROLPANEL_H
#define CONTROLPANEL_H

#include <QWidget>

class StatusIndicator;
class DataSimulator;
class QPushButton;
class QLabel;

class ControlPanelPage : public QWidget
{
    Q_OBJECT
public:
    explicit ControlPanelPage(QWidget *parent = nullptr);
    void setDataSimulator(DataSimulator *simulator);
    void setControlPermissions(bool canControl, bool canEmergency, const QString &roleName);

signals:
    void eventOccurred(const QString &source, const QString &message, const QString &severity);

private slots:
    void startPump1();
    void stopPump1();
    void openValveA();
    void closeValveA();
    void emergencyStop();
    void openAlumPump1Faceplate();
    void openAlumPump2Faceplate();
    void refreshFromSimulator();

private:
    bool confirmAction(const QString &title, const QString &message);

    QPushButton *m_startPumpBtn;
    QPushButton *m_stopPumpBtn;
    QPushButton *m_openValveBtn;
    QPushButton *m_closeValveBtn;
    QPushButton *m_estopBtn;
    QPushButton *m_alumPump1FaceplateBtn;
    QPushButton *m_alumPump2FaceplateBtn;
    QLabel *m_permissionLabel;

    StatusIndicator *m_pump1Indicator;
    StatusIndicator *m_valveAIndicator;
    DataSimulator *m_simulator;
};

#endif // CONTROLPANEL_H
