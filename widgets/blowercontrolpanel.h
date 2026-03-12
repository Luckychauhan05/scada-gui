#ifndef BLOWERCONTROLPANEL_H
#define BLOWERCONTROLPANEL_H

#include <QDialog>

class QPushButton;
class QLabel;
class QGridLayout;

class BlowerControlPanel : public QDialog
{
    Q_OBJECT

public:
    explicit BlowerControlPanel(const QString &blowerName, QWidget *parent = nullptr);

signals:
    void runClicked();
    void runFBFailClicked();
    void tripClicked();
    void runCmdClicked();
    void autoClicked();
    void manualClicked();
    void startClicked();
    void stopClicked();

private slots:
    void onRunClicked();
    void onRunFBFailClicked();
    void onTripClicked();
    void onRunCmdClicked();
    void onAutoClicked();
    void onManualClicked();
    void onStartClicked();
    void onStopClicked();

private:
    void setupUI();
    void createControlButton(const QString &text, const char *signalSlot, QGridLayout *layout, int row, int col);

    QString m_blowerName;
    QPushButton *m_runBtn;
    QPushButton *m_runFBFailBtn;
    QPushButton *m_tripBtn;
    QPushButton *m_runCmdBtn;
    QPushButton *m_autoBtn;
    QPushButton *m_manualBtn;
    QPushButton *m_startBtn;
    QPushButton *m_stopBtn;
    QPushButton *m_closeBtn;
    QLabel *m_statusLabel;
};

#endif // BLOWERCONTROLPANEL_H
