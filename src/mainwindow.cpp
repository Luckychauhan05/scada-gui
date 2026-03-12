#include "src/mainwindow.h"

#include "pages/alarms.h"
#include "pages/dashboard.h"
#include "widgets/alumclariflocculatorview.h"
#include "widgets/processflowview.h"

#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QTimer>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_titleLabel(nullptr),
      m_dateTimeLabel(nullptr),
      m_stack(new QStackedWidget(this)),
      m_clock(new QTimer(this)),
      m_welcomePage(new DashboardPage(this)),
      m_alumView(new AlumClariflocculatorView(this)),
      m_filtrationView(new ProcessFlowView(this)),
      m_alarmPage(new AlarmManagerPage(this)),
      m_navAlum(nullptr),
      m_navFiltration(nullptr),
      m_navWelcome(nullptr),
      m_navAlarm(nullptr),
      m_navReset(nullptr),
      m_navExit(nullptr)
{
    auto *root = new QWidget(this);
    auto *rootLayout = new QVBoxLayout(root);
    rootLayout->setContentsMargins(10, 8, 10, 8);
    rootLayout->setSpacing(6);

    auto *top = new QHBoxLayout();
    auto *logo = new QLabel("[COMPANY LOGO]", root);
    logo->setFixedSize(160, 36);
    logo->setAlignment(Qt::AlignCenter);
    logo->setStyleSheet("background: #D8EAF2; border: 1px solid #7CA2B8; color: #1A4860; font-weight: 700;");

    m_titleLabel = new QLabel(pageTitle(AlumPage), root);
    m_titleLabel->setStyleSheet("font-size: 30px; font-weight: 800; color: #10364B; letter-spacing: 1px;");

    m_dateTimeLabel = new QLabel(root);
    m_dateTimeLabel->setFixedWidth(260);
    m_dateTimeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_dateTimeLabel->setStyleSheet("font-size: 15px; font-weight: 700; color: #15435A;");

    top->addWidget(logo);
    top->addStretch();
    top->addWidget(m_titleLabel);
    top->addStretch();
    top->addWidget(m_dateTimeLabel);

    rootLayout->addLayout(top);

    m_stack->addWidget(m_welcomePage);
    m_stack->addWidget(m_alumView);
    m_stack->addWidget(m_filtrationView);
    m_stack->addWidget(m_alarmPage);
    rootLayout->addWidget(m_stack, 1);

    auto *nav = new QHBoxLayout();
    nav->setSpacing(10);

    m_navAlum = createNavButton("ALUM SOLUTION & CLARIFLOCCULATOR");
    m_navFiltration = createNavButton("FILTRATION");
    m_navWelcome = createNavButton("WELCOME");
    m_navAlarm = createNavButton("ALARM");
    m_navReset = createNavButton("RESET");
    m_navExit = createNavButton("EXIT");

    m_navAlum->setMinimumWidth(290);

    nav->addStretch();
    nav->addWidget(m_navAlum);
    nav->addWidget(m_navFiltration);
    nav->addWidget(m_navWelcome);
    nav->addWidget(m_navAlarm);
    nav->addWidget(m_navReset);
    nav->addWidget(m_navExit);
    nav->addStretch();

    rootLayout->addLayout(nav);

    connect(m_navWelcome, &QPushButton::clicked, this, &MainWindow::switchToWelcome);
    connect(m_navAlum, &QPushButton::clicked, this, &MainWindow::switchToAlum);
    connect(m_navFiltration, &QPushButton::clicked, this, &MainWindow::switchToFiltration);
    connect(m_navAlarm, &QPushButton::clicked, this, &MainWindow::switchToAlarm);
    connect(m_navReset, &QPushButton::clicked, this, &MainWindow::resetCurrentPage);
    connect(m_navExit, &QPushButton::clicked, this, &MainWindow::close);

    connect(m_clock, &QTimer::timeout, this, &MainWindow::updateDateTime);
    m_clock->start(1000);
    updateDateTime();

    setCurrentPage(AlumPage);
    setWindowTitle(pageTitle(AlumPage));
    setCentralWidget(root);
    setWindowState(Qt::WindowFullScreen);
}

QPushButton *MainWindow::createNavButton(const QString &text)
{
    auto *button = new QPushButton(text, this);
    button->setStyleSheet(
        "QPushButton {"
        "background-color: #285D7C;"
        "color: #E8F8FF;"
        "border: 1px solid #2E749A;"
        "border-radius: 6px;"
        "font-size: 13px;"
        "font-weight: 700;"
        "padding: 8px 14px;"
        "min-width: 150px;"
        "}"
        "QPushButton:hover { background-color: #2F6D90; }"
        "QPushButton:pressed { background-color: #204D66; }");
    return button;
}

QString MainWindow::pageTitle(PageIndex page) const
{
    switch (page) {
    case WelcomePage:
        return QStringLiteral("WELCOME");
    case AlumPage:
        return QStringLiteral("ALUM SOLUTION AND CLARIFLOCCULATOR");
    case FiltrationPage:
        return QStringLiteral("FILTRATION");
    case AlarmPage:
        return QStringLiteral("ALARM");
    }
    return QStringLiteral("SCADA");
}

void MainWindow::setNavActive(PageIndex page)
{
    const QString active = "QPushButton {"
                           "background-color: #1E4A63;"
                           "color: #E8F8FF;"
                           "border: 1px solid #2E749A;"
                           "border-radius: 6px;"
                           "font-size: 13px;"
                           "font-weight: 800;"
                           "padding: 8px 14px;"
                           "min-width: 150px;"
                           "}";
    const QString normal = "QPushButton {"
                           "background-color: #285D7C;"
                           "color: #E8F8FF;"
                           "border: 1px solid #2E749A;"
                           "border-radius: 6px;"
                           "font-size: 13px;"
                           "font-weight: 700;"
                           "padding: 8px 14px;"
                           "min-width: 150px;"
                           "}"
                           "QPushButton:hover { background-color: #2F6D90; }"
                           "QPushButton:pressed { background-color: #204D66; }";

    m_navWelcome->setStyleSheet(page == WelcomePage ? active : normal);
    m_navAlum->setStyleSheet(page == AlumPage ? active : normal);
    m_navFiltration->setStyleSheet(page == FiltrationPage ? active : normal);
    m_navAlarm->setStyleSheet(page == AlarmPage ? active : normal);
}

void MainWindow::setCurrentPage(PageIndex page)
{
    m_stack->setCurrentIndex(static_cast<int>(page));
    m_titleLabel->setText(pageTitle(page));
    setWindowTitle(pageTitle(page));
    setNavActive(page);
}

void MainWindow::switchToWelcome()
{
    setCurrentPage(WelcomePage);
}

void MainWindow::switchToAlum()
{
    setCurrentPage(AlumPage);
}

void MainWindow::switchToFiltration()
{
    setCurrentPage(FiltrationPage);
}

void MainWindow::switchToAlarm()
{
    setCurrentPage(AlarmPage);
}

void MainWindow::resetCurrentPage()
{
    if (m_stack->currentIndex() == FiltrationPage) {
        m_filtrationView->resetState();
    }
}

void MainWindow::updateDateTime()
{
    m_dateTimeLabel->setText(QDateTime::currentDateTime().toString("dd-MMM-yyyy  HH:mm:ss"));
}
