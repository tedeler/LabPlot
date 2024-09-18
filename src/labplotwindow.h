#ifndef LABPLOTWINDOW_H
#define LABPLOTWINDOW_H

#include <QMainWindow>
#include <QVector>

#include <qwt_plot_marker.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_curve.h>
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>

#include "labjack.h"
#include "experiment.h"

namespace Ui {
class LabPlotWindow;
}

class LabPlotWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LabPlotWindow(QWidget *parent = 0);
    ~LabPlotWindow();
protected:
    void closeEvent(QCloseEvent *);

public slots:
    void menu_triggered(QAction *);
    void delrecords();
    void copy_clipboard();
    void connect_device();

    void loadExperiment(Experiment *newExperiment);

private slots:
    void plot_new_data();
    void device_state_changed();

private:
    Ui::LabPlotWindow *ui;
    QwtPlot m_plot;
    QVector<QwtPlotCurve *> m_curves;
    QVector<QVector<QPointF>> m_curve_data;

    QwtPlotZoomer *m_zoomer;
    QwtPlotPanner *m_panner;
    QwtPlotMagnifier *m_magnifier;
    QwtPlotMarker m_marker;

    Labjack m_device;
    Experiment *m_currentExperiment;


};

#endif // LABPLOTWINDOW_H
