#include "widgets/valveindicator.h"

#include <QPainter>

ValveIndicator::ValveIndicator(const QString &name, bool open, QWidget *parent)
    : QWidget(parent),
      m_name(name),
      m_open(open)
{
    setFixedSize(64, 38);
    setAttribute(Qt::WA_TranslucentBackground, true);
}

void ValveIndicator::setOpen(bool open)
{
    if (m_open == open) {
        return;
    }
    m_open = open;
    update();
}

bool ValveIndicator::isOpen() const
{
    return m_open;
}

QString ValveIndicator::name() const
{
    return m_name;
}

void ValveIndicator::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    p.setPen(QPen(QColor("#18435B"), 1.6));
    p.setBrush(QColor("#EAF5FA"));

    const QPolygonF valveShape{QPointF(10, 19), QPointF(20, 10), QPointF(30, 19), QPointF(20, 28)};
    p.drawPolygon(valveShape);
    p.drawLine(QPointF(6, 19), QPointF(34, 19));

    p.setPen(Qt::NoPen);
    p.setBrush(m_open ? QColor("#2ECC71") : QColor("#E74C3C"));
    p.drawEllipse(QRectF(39, 11, 12, 12));

    p.setPen(QColor("#0E3447"));
    p.setFont(QFont("Segoe UI", 8, QFont::Bold));
    p.drawText(QRectF(0, 25, width(), 12), Qt::AlignCenter, m_name);
}
