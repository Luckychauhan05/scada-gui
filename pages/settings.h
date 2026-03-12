#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>

class SettingsPage : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsPage(QWidget *parent = nullptr);
};

#endif // SETTINGS_H
