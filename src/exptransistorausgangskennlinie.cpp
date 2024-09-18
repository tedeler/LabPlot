#include "exptransistorausgangskennlinie.h"

ExpTransistorAusgangskennlinie::ExpTransistorAusgangskennlinie(QWidget *parentDatenanzeige, QLabel *experimentImage, QObject *parent)
    : Experiment{parentDatenanzeige, experimentImage, parent}
{
    xlabel = "Uce [V]";
    ylabel = "Ic [mA]";
    initialViewPort.setBottomLeft( QPointF(-2, -2) );
    initialViewPort.setTopRight(QPointF(2, 2));

    addLabelAndValueDisplay("Uce", "UCE", "%.1fV");
    addLabelAndValueDisplay("Ic", "IC", "%.1fmA");

    experimentImageRessourceName = ":/BJT_AK_Messschaltung.png";
}

QPointF ExpTransistorAusgangskennlinie::dataToPlotXY(data_t data)
{
    if(data.overflow)
        return QPointF();

    float Uce = data.channel0 ;
    float Ic = data.channel1;

    addValue("UCE", Uce);
    addValue("IC", Ic);

    return QPointF(Uce, Ic);

}
