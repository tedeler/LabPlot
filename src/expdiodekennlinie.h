#ifndef EXPDIODEKENNLINIE_H
#define EXPDIODEKENNLINIE_H

#include "experiment.h"

class ExpDiodeKennlinie : public Experiment
{
public:
    explicit ExpDiodeKennlinie(QObject *parent = nullptr);
    virtual void initDatenanzeige(QGroupBox *parent);
};

#endif // EXPDIODEKENNLINIE_H
