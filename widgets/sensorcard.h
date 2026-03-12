#ifndef SENSORCARD_H
#define SENSORCARD_H

#include <QFrame>

class QLabel;

class SensorCard : public QFrame
{
    Q_OBJECT
public:
    explicit SensorCard(const QString &title, const QString &unit, QWidget *parent = nullptr);

    void setValue(double value);
    void setHealthy(bool healthy);

private:
    QLabel *m_titleLabel;
    QLabel *m_valueLabel;
    QLabel *m_statusLabel;
    QString m_unit;
};

#endif // SENSORCARD_H
