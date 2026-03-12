#include "widgets/processmimicwidget.h"

#include "src/datasimulator.h"

#include <QPainter>
#include <QTimer>

ProcessMimicWidget::ProcessMimicWidget(QWidget *parent)
    : QWidget(parent),
      m_simulator(nullptr),
      m_animationTimer(new QTimer(this)),
      m_tankA(0.0),
      m_tankB(0.0),
      m_flowRate(0.0),
    m_temperature(0.0),
      m_pressure(0.0),
      m_pump1Running(false),
      m_valveAOpen(false),
    m_flowPhase(0),
    m_alarmFlashOn(false),
    m_healthState(0)
{
    setMinimumHeight(360);
    connect(m_animationTimer, &QTimer::timeout, this, &ProcessMimicWidget::advanceAnimation);
    m_animationTimer->start(120);
}

void ProcessMimicWidget::setDataSimulator(DataSimulator *simulator)
{
    if (m_simulator == simulator) {
        return;
    }

    if (m_simulator) {
        disconnect(m_simulator, &DataSimulator::dataUpdated, this, &ProcessMimicWidget::refreshData);
    }

    m_simulator = simulator;

    if (m_simulator) {
        connect(m_simulator, &DataSimulator::dataUpdated, this, &ProcessMimicWidget::refreshData);
        refreshData();
    }
}

void ProcessMimicWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), QColor("#182129"));

    const QRectF leftTank(60, 70, 120, 220);
    const QRectF rightTank(width() - 180, 70, 120, 220);
    const QPointF lineStart(leftTank.right(), leftTank.center().y());
    const QPointF lineEnd(rightTank.left(), rightTank.center().y());
    const QPointF pumpCenter(width() / 2.0 - 70, leftTank.center().y());
    const QRectF valveRect(width() / 2.0 + 30, leftTank.center().y() - 24, 44, 48);
    const bool tankAAlarm = m_tankA < 15.0 || m_tankA > 92.0;
    const bool tankBAlarm = m_tankB < 15.0 || m_tankB > 92.0;
    const bool pressureAlarm = m_pressure < 3.2 || m_pressure > 6.8;
    const bool flowAlarm = m_flowRate < 80.0 || m_flowRate > 165.0;
    const bool tempAlarm = m_temperature > 30.0;
    const bool plantCritical = m_healthState == static_cast<int>(DataSimulator::HealthState::Critical);
    const bool plantWarning = m_healthState == static_cast<int>(DataSimulator::HealthState::Warning);

    auto drawTank = [&painter, this](const QRectF &tankRect, double level, const QString &title, bool alarmed) {
        const QColor borderColor = alarmed && m_alarmFlashOn ? QColor("#EF5350") : QColor("#A6B1BD");
        painter.setPen(QPen(borderColor, alarmed ? 3 : 2));
        painter.setBrush(QColor("#1F2B35"));
        painter.drawRoundedRect(tankRect, 10, 10);

        const QRectF fluid(tankRect.left() + 5,
                           tankRect.bottom() - ((tankRect.height() - 10) * (level / 100.0)),
                           tankRect.width() - 10,
                           (tankRect.height() - 10) * (level / 100.0));
        QLinearGradient gradient(fluid.topLeft(), fluid.bottomLeft());
        gradient.setColorAt(0.0, QColor("#4FC3F7"));
        gradient.setColorAt(1.0, QColor("#01579B"));
        painter.setPen(Qt::NoPen);
        painter.setBrush(gradient);
        painter.drawRoundedRect(fluid, 8, 8);

        painter.setPen(QColor("#E2EBF3"));
        painter.drawText(QRectF(tankRect.left(), tankRect.top() - 30, tankRect.width(), 20), Qt::AlignCenter, title);
        painter.drawText(QRectF(tankRect.left(), tankRect.bottom() + 8, tankRect.width(), 22), Qt::AlignCenter,
                         QString::number(level, 'f', 1) + "%");

        if (alarmed) {
            painter.setPen(Qt::NoPen);
            painter.setBrush(m_alarmFlashOn ? QColor("#C62828") : QColor("#7F1D1D"));
            painter.drawRoundedRect(QRectF(tankRect.left() + 16, tankRect.top() + 12, tankRect.width() - 32, 24), 6, 6);
            painter.setPen(QColor("#FBE9E7"));
            painter.drawText(QRectF(tankRect.left() + 16, tankRect.top() + 12, tankRect.width() - 32, 24), Qt::AlignCenter, "ALARM");
        }
    };

    drawTank(leftTank, m_tankA, "Raw Water Tank A", tankAAlarm);
    drawTank(rightTank, m_tankB, "Process Tank B", tankBAlarm);

    const QColor pipeColor = (flowAlarm && m_alarmFlashOn) ? QColor("#F57C00") : QColor("#4B6277");
    painter.setPen(QPen(pipeColor, 12, Qt::SolidLine, Qt::RoundCap));
    painter.drawLine(lineStart, lineEnd);

    if (m_pump1Running && m_valveAOpen) {
        QPen flowPen(QColor("#4FC3F7"), 6, Qt::CustomDashLine, Qt::RoundCap);
        flowPen.setDashPattern({8, 10});
        flowPen.setDashOffset(-m_flowPhase);
        painter.setPen(flowPen);
        painter.drawLine(lineStart, lineEnd);
    }

    const QColor pumpColor = (!m_pump1Running && (flowAlarm || pressureAlarm) && m_alarmFlashOn)
                                 ? QColor("#E53935")
                                 : (m_pump1Running ? QColor("#2E7D32") : QColor("#7B1F1F"));
    painter.setPen(QPen(QColor("#90A4AE"), 2));
    painter.setBrush(pumpColor);
    painter.drawEllipse(pumpCenter, 28, 28);
    painter.setPen(QColor("#E2EBF3"));
    painter.drawText(QRectF(pumpCenter.x() - 38, pumpCenter.y() + 38, 76, 20), Qt::AlignCenter,
                     m_pump1Running ? "Pump 1 RUN" : "Pump 1 STOP");

    const QColor valveColor = (pressureAlarm && m_alarmFlashOn) ? QColor("#FF7043") : (m_valveAOpen ? QColor("#66BB6A") : QColor("#EF5350"));
    painter.setPen(QPen(valveColor, 3));
    painter.drawLine(valveRect.topLeft(), valveRect.bottomRight());
    painter.drawLine(valveRect.bottomLeft(), valveRect.topRight());
    painter.drawRect(valveRect);
    painter.setPen(QColor("#E2EBF3"));
    painter.drawText(QRectF(valveRect.left() - 18, valveRect.bottom() + 10, 80, 20), Qt::AlignCenter,
                     m_valveAOpen ? "Valve A OPEN" : "Valve A CLOSED");

    painter.setPen(QColor("#C5D4E2"));
    painter.drawText(QRectF(width() / 2.0 - 150, 28, 300, 24), Qt::AlignCenter,
                     QString("Flow %1 m³/h | Pressure %2 bar | Temp %3 °C")
                         .arg(QString::number(m_flowRate, 'f', 1))
                         .arg(QString::number(m_pressure, 'f', 2))
                         .arg(QString::number(m_temperature, 'f', 1)));

    if (plantCritical || plantWarning || tempAlarm) {
        const QColor bannerColor = plantCritical
                                       ? (m_alarmFlashOn ? QColor("#C62828") : QColor("#8E2424"))
                                       : QColor("#8A5A16");
        painter.setPen(Qt::NoPen);
        painter.setBrush(bannerColor);
        painter.drawRoundedRect(QRectF(width() / 2.0 - 150, height() - 56, 300, 30), 8, 8);
        painter.setPen(QColor("#FFF3E0"));
        painter.drawText(QRectF(width() / 2.0 - 150, height() - 56, 300, 30),
                         Qt::AlignCenter,
                         plantCritical ? "CRITICAL PROCESS CONDITION" : "PROCESS WARNING ACTIVE");
    }

    painter.setPen(QPen(QColor("#2F4152"), 1));
    painter.drawRoundedRect(rect().adjusted(1, 1, -2, -2), 10, 10);
}

QSize ProcessMimicWidget::sizeHint() const
{
    return QSize(900, 420);
}

void ProcessMimicWidget::refreshData()
{
    if (!m_simulator) {
        return;
    }

    m_tankA = m_simulator->tankALevel();
    m_tankB = m_simulator->tankBLevel();
    m_flowRate = m_simulator->flowRate();
    m_temperature = m_simulator->temperature();
    m_pressure = m_simulator->pressure();
    m_pump1Running = m_simulator->pump1Running();
    m_valveAOpen = m_simulator->valveAOpen();
    m_healthState = static_cast<int>(m_simulator->healthState());
    update();
}

void ProcessMimicWidget::advanceAnimation()
{
    m_flowPhase = (m_flowPhase + 2) % 18;
    m_alarmFlashOn = !m_alarmFlashOn;
    if (m_pump1Running && m_valveAOpen) {
        update();
        return;
    }

    if (m_healthState != static_cast<int>(DataSimulator::HealthState::Healthy)) {
        update();
    }
}
