#include "expdiodekennlinie.h"

#include <QApplication>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWidget>
#include <QDebug>

ExpDiodeKennlinie::ExpDiodeKennlinie(QWidget *parentDatenanzeige, QLabel *experimentImage, QObject *parent)
    : Experiment{parentDatenanzeige, experimentImage, parent}
{
    xlabel = "Ud [mV]";
    ylabel = "Id [mA]";
    initialViewPort.setBottomLeft( QPointF(0, 0) );
    initialViewPort.setTopRight(QPointF(800, 10));

    addLabelAndValueDisplay("UD", "UD", "%.1fmV");
    addLabelAndValueDisplay("UR2", "UR2", "%.1fmV");
    addLabelAndValueDisplay("ID", "ID", "%.1fmA");

    experimentImageRessourceName = ":/Dioden_Messschaltung.png";
}


QPointF ExpDiodeKennlinie::dataToPlotXY(data_t data)
{
    if(data.overflow)
        return QPointF();

    float Ur2_mV = data.channel0 * 1e3;
    float Id_mA = Ur2_mV / 1e3;
    float Ud_mV = data.channel1 * 1e3;

    addValue("UR2", Ur2_mV);
    addValue("ID", Id_mA);
    addValue("UD", Ud_mV);

    return QPointF(Ud_mV, Id_mA);
}

