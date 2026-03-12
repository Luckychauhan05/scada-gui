#include "widgets/statusindicator.h"

#include <QPainter>

StatusIndicator::StatusIndicator(const QString &label, QWidget *parent)
    : QWidget(parent),
      m_online(true),
      m_state(State::Normal),
      m_text(label)
{
    setMinimumHeight(24);
}

void StatusIndicator::setOnline(bool online)
{
    if (m_online == online && m_state == (online ? State::Normal : State::Alarm)) {
        return;
    }
    m_online = online;
    m_state = online ? State::Normal : State::Alarm;
    update();
}

bool StatusIndicator::isOnline() const
{
    return m_online;
}

void StatusIndicator::setText(const QString &text)
{
    m_text = text;
    update();
}

void StatusIndicator::setState(State state)
{
    if (m_state == state) {
        return;
    }

    m_state = state;
    update();
}

StatusIndicator::State StatusIndicator::state() const
{
    return m_state;
}

void StatusIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    const QRect r = rect();
    const int circleDiameter = 12;
    const int margin = 6;
    const QRect circleRect(margin, (r.height() - circleDiameter) / 2, circleDiameter, circleDiameter);

    QColor color;
    switch (m_state) {
    case State::Normal:
        color = QColor("#4CAF50");
        break;
    case State::Warning:
        color = QColor("#FFA726");
        break;
    case State::Alarm:
        color = QColor("#E53935");
        break;
    }

    p.setPen(Qt::NoPen);
    p.setBrush(color);
    p.drawEllipse(circleRect);

    p.setPen(QColor("#DDE3EA"));
    p.drawText(circleRect.right() + 8, 0, r.width() - circleRect.right() - 8, r.height(),
               Qt::AlignVCenter | Qt::AlignLeft,
               m_text.isEmpty() ? (m_online ? "Online" : "Offline") : m_text);
}

QSize StatusIndicator::sizeHint() const
{
    return QSize(140, 24);
}
