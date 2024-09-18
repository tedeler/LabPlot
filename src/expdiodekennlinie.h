#ifndef EXPDIODEKENNLINIE_H
#define EXPDIODEKENNLINIE_H

#include "experiment.h"

class ExpDiodeKennlinie : public Experiment
{
public:
    explicit ExpDiodeKennlinie(QObject *parent = nullptr);
    virtual void initDatenanzeige(QGroupBox *parent, QLabel *ExperimentImage);
    virtual void deinitDatenanzeige(QGroupBox *parent);
    virtual QPointF dataToPlotXY(data_t data);
    virtual void displayData(data_t data);
};

#endif // EXPDIODEKENNLINIE_H
