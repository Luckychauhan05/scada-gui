#include "pages/alumclariflocculator.h"

#include "pages/filtrationdashboard.h"
#include "widgets/alumclariflocculatorview.h"

#include <QDateTime>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

AlumClariflocculatorPage::AlumClariflocculatorPage(QWidget *parent)
    : QWidget(parent),
      m_dateTime(new QLabel(this)),
      m_view(new AlumClariflocculatorView(this)),
            m_timer(new QTimer(this)),
            m_filtrationDialog(nullptr)
{
    setObjectName("alumRoot");

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(10, 8, 10, 8);
    root->setSpacing(6);

    auto *top = new QHBoxLayout();
    auto *logo = new QLabel("[COMPANY LOGO]", this);
    logo->setFixedSize(160, 36);
    logo->setAlignment(Qt::AlignCenter);
    logo->setStyleSheet("background: #D8EAF2; border: 1px solid #7CA2B8; color: #1A4860; font-weight: 700;");

    auto *title = new QLabel("ALUM SOLUTION AND CLARIFLOCCULATOR", this);
    title->setStyleSheet("font-size: 30px; font-weight: 800; color: #10364B; letter-spacing: 1px;");

    m_dateTime->setFixedWidth(260);
    m_dateTime->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_dateTime->setStyleSheet("font-size: 15px; font-weight: 700; color: #15435A;");

    top->addWidget(logo);
    top->addStretch();
    top->addWidget(title);
    top->addStretch();
    top->addWidget(m_dateTime);

    root->addLayout(top);
    root->addWidget(m_view, 1);

    auto *nav = new QHBoxLayout();
    nav->setSpacing(10);

    const QString btnStyle = "QPushButton {"
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

    auto make = [this, &btnStyle](const QString &text) {
        auto *b = new QPushButton(text, this);
        b->setStyleSheet(btnStyle);
        return b;
    };

    auto *b1 = make("ALUM SOLUTION & CLARIFLOCCULATOR");
    auto *b2 = make("FILTRATION");
    auto *b3 = make("WELCOME");
    auto *b4 = make("ALARM");
    auto *b6 = make("RESET");
    auto *b7 = make("EXIT");

    b1->setMinimumWidth(290);

    nav->addStretch();
    nav->addWidget(b1);
    nav->addWidget(b2);
    nav->addWidget(b3);
    nav->addWidget(b4);
    nav->addWidget(b6);
    nav->addWidget(b7);
    nav->addStretch();

    root->addLayout(nav);

    connect(m_timer, &QTimer::timeout, this, &AlumClariflocculatorPage::updateDateTime);
    connect(b2, &QPushButton::clicked, this, &AlumClariflocculatorPage::openFiltration);
    connect(b7, &QPushButton::clicked, [this]() {
        if (window()) {
            window()->close();
        }
    });

    m_timer->start(1000);
    updateDateTime();
}

void AlumClariflocculatorPage::updateDateTime()
{
    m_dateTime->setText(QDateTime::currentDateTime().toString("dd-MMM-yyyy  HH:mm:ss"));
}

void AlumClariflocculatorPage::openFiltration()
{
    if (m_filtrationDialog && m_filtrationDialog->isVisible()) {
        m_filtrationDialog->raise();
        m_filtrationDialog->activateWindow();
        return;
    }

    m_filtrationDialog = new QDialog(this);
    m_filtrationDialog->setAttribute(Qt::WA_DeleteOnClose, true);
    m_filtrationDialog->setWindowTitle("FILTRATION");
    m_filtrationDialog->resize(1280, 860);

    auto *layout = new QVBoxLayout(m_filtrationDialog);
    layout->setContentsMargins(0, 0, 0, 0);

    auto *page = new FiltrationDashboardPage(m_filtrationDialog);
    layout->addWidget(page);

    connect(m_filtrationDialog, &QDialog::destroyed, this, [this]() {
        m_filtrationDialog = nullptr;
    });

    m_filtrationDialog->show();
}
