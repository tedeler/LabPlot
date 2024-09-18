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

ExpDiodeKennlinie::ExpDiodeKennlinie(QObject *parent)
    : Experiment{parent}
{
    xlabel = "Ud [mV]";
    ylabel = "Id [mA]";
    initialViewPort.setBottomLeft( QPointF(0, 0) );
    initialViewPort.setTopRight(QPointF(800, 10));
}

void ExpDiodeKennlinie::initDatenanzeige(QGroupBox *gbDatenanzeige, QLabel *ExperimentImage)
{
    QGridLayout *gridLayout_2 = (QGridLayout *) gbDatenanzeige->layout();
    addLabelAndValueDisplay(gbDatenanzeige, gridLayout_2, "UD", "UD", 0, 1);
    addLabelAndValueDisplay(gbDatenanzeige, gridLayout_2, "UR2", "UR2", 1, 1);
    addLabelAndValueDisplay(gbDatenanzeige, gridLayout_2, "ID", "ID", 2, 1);

    ExperimentImage->setPixmap(QPixmap(QString::fromUtf8(":/Dioden_Messschaltung.png")));
    ExperimentImage->setScaledContents(true);


    valueSums["UR2"] = 0; valueCounter["UR2"] = 0;
    valueSums["ID"] = 0; valueCounter["ID"] = 0;
    valueSums["UD"] = 0; valueCounter["UD"] = 0;

}

void ExpDiodeKennlinie::deinitDatenanzeige(QGroupBox *parent)
{
    QLayout *gridlayout = parent->layout();

    QLayoutItem *item;
    while ((item = gridlayout->takeAt(0)) != nullptr) {
        if (QWidget *widget = item->widget()) {
            widget->deleteLater();
        }
        delete item;
    }
}

QPointF ExpDiodeKennlinie::dataToPlotXY(data_t data)
{
    if(data.overflow)
        return QPointF();

    float Ur2_mV = data.channel0 * 1e3;
    float Id_mA = Ur2_mV / 1e3;
    float Ud_mV = data.channel1 * 1e3;

    valueSums["UR2"] += Ur2_mV; valueCounter["UR2"]++;
    valueSums["ID"] += Id_mA; valueCounter["ID"]++;
    valueSums["UD"] += Ud_mV; valueCounter["UD"]++;
    return QPointF(Ud_mV, Id_mA);
}

void ExpDiodeKennlinie::displayData(data_t data)
{
    QString id;
    if (data.overflow)
    {
        valueFields["UD"]->setText("Overflow");
        valueFields["UR2"]->setText("Overflow");
        valueFields["ID"]->setText("Overflow");

        id = "ID"; valueSums[id] = 0; valueCounter[id] = 0;
        id = "UD"; valueSums[id] = 0; valueCounter[id] = 0;
        id = "UR2"; valueSums[id] = 0; valueCounter[id] = 0;

    }
    else
    {
        QString s;
        id = "ID";
        s.asprintf("%.2fmA", valueSums[id]/valueCounter[id]);
        valueFields[id]->setText(s);
        valueSums[id] = 0; valueCounter[id] = 0;

        id = "UD";
        s.asprintf("%.2fmV", valueSums[id]/valueCounter[id]);
        valueFields[id]->setText(s);
        valueSums[id] = 0; valueCounter[id] = 0;

        id = "UR2";
        s.asprintf("%.2fmV", valueSums[id]/valueCounter[id]);
        valueFields[id]->setText(s);
        valueSums[id] = 0; valueCounter[id] = 0;
    }

}
