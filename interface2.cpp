#include "interface2.h"
#include "ui_interface2.h"


const double pi = 3.14159265358979323846;


QVector<QPointF> interface2::ricker(double f, double dt ,double length)
 {
   size_t N = (length - dt/2.0)/dt;
   const double discrete_length = (N-1) * dt;

   QVector<QPointF> w(N);
   for (size_t i = 0; i < N; ++i)
   {
      double t = -discrete_length/2.0 + i*dt;
      w[i].setX(t);
      w[i].setY(polarity*((1.0 - 2*pi*pi*f*f*t*t) * exp(-pi*pi*f*f*t*t)));
   }
   return w;
 }


QLineSeries* interface2::rickerSeries(double f) {
   auto *series = new QLineSeries;
   series->setName(QStringLiteral("Ricker Wavelet for f=%1").arg(f, 2));
   series->replace(interface2::ricker(f, dt, length));
   return series;

}

void interface2::plot()
{
    m_view.chart()->removeAllSeries();
    m_view.chart()->addSeries(rickerSeries(f));
    m_view.chart()->createDefaultAxes();
    m_view.setMinimumSize(800, 600);
    m_view.show();
}

interface2::interface2(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::interface2)
{
    ui->setupUi(this);
    m_view.setWindowFlag(Qt::Window);
}


interface2::~interface2()
{
    delete ui;

}


void interface2::on_RUN_clicked()
{

    length=ui->length->value();
    number_traces = ui->traces->value();
    trace_samples = ui->samples->value();
    sampling_rate = ui->rate->value();
    dt1=ui->rate->value();
    wavelet_freq = ui->freq->value();
    f=ui->freq->value();
    dt=dt1/1000;

    {
        if(ui->reflectivity->isChecked())/*refletividade padrao*/
        {
         standard_reflec= 1;
        }
        else
        {
         standard_reflec= 0;
        }
    }


    {
        if(ui->Noise->isChecked())/*introduzir ruido*/
        {
          noise= 1;
        }
        else
        {
         noise= 0;
        }
    }

    {
        if(ui->negative->isChecked())/*polaridade negativa*/
        {
          polarity=-1;
        }

    }

    {
        if(ui->positive->isChecked())/*polaridade positiva*/
        {
          polarity=1;
        }
    }


  {

    QString outputFilename = "sismica1.ini";
    QFile outputFile(outputFilename);
    outputFile.open(QIODevice::WriteOnly|QIODevice::Text);

    /* Check it opened OK */
    if(!outputFile.isOpen())
    {
        QMessageBox::warning(this,tr("Sismica"),tr("- Error, unable to open sismica1.ini for output"));

    }

    /* Point a QTextStream object at the file */
    QTextStream outStream(&outputFile);

    /* Write the line to the file */
    outStream << "////////////////////////////////////////////////////////////////////////////////////"<<endl
              << "//                  INITIALIZATION FILE FOR SISMICA1 PROGRAM                      //"<<endl
              << "//                                                                                //"<<endl
              << "// This is the initialization file, it is not required for running the program    //"<<endl
              << "// but any change on the parameters MUST be done trough it, otherwise pre-loaded  //"<<endl
              << "// values will be used.                                                           //"<<endl
              << "//                                                                                //"<<endl
              << "//                                                                                //"<<endl
              << "// It MUST be on the same directory as sismica1.exe                               //"<<endl
              << "//                                                                                //"<<endl
              << "// The file generated will be named: traços.txt, it will contain the convolution  //"<<endl
              << "// results.                                                                       //"<<endl
              << "//                                                                                //"<<endl
              << "//                                                                                //"<<endl
              << "// IMPORTANT  :                                                                   //"<<endl
              << "// ==========                                                                     //"<<endl
              << "//                                                                                //"<<endl
              << "// Do not change the amount of lines in this file, except, if you include new para//"<<endl
              << "// meters on the source code;                                                     //"<<endl
              << "// DO NOT change the parameters order;                                            //"<<endl
              << "// Use dot(.) as decimal separator;                                               //"<<endl
              << "// There MUST NOT be any blank spaces or any non printable charactere.            //"<<endl
              << "//                                                                                //"<<endl
              << "//                                                                                //"<<endl
              << "// The parameters are separated by a new line character, so everything along the  //"<<endl
              << "// line below this header, will be considered as parameter.                       //"<<endl
              << "//                                                                                //"<<endl
              << "//                                                                                //"<<endl
              << "// Parameters                                                                     //"<<endl
              << "// ==========                                                                     //"<<endl
              << "//                                                                                //"<<endl
              << "// Impedance file name                                                            //"<<endl
              << "// Number of traces (0 use all the traces on file)                                //"<<endl
              << "// Trace samples amount                                                           //"<<endl
              << "// Sampling rate                                                                  //"<<endl
              << "// Wavelet peak frequency                                                         //"<<endl
              << "// Polarity                                                                       //"<<endl
              << "// Flag indicating the use of noise                                               //"<<endl
              << "// Flag indicating the use of standard reflectivity model                         //"<<endl
              << "//                                                                                //"<<endl
              << "//                                                                                //"<<endl
              << "///////////////////////////////////////////////////////////////////////////////////"<<endl
              << "Impedancia.txt"<<"\n"
              << number_traces<<"\n"
              << trace_samples<<"\n"
              << sampling_rate/1000<<"\n"
              << wavelet_freq<<"\n"
              << polarity<<"\n"
              << noise<<"\n"
              << standard_reflec<<"\n";

    /* Close the file */
    outputFile.close();

 }
    plot();
}


