#include "pages/filtrationdashboard.h"

#include "widgets/processflowview.h"

#include <QDateTime>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QVBoxLayout>

FiltrationDashboardPage::FiltrationDashboardPage(QWidget *parent)
    : QWidget(parent),
      m_dateTimeLabel(new QLabel(this)),
      m_flowView(new ProcessFlowView(this)),
      m_clock(new QTimer(this))
{
    setObjectName("filtrationRoot");

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(12, 10, 12, 10);
    root->setSpacing(8);

    auto *top = new QHBoxLayout();
    top->setContentsMargins(8, 4, 8, 0);

    auto *leftGap = new QLabel(this);
    leftGap->setFixedWidth(220);

    auto *title = new QLabel("FILTRATION", this);
    title->setStyleSheet("font-size: 38px; font-weight: 800; color: #0E3348; letter-spacing: 2px; background: transparent;");

    m_dateTimeLabel->setFixedWidth(280);
    m_dateTimeLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_dateTimeLabel->setStyleSheet("font-size: 16px; font-weight: 700; color: #12435C; background: transparent;");

    top->addWidget(leftGap);
    top->addStretch();
    top->addWidget(title);
    top->addStretch();
    top->addWidget(m_dateTimeLabel);

    root->addLayout(top);
    root->addWidget(m_flowView, 1);

    auto *bottom = new QHBoxLayout();
    bottom->setContentsMargins(10, 0, 10, 0);
    bottom->setSpacing(14);

    const QString btnStyle = "QPushButton {"
                             "background-color: #1F5674;"
                             "color: #ECF8FF;"
                             "border: 1px solid #2B7197;"
                             "border-radius: 6px;"
                             "font-size: 14px;"
                             "font-weight: 700;"
                             "padding: 9px 18px;"
                             "min-width: 110px;"
                             "}"
                             "QPushButton:hover { background-color: #2A688B; }"
                             "QPushButton:pressed { background-color: #18475F; }";

    auto makeBtn = [this, &btnStyle](const QString &text) {
        auto *b = new QPushButton(text, this);
        b->setStyleSheet(btnStyle);
        return b;
    };

    auto *welcome = makeBtn("Welcome");
    auto *filtration = makeBtn("Filtration");
    auto *alarm = makeBtn("Alarm");
    auto *reset = makeBtn("Reset");
    auto *exit = makeBtn("Exit");

    bottom->addStretch();
    bottom->addWidget(welcome);
    bottom->addWidget(filtration);
    bottom->addWidget(alarm);
    bottom->addWidget(reset);
    bottom->addWidget(exit);
    bottom->addStretch();
    root->addLayout(bottom);

    connect(m_clock, &QTimer::timeout, this, &FiltrationDashboardPage::updateDateTime);
    connect(welcome, &QPushButton::clicked, [this]() { m_flowView->centerOn(500, 260); });
    connect(filtration, &QPushButton::clicked, [this]() { m_flowView->centerOn(790, 360); });
    connect(alarm, &QPushButton::clicked, m_flowView, &ProcessFlowView::toggleAlarmState);
    connect(reset, &QPushButton::clicked, m_flowView, &ProcessFlowView::resetState);
    connect(exit, &QPushButton::clicked, [this]() {
        if (window()) {
            window()->close();
        }
    });

    m_clock->start(1000);
    updateDateTime();
}

void FiltrationDashboardPage::updateDateTime()
{
    m_dateTimeLabel->setText(QDateTime::currentDateTime().toString("dd-MMM-yyyy  HH:mm:ss"));
}
