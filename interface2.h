#ifndef INTERFACE2_H
#define INTERFACE2_H
#include <QtGui>
#include <QWidget>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QTextStream>
#include <QMessageBox>
#include <cmath>
#include <QtCharts/QLineSeries>
#include <QtCharts/QChartView>
#include <unistd.h>

using namespace QtCharts;

namespace Ui {
class interface2;
}


class interface2 : public QWidget
{
    Q_OBJECT

public:
    explicit interface2(QWidget *parent = 0);
    ~interface2();




   QVector<QPointF> ricker(double f, double dt ,double length);
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
    const double pi = 3.14159265358979323846;

private:
 Ui::interface2 *ui;
 QChartView m_view{this};
 void plot();


private slots:
    void on_RUN_clicked();

};

#endif // INTERFACE2_H
