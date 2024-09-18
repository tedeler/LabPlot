#ifndef EXPDIODEKENNLINIE_H
#define EXPDIODEKENNLINIE_H

#include "experiment.h"

class ExpDiodeKennlinie : public Experiment
{
public:
    explicit ExpDiodeKennlinie(QWidget *parentDatenanzeige, QLabel *experimentImage, QObject *parent = nullptr);
    virtual QPointF dataToPlotXY(data_t data);
};

#endif // EXPDIODEKENNLINIE_H
