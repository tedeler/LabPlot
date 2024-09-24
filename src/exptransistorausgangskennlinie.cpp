#include "exptransistorausgangskennlinie.h"

ExpTransistorAusgangskennlinie::ExpTransistorAusgangskennlinie(QWidget *parentDatenanzeige, QLabel *experimentImage, QObject *parent)
    : Experiment{parentDatenanzeige, experimentImage, parent}
{
    xlabel = "Uce [V]";
    ylabel = "Ic [mA]";
    initialViewPort.setBottomLeft( QPointF(-2, -2) );
    initialViewPort.setTopRight(QPointF(2, 2));

    addLabelAndValueDisplay("Uce (Kanal 1)", "Uce", "%.1fV");
    addLabelAndValueDisplay("Urc (Kanal 2)", "Urc", "%.1fmV");
    addLabelAndValueDisplay("Ic = Urc/Rc", "Ic", "%.1fmA");

    experimentImageRessourceName = ":/fromlatex/02_AKTransistor.png";
}

QPointF ExpTransistorAusgangskennlinie::dataToPlotXY(data_t data)
{
    if(data.overflow)
        return QPointF();

    float Uce = data.channel0 ;
    float Urc_mV = 1e3*data.channel1;
    float Ic_mA = Urc_mV/22;

    addValue("Uce", Uce);
    addValue("Urc", Urc_mV);
    addValue("Ic", Ic_mA);

    return QPointF(Uce, Ic_mA);

}
