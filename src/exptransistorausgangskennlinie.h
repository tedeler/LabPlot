#ifndef EXPTRANSISTORAUSGANGSKENNLINIE_H
#define EXPTRANSISTORAUSGANGSKENNLINIE_H

#include "experiment.h"

class ExpTransistorAusgangskennlinie : public Experiment
{
public:
    explicit ExpTransistorAusgangskennlinie(QWidget *parentDatenanzeige, QLabel *experimentImage, QObject *parent = nullptr);
    virtual QPointF dataToPlotXY(data_t data);
};

#endif // EXPTRANSISTORAUSGANGSKENNLINIE_H
