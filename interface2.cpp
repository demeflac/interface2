#include "interface2.h"
#include <QtCharts>
using namespace QtCharts;

constexpr double pi = 3.14159265358979323846;

struct RickerParameters {
    double f; // Hz
    double dt; // s
    double length; // s
    int polarity;
};

QVector<QPointF> rickerData(const RickerParameters &p)
{
    size_t N = (p.length - p.dt/2.0)/p.dt;
    const double discrete_length = (N-1) * p.dt;

    QVector<QPointF> w(N);
    for (size_t i = 0; i < N; ++i)
    {
        const double t = -discrete_length/2.0 + i*p.dt;
        w[i].setX(t);
        w[i].setY(p.polarity * ((1.0 - 2*pi*pi*p.f*p.f*t*t) * exp(-pi*pi*p.f*p.f*t*t)));
    }
    return w;
}

QLineSeries* rickerSeries(const RickerParameters &p)
{
    auto *series = new QLineSeries;
    series->setName(QStringLiteral("Ricker Wavelet for f=%1 Hz").arg(p.f, 2));
    series->replace(rickerData(p));
    return series;
}

struct Sismica1Parameters
{
    QString impedance_file = QStringLiteral("Impedancia.txt");
    int trace_count;
    int sample_count;
    double sample_rate; // Hz
    double wavelet_freq; // Hz
    int polarity; // +1/-1
    bool use_noise;
    bool use_standard_reflectivity;
};

QByteArray initFileContents(const Sismica1Parameters &sp)
{
    static const QString header = QStringLiteral(
                "////////////////////////////////////////////////////////////////////////////////////\n"
                "//                  INITIALIZATION FILE FOR SISMICA1 PROGRAM                      //\n"
                "//                                                                                //\n"
                "// This is the initialization file, it is not required for running the program    //\n"
                "// but any change on the parameters MUST be done trough it, otherwise pre-loaded  //\n"
                "// values will be used.                                                           //\n"
                "//                                                                                //\n"
                "//                                                                                //\n"
                "// It MUST be on the same directory as sismica1.exe                               //\n"
                "//                                                                                //\n"
                "// The file generated will be named: traços.txt, it will contain the convolution  //\n"
                "// results.                                                                       //\n"
                "//                                                                                //\n"
                "//                                                                                //\n"
                "// IMPORTANT  :                                                                   //\n"
                "// ==========                                                                     //\n"
                "//                                                                                //\n"
                "// Do not change the amount of lines in this file, except, if you include new para//\n"
                "// meters in the source code;                                                     //\n"
                "// DO NOT change the parameters order;                                            //\n"
                "// Use dot(.) as decimal separator;                                               //\n"
                "// There MUST NOT be any blank spaces or any non printable characters.            //\n"
                "//                                                                                //\n"
                "//                                                                                //\n"
                "// The parameters are separated by a new line character, so everything along the  //\n"
                "// line below this header, will be considered as parameter.                       //\n"
                "//                                                                                //\n"
                "//                                                                                //\n"
                "// Parameters                                                                     //\n"
                "// ==========                                                                     //\n"
                "//                                                                                //\n"
                "// Impedance file name                                                            //\n"
                "// Number of traces (0 use all the traces on file)                                //\n"
                "// Trace samples amount                                                           //\n"
                "// Sampling rate                                                                  //\n"
                "// Wavelet peak frequency                                                         //\n"
                "// Polarity                                                                       //\n"
                "// Flag indicating the use of noise                                               //\n"
                "// Flag indicating the use of standard reflectivity model                         //\n"
                "//                                                                                //\n"
                "//                                                                                //\n"
                "////////////////////////////////////////////////////////////////////////////////////\n");

    QBuffer buf;
    buf.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream str(&buf);
    str
            << header
            << sp.impedance_file << endl
            << sp.trace_count << endl
            << sp.sample_count << endl
            << sp.sample_rate / 1000.0 << endl
            << sp.wavelet_freq << endl
            << sp.polarity << endl
            << (sp.use_noise ? 1 : 0) << endl
            << (sp.use_standard_reflectivity ? 1 : 0) << endl;
    return buf.data();
}

Interface2::Interface2(QWidget *parent) :
    QWidget(parent)
{
    ui.setupUi(this);
    m_runButton.setText(tr("Run"));
    ui.buttonBox->addButton(&m_runButton, QDialogButtonBox::ApplyRole);
    connect(&m_runButton, &QPushButton::clicked, this, &Interface2::run);
    connect(ui.buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &QWidget::close);
    m_view.setWindowFlag(Qt::Window); // the view is a top-level window
}

void Interface2::run()
{
    RickerParameters p;
    p.length = ui.length->value() / 1000.0;
    p.polarity = ui.positive->isChecked() ? +1 : -1;
    p.f = ui.freq->value();
    p.dt = ui.rate->value() / 1000.0;

    Sismica1Parameters sp;
    sp.trace_count = ui.traces->value();
    sp.sample_count = ui.samples->value();
    sp.sample_rate = ui.rate->value();
    sp.wavelet_freq = ui.freq->value();
    sp.polarity = ui.positive->isChecked() ? +1 : -1;
    sp.use_noise = ui.noise->isChecked();
    sp.use_standard_reflectivity = ui.reflectivity->isChecked();

    bool saved = false;
    QSaveFile file{"sismica1.ini"};
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        file.write(initFileContents(sp));
        saved = file.commit();
    }

    plot(rickerSeries(p));
    if (!saved)
        QMessageBox::warning(this, tr("Sismica"), tr("Error: Unable to save the file."));
}

void Interface2::plot(QLineSeries *series)
{
    m_view.chart()->removeAllSeries();
    m_view.chart()->addSeries(series);
    m_view.chart()->createDefaultAxes();
    m_view.setMinimumSize(800, 600);
    m_view.show();
}
