#ifndef INTERFACE2_H
#define INTERFACE2_H
#include <QtCharts>
#include "ui_interface2.h"

using namespace QtCharts;

namespace Ui {
class Interface2;
}

class Interface2 : public QWidget
{
    Q_OBJECT

public:
    explicit Interface2(QWidget *parent = 0);

    QVector<QPointF> ricker(double f, double dt, double length);
    QLineSeries* rickerSeries(double f);

    double length;
    int number_traces;
    int trace_samples;
    double sampling_rate;
    int polarity;
    double wavelet_freq;
    bool noise;
    bool standard_reflec;
    double f;
    double dt1;
    double dt;

private:
    Ui::Interface2 ui;
    QChartView m_view{this};
    QPushButton m_runButton{"Run"};
    void plot();

private slots:
    void on_RUN_clicked();
};

#endif // INTERFACE2_H
