#include "pages/trends.h"

#include "src/datasimulator.h"

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QDateTimeAxis>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>

#include <QComboBox>
#include <QDateTime>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QVBoxLayout>

TrendsPage::TrendsPage(QWidget *parent)
    : QWidget(parent),
      m_tankLevelSeries(new QLineSeries(this)),
      m_flowRateSeries(new QLineSeries(this)),
      m_pressureSeries(new QLineSeries(this)),
            m_tankAxisX(nullptr),
            m_flowAxisX(nullptr),
            m_pressureAxisX(nullptr),
            m_tankAxisY(nullptr),
            m_flowAxisY(nullptr),
            m_pressureAxisY(nullptr),
      m_rangeSelector(new QComboBox(this)),
            m_simulator(nullptr)
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);

    auto *topRow = new QHBoxLayout();
    topRow->addWidget(new QLabel("Time Range:", this));
    m_rangeSelector->addItems({"1 min", "5 min", "15 min"});
    m_rangeSelector->setCurrentIndex(1);
    topRow->addWidget(m_rangeSelector);
    topRow->addStretch();

    m_tankLevelSeries->setName("Tank Level (%)");
    m_flowRateSeries->setName("Flow Rate (m³/h)");
    m_pressureSeries->setName("Pressure (bar)");

    auto *tankChart = createChart("Tank Level vs Time",
                                  m_tankLevelSeries,
                                  m_tankAxisX,
                                  m_tankAxisY,
                                  0.0,
                                  100.0,
                                  "Level (%)",
                                  QColor("#4FC3F7"));
    auto *flowChart = createChart("Flow Rate vs Time",
                                  m_flowRateSeries,
                                  m_flowAxisX,
                                  m_flowAxisY,
                                  60.0,
                                  180.0,
                                  "Flow (m³/h)",
                                  QColor("#66BB6A"));
    auto *pressureChart = createChart("Pressure vs Time",
                                      m_pressureSeries,
                                      m_pressureAxisX,
                                      m_pressureAxisY,
                                      2.0,
                                      8.0,
                                      "Pressure (bar)",
                                      QColor("#FFB74D"));

    auto *tankChartView = new QChartView(tankChart, this);
    auto *flowChartView = new QChartView(flowChart, this);
    auto *pressureChartView = new QChartView(pressureChart, this);

    tankChartView->setRenderHint(QPainter::Antialiasing);
    flowChartView->setRenderHint(QPainter::Antialiasing);
    pressureChartView->setRenderHint(QPainter::Antialiasing);

    auto *chartsGrid = new QGridLayout();
    chartsGrid->setHorizontalSpacing(12);
    chartsGrid->setVerticalSpacing(12);
    chartsGrid->addWidget(tankChartView, 0, 0);
    chartsGrid->addWidget(flowChartView, 0, 1);
    chartsGrid->addWidget(pressureChartView, 1, 0, 1, 2);

    mainLayout->addLayout(topRow);
    mainLayout->addLayout(chartsGrid, 1);

    connect(m_rangeSelector, &QComboBox::currentTextChanged, this, &TrendsPage::updateTimeRange);
}

void TrendsPage::setDataSimulator(DataSimulator *simulator)
{
    if (m_simulator == simulator) {
        return;
    }

    if (m_simulator) {
        disconnect(m_simulator, &DataSimulator::dataUpdated, this, &TrendsPage::appendDataPoint);
    }

    m_simulator = simulator;

    if (m_simulator) {
        connect(m_simulator, &DataSimulator::dataUpdated, this, &TrendsPage::appendDataPoint);
        appendDataPoint();
    }
}

void TrendsPage::appendDataPoint()
{
    if (!m_simulator) {
        return;
    }

    const qint64 now = QDateTime::currentDateTime().toMSecsSinceEpoch();

    m_tankLevelSeries->append(now, m_simulator->tankALevel());
    m_flowRateSeries->append(now, m_simulator->flowRate());
    m_pressureSeries->append(now, m_simulator->pressure());

    const int maxPoints = maxPointsFromRange();
    trimSeries(m_tankLevelSeries, maxPoints);
    trimSeries(m_flowRateSeries, maxPoints);
    trimSeries(m_pressureSeries, maxPoints);

    updateAxisRanges(now);
}

void TrendsPage::updateTimeRange()
{
    const int maxPoints = maxPointsFromRange();
    trimSeries(m_tankLevelSeries, maxPoints);
    trimSeries(m_flowRateSeries, maxPoints);
    trimSeries(m_pressureSeries, maxPoints);
    updateAxisRanges(QDateTime::currentDateTime().toMSecsSinceEpoch());
}

int TrendsPage::maxPointsFromRange() const
{
    const QString current = m_rangeSelector->currentText();
    if (current.startsWith("1")) {
        return 60;
    }
    if (current.startsWith("5")) {
        return 300;
    }
    return 900;
}

void TrendsPage::trimSeries(QLineSeries *series, int maxPoints) const
{
    while (series->count() > maxPoints) {
        series->remove(0);
    }
}

void TrendsPage::updateAxisRanges(qint64 now)
{
    const int maxPoints = maxPointsFromRange();
    const qint64 minTime = now - static_cast<qint64>(maxPoints - 1) * 1000;
    const auto min = QDateTime::fromMSecsSinceEpoch(minTime);
    const auto max = QDateTime::fromMSecsSinceEpoch(now);

    m_tankAxisX->setRange(min, max);
    m_flowAxisX->setRange(min, max);
    m_pressureAxisX->setRange(min, max);
}

QChart *TrendsPage::createChart(const QString &title,
                                QLineSeries *series,
                                QDateTimeAxis *&axisX,
                                QValueAxis *&axisY,
                                double minY,
                                double maxY,
                                const QString &yTitle,
                                const QColor &seriesColor)
{
    axisX = new QDateTimeAxis(this);
    axisY = new QValueAxis(this);

    axisX->setFormat("HH:mm:ss");
    axisX->setTitleText("Time");
    axisY->setRange(minY, maxY);
    axisY->setTitleText(yTitle);

    QPen pen(seriesColor);
    pen.setWidth(2);
    series->setPen(pen);

    auto *chart = new QChart();
    chart->setTheme(QChart::ChartThemeDark);
    chart->setTitle(title);
    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    chart->legend()->setVisible(false);

    return chart;
}
