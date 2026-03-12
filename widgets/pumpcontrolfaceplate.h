#ifndef PUMPCONTROLFACEPLATE_H
#define PUMPCONTROLFACEPLATE_H

#include <QDialog>

class QLabel;
class QPushButton;

class PumpControlFaceplate : public QDialog
{
    Q_OBJECT
public:
    explicit PumpControlFaceplate(const QString &pumpName, QWidget *parent = nullptr);

    void setTrip(bool tripped);
    void setRunFeedbackFail(bool failed);
    void setRunState(bool running);


signals:
    void started();
    void stopped();

private slots:
    void setAutoMode();
    void setManualMode();
    void handleStart();
    void handleStop();

private:
    enum class LedState {
        Inactive,
        Active,
        Fault
    };

    QWidget *createIndicatorCell(const QString &label, QLabel **ledOut);
    void updateModeVisuals();
    void updateIndicators();
    void setLedColor(QLabel *led, LedState state);

    QString m_pumpName;

    QLabel *m_runLed;
    QLabel *m_runFbFailLed;
    QLabel *m_tripLed;
    QLabel *m_runCmdLed;

    QPushButton *m_autoButton;
    QPushButton *m_manualButton;
    QPushButton *m_startButton;
    QPushButton *m_stopButton;

    bool m_autoMode;
    bool m_run;
    bool m_runCmd;
    bool m_trip;
    bool m_runFbFail;
};

#endif // PUMPCONTROLFACEPLATE_H
