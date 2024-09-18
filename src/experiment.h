#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QObject>
#include <QRectF>
#include <QGroupBox>


class Experiment : public QObject
{
    Q_OBJECT
public:
    explicit Experiment(QObject *parent = nullptr);
    virtual void initDatenanzeige(QGroupBox *parent);
    QString xlabel, ylabel;
    QRectF initialViewPort;

signals:
};

#endif // EXPERIMENT_H
