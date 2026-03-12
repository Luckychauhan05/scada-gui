#ifndef TANKWIDGET_H
#define TANKWIDGET_H

#include <QWidget>

class QPropertyAnimation;

class TankWidget : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(double level READ level WRITE setLevel NOTIFY levelChanged)

public:
    explicit TankWidget(const QString &title, QWidget *parent = nullptr);

    double level() const;
    void setLevel(double level);
    void animateToLevel(double targetLevel);

signals:
    void levelChanged(double level);

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    QString m_title;
    double m_level;
    QPropertyAnimation *m_animation;
};

#endif // TANKWIDGET_H
