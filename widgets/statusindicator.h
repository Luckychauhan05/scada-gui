#ifndef STATUSINDICATOR_H
#define STATUSINDICATOR_H

#include <QWidget>
#include <QString>

class StatusIndicator : public QWidget
{
    Q_OBJECT
public:
    enum class State {
        Normal,
        Warning,
        Alarm
    };

    explicit StatusIndicator(const QString &label = QString(), QWidget *parent = nullptr);

    void setOnline(bool online);
    bool isOnline() const;

    void setText(const QString &text);
    void setState(State state);
    State state() const;

protected:
    void paintEvent(QPaintEvent *event) override;
    QSize sizeHint() const override;

private:
    bool m_online;
    State m_state;
    QString m_text;
};

#endif // STATUSINDICATOR_H
