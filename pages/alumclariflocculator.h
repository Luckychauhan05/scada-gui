#ifndef ALUMCLARIFLOCCULATOR_H
#define ALUMCLARIFLOCCULATOR_H

#include <QWidget>

class QLabel;
class QTimer;
class AlumClariflocculatorView;
class QDialog;

class AlumClariflocculatorPage : public QWidget
{
    Q_OBJECT
public:
    explicit AlumClariflocculatorPage(QWidget *parent = nullptr);

private slots:
    void updateDateTime();
    void openFiltration();

private:
    QLabel *m_dateTime;
    AlumClariflocculatorView *m_view;
    QTimer *m_timer;
    QDialog *m_filtrationDialog;
};

#endif // ALUMCLARIFLOCCULATOR_H
