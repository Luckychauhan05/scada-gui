#include "widgets/tankwidget.h"

#include <QPainter>
#include <QPropertyAnimation>

TankWidget::TankWidget(const QString &title, QWidget *parent)
    : QWidget(parent),
      m_title(title),
      m_level(0.0),
      m_animation(new QPropertyAnimation(this, "level", this))
{
    m_animation->setDuration(700);
    setMinimumSize(120, 180);
}

double TankWidget::level() const
{
    return m_level;
}

void TankWidget::setLevel(double level)
{
    const double bounded = qBound(0.0, level, 100.0);
    if (qFuzzyCompare(m_level, bounded)) {
        return;
    }
    m_level = bounded;
    emit levelChanged(m_level);
    update();
}

void TankWidget::animateToLevel(double targetLevel)
{
    m_animation->stop();
    m_animation->setStartValue(m_level);
    m_animation->setEndValue(qBound(0.0, targetLevel, 100.0));
    m_animation->start();
}

void TankWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const QRectF outer(25, 25, width() - 50.0, height() - 70.0);
    p.setPen(QPen(QColor("#A6B1BD"), 2));
    p.setBrush(QColor("#1B232C"));
    p.drawRoundedRect(outer, 8, 8);

    const double fillRatio = m_level / 100.0;
    const QRectF fluid(outer.left() + 4,
                       outer.bottom() - (outer.height() - 8) * fillRatio,
                       outer.width() - 8,
                       (outer.height() - 8) * fillRatio);

    QLinearGradient gradient(fluid.topLeft(), fluid.bottomLeft());
    gradient.setColorAt(0.0, QColor("#4FC3F7"));
    gradient.setColorAt(1.0, QColor("#0288D1"));
    p.setBrush(gradient);
    p.setPen(Qt::NoPen);
    p.drawRoundedRect(fluid, 6, 6);

    p.setPen(QColor("#DDE3EA"));
    p.drawText(QRect(0, 0, width(), 20), Qt::AlignCenter, m_title);
    p.drawText(QRect(0, height() - 36, width(), 24), Qt::AlignCenter,
               QString::number(m_level, 'f', 1) + "%");
}

QSize TankWidget::sizeHint() const
{
    return QSize(160, 220);
}
