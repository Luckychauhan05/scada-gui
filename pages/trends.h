#ifndef TRENDS_H
#define TRENDS_H

#include <QWidget>

class QComboBox;
class QChartView;
class QDateTimeAxis;
class QLineSeries;
class QValueAxis;
class QChart;
class DataSimulator;

class TrendsPage : public QWidget
{
    Q_OBJECT
public:
    explicit TrendsPage(QWidget *parent = nullptr);
    void setDataSimulator(DataSimulator *simulator);

private slots:
    void appendDataPoint();
    void updateTimeRange();

private:
    int maxPointsFromRange() const;
    void trimSeries(QLineSeries *series, int maxPoints) const;
    void updateAxisRanges(qint64 now);

    QChart *createChart(const QString &title,
                        QLineSeries *series,
                        QDateTimeAxis *&axisX,
                        QValueAxis *&axisY,
                        double minY,
                        double maxY,
                        const QString &yTitle,
                        const QColor &seriesColor);

    QLineSeries *m_tankLevelSeries;
    QLineSeries *m_flowRateSeries;
    QLineSeries *m_pressureSeries;
    QDateTimeAxis *m_tankAxisX;
    QDateTimeAxis *m_flowAxisX;
    QDateTimeAxis *m_pressureAxisX;
    QValueAxis *m_tankAxisY;
    QValueAxis *m_flowAxisY;
    QValueAxis *m_pressureAxisY;
    QComboBox *m_rangeSelector;
    DataSimulator *m_simulator;
};

#endif // TRENDS_H
