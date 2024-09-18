#include "labplotwindow.h"
#include "ui_labplotwindow.h"

#include <qwt_symbol.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_renderer.h>
#include <QMessageBox>
#include <QDateTime>

#include <QClipboard>
#include <QImage>
#include <QPainter>
#include <QApplication>
#include <QDebug>
#include "expdiodekennlinie.h"


LabPlotWindow::LabPlotWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LabPlotWindow), m_curves(2), m_curve_data(2)
{
    ui->setupUi(this);
    m_currentExperiment = new ExpDiodeKennlinie();
    m_currentExperiment->initDatenanzeige(ui->gbDatenanzeige);

    QString s;
    s.sprintf("LabPlot V%d.%d", Version_MAJOR, Version_MINOR);
    this->setWindowTitle(s);

    //Knöpfe verbinden
    connect(ui->pb_connect, SIGNAL(clicked()), this, SLOT(connect_device()));
    connect(ui->pb_delrecords, SIGNAL(clicked()), this, SLOT(delrecords()));
    connect(ui->pb_clipboard, SIGNAL(clicked()), this, SLOT(copy_clipboard()));

    //Menü konfigurieren
    connect(ui->menuBar, SIGNAL(triggered(QAction*)), this, SLOT(menu_triggered(QAction *)));

    //Plot erstellen
    ui->plot_layout->addWidget(&this->m_plot);

    //Plot Hintergrundfarbe setzen
    QPalette MyPal;
    MyPal.setColor(QPalette::All, QPalette::Window, Qt::darkGreen);
    m_plot.canvas()->setPalette(MyPal);

    //Plot Gitter erstellen und Farbe setzen
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setPen(Qt::green);
    grid->attach(&m_plot);

    //Achsen beschriften und Bereich einstellen
    QRectF vp = m_currentExperiment->initialViewPort;
    m_plot.setAxisScale(m_plot.xBottom, vp.left(), vp.right());
    m_plot.setAxisScale(m_plot.yLeft, vp.bottom(), vp.top());
    m_plot.setAxisTitle(m_plot.yLeft, m_currentExperiment->ylabel);
    m_plot.setAxisTitle(m_plot.xBottom, m_currentExperiment->xlabel);

    //Kurve hinzufügen
    m_curves[0] = new QwtPlotCurve();
    m_curves[0]->setPen(Qt::yellow, 2);
    m_curves[0]->setStyle(QwtPlotCurve::Dots);
    m_curves[0]->attach(&m_plot);

    m_curves[1] = new QwtPlotCurve();
    m_curves[1]->setPen(Qt::red, 2);
    m_curves[1]->setStyle(QwtPlotCurve::Dots);
    m_curves[1]->attach(&m_plot);

    //Aktiviere Zoom und Pan
/*    m_zoomer = new QwtPlotZoomer(QwtPlot::xBottom, QwtPlot::yLeft, m_plot.canvas());
    m_zoomer->setRubberBand( QwtPicker::RectRubberBand );
    m_zoomer->setRubberBandPen( QColor( Qt::blue ) );
    m_zoomer->setTrackerMode( QwtPicker::ActiveOnly );
    m_zoomer->setTrackerPen( QColor( Qt::white ) );
*/
    m_panner = new QwtPlotPanner(m_plot.canvas());
//    m_panner->setMouseButton(Qt::MidButton);
    m_magnifier = new QwtPlotMagnifier(m_plot.canvas());
//    m_magnifier->setWheelFactor(-0.9);

    //Füge Marker hinzu
    QwtSymbol *sym=new QwtSymbol(QwtSymbol::Cross,QBrush(Qt::red),QPen(Qt::yellow),QSize(20,20));
    sym->setPen(Qt::red, 1);
    m_marker.setSymbol(sym);
    m_marker.attach(&m_plot);


    //setup measurement device
    connect(&m_device, SIGNAL(new_data()), this, SLOT(plot_new_data()));
    connect(&m_device, SIGNAL(state_changed()), this, SLOT(device_state_changed()));
    connect_device();

    //Statusleiste beschriften
    QLabel *L = new QLabel("Linke Maustaste: Plot verschieben");
    ui->statusBar->addWidget(L);
    L = new QLabel("Mausrad: Zoom");
    ui->statusBar->addWidget(L);
}

void LabPlotWindow::closeEvent(QCloseEvent *event)
{
    qDebug() << "Enter close event";
    if(m_device.is_connected())
            m_device.dev_disconnect();
    qDebug() << "Leave close event";
}

LabPlotWindow::~LabPlotWindow()
{

    qDebug() << "Destroy window object";
    delete ui;
    delete m_curves[0];
    delete m_curves[1];
//    delete m_zoomer;
    delete m_panner;
    delete m_magnifier;
}


void LabPlotWindow::connect_device()
{
   m_device.dev_connect();
}

void LabPlotWindow::device_state_changed()
{
    Labjack::EDeviceState state = m_device.get_device_state();
    QString info = m_device.get_device_state_info_str();

    qDebug() << "STATE is now " << state << info;

    QString state_str;

    switch(state)
    {
        case Labjack::EDS_CONNECTED:
            ui->pb_connect->setEnabled(false);
            state_str = "Verbunden";
            break;
        case Labjack::EDS_DISCONNECTED:
            ui->pb_connect->setEnabled(true);
            state_str = "Nicht verbunden";
            break;
        case Labjack::EDS_ERROR:
            ui->pb_connect->setEnabled(true);
            state_str = "Fehler: "+info;
            break;
        default:
            ui->pb_connect->setEnabled(false);
            state_str = "Unbekannt";
            break;
    }

    ui->lb_device_status->setText(state_str);
}

void LabPlotWindow::delrecords()
{
    for(int i=0; i<m_curves.size(); i++)
    {
        m_curve_data[i].clear();
        m_curves[i]->setSamples(m_curve_data[i]);
    }

    ui->lb_datainfo->setText("Aufzeichnung gelöscht");
    m_plot.replot();
    m_plot.repaint();
}

void LabPlotWindow::copy_clipboard()
{
    qDebug() << "Copy image to clipboard";

    QClipboard *clipboard = QApplication::clipboard();
    QwtPlotRenderer renderer;
    QImage img(m_plot.size(), QImage::Format_ARGB32);
    QPainter painter(&img);
    renderer.render(&m_plot, &painter, m_plot.rect());


    QDateTime current = QDateTime::currentDateTime();
    QString text = current.toString();

    painter.drawText(img.rect(), Qt::AlignCenter, text);

    clipboard->setImage(img);
}


void LabPlotWindow::plot_new_data()
{

}
/*{
    QString DisplayString;
    static qint64 nextDisplayTime = 0;


    while(this->m_device.dataAvailable())
    {
        data_t data = m_device.getData();
        float Ur3_mV = data.channel0 * 1e3;
        float Id_mA = Ur3_mV / 1e3;
        float Ud_mV = data.channel1 * 1e3;
        static float sum_ur3=0, sum_id=0, sum_ud=0;
        static int sumcounter=0;

//        qDebug() << "[RCV] Time: " << data.time << " Ud_mV: " << Ud_mV << " Id_mA: " << Id_mA;

        if (data.overflow)
        {
            ui->ln_id->setText("Overflow");
            ui->ln_ud->setText("Overflow");
            ui->ln_ur3->setText("Overflow");
            nextDisplayTime = 0;
        }
        else
        {
            m_marker.setValue(Ud_mV, Id_mA);
            QPointF datapoint(Ud_mV, Id_mA);
            if (ui->cb_record->isChecked())
            {
                m_curve_data[0].append(datapoint);
            }
            sum_ur3 += Ur3_mV;
            sum_ud += Ud_mV;
            sum_id += Id_mA;
            sumcounter += 1;
            if (QDateTime::currentMSecsSinceEpoch() > nextDisplayTime)
            {
                qDebug() << sumcounter;
                QString s;
                s.sprintf("%.2fmA", sum_id/sumcounter);
                ui->ln_id->setText(s);
                s.sprintf("%.2fmV", sum_ud/sumcounter);
                ui->ln_ud->setText(s);
                s.sprintf("%.2fmV", sum_ur3/sumcounter);
                ui->ln_ur3->setText(s);

                nextDisplayTime = QDateTime::currentMSecsSinceEpoch() + 250;
                sum_ur3 = 0;
                sum_ud = 0;
                sum_id = 0;
                sumcounter = 0;
            }
        }
    }
    QString s;
    s.sprintf("Anzahl Datenpunkte: %d", m_curve_data[0].size());
    ui->lb_datainfo->setText(s);
    m_curves[0]->setSamples(m_curve_data[0]);

    m_plot.replot();
    m_plot.repaint();

    ui->lb_datadisplay->setText(DisplayString);
}
*/
void LabPlotWindow::menu_triggered(QAction *action)
{
    //About box
    if(action->objectName() == "action_copy")
    {
        copy_clipboard();
    }
    if(action->objectName() == "action_close")
    {
        QCoreApplication::quit();
    }
    if(action->objectName() == "about")
    {
        QMessageBox msgBox;
        QString s;
        s.sprintf("LabPlot V%d.%d\n(c) 2024 Prof. Dr. Edeler\n\nBuild date: %s\nGitid: %s",
                  Version_MAJOR, Version_MINOR, Version_DATESTR, Version_GITID);
        msgBox.setText(s);
        msgBox.exec();
    }
}
