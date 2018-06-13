#ifndef INTERFACE2_H
#define INTERFACE2_H
#include <QChartView>
#include <QLineSeries>
#include <QPushButton>
#include "ui_interface2.h"

class Interface2 : public QWidget
{
    Q_OBJECT
public:
    explicit Interface2(QWidget *parent = 0);

private:
    Ui::Interface2 ui;
    QPushButton m_runButton;
    QtCharts::QChartView m_view{this};
    void run();
    void plot(QtCharts::QLineSeries *);
};

#endif // INTERFACE2_H
