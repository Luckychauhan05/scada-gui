#include "widgets/sensorcard.h"

#include <QLabel>
#include <QVBoxLayout>

SensorCard::SensorCard(const QString &title, const QString &unit, QWidget *parent)
    : QFrame(parent),
      m_titleLabel(new QLabel(title, this)),
      m_valueLabel(new QLabel("0.0", this)),
      m_statusLabel(new QLabel("Healthy", this)),
      m_unit(unit)
{
    setObjectName("sensorCard");
    setFrameShape(QFrame::StyledPanel);

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(8);

    m_titleLabel->setStyleSheet("font-size: 13px; color: #9FB0C0;");
    m_valueLabel->setStyleSheet("font-size: 22px; font-weight: 600; color: #E7EEF6;");
    m_statusLabel->setStyleSheet("font-size: 12px; color: #66BB6A;");

    layout->addWidget(m_titleLabel);
    layout->addWidget(m_valueLabel);
    layout->addWidget(m_statusLabel);
}

void SensorCard::setValue(double value)
{
    m_valueLabel->setText(QString::number(value, 'f', 1) + " " + m_unit);
}

void SensorCard::setHealthy(bool healthy)
{
    m_statusLabel->setText(healthy ? "Healthy" : "Warning");
    m_statusLabel->setStyleSheet(healthy
                                     ? "font-size: 12px; color: #66BB6A;"
                                     : "font-size: 12px; color: #FFA726;");
}
