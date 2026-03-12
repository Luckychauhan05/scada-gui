#ifndef EVENTLOGWIDGET_H
#define EVENTLOGWIDGET_H

#include <QWidget>

class QTableWidget;

class EventLogWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EventLogWidget(QWidget *parent = nullptr);

public slots:
    void addEvent(const QString &source, const QString &message, const QString &severity);

private:
    QTableWidget *m_table;
};

#endif // EVENTLOGWIDGET_H
