#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QStackedWidget;
class QTimer;
class QPushButton;

class DashboardPage;
class AlarmManagerPage;
class AlumClariflocculatorView;
class ProcessFlowView;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

private slots:
    void switchToWelcome();
    void switchToAlum();
    void switchToFiltration();
    void switchToAlarm();
    void resetCurrentPage();
    void updateDateTime();

private:
    enum PageIndex {
        WelcomePage = 0,
        AlumPage,
        FiltrationPage,
        AlarmPage
    };

    QPushButton *createNavButton(const QString &text);
    void setCurrentPage(PageIndex page);
    void setNavActive(PageIndex page);
    QString pageTitle(PageIndex page) const;

    QLabel *m_titleLabel;
    QLabel *m_dateTimeLabel;
    QStackedWidget *m_stack;
    QTimer *m_clock;

    DashboardPage *m_welcomePage;
    AlumClariflocculatorView *m_alumView;
    ProcessFlowView *m_filtrationView;
    AlarmManagerPage *m_alarmPage;

    QPushButton *m_navAlum;
    QPushButton *m_navFiltration;
    QPushButton *m_navWelcome;
    QPushButton *m_navAlarm;
    QPushButton *m_navReset;
    QPushButton *m_navExit;
};

#endif // MAINWINDOW_H
