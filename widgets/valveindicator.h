#ifndef VALVEINDICATOR_H
#define VALVEINDICATOR_H

#include <QWidget>

class ValveIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit ValveIndicator(const QString &name, bool open = true, QWidget *parent = nullptr);

    void setOpen(bool open);
    bool isOpen() const;
    QString name() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QString m_name;
    bool m_open;
};

#endif // VALVEINDICATOR_H
