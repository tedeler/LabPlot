#ifndef EXPERIMENT_H
#define EXPERIMENT_H

#include <QObject>
#include <QRectF>
#include <QGroupBox>
#include <QLineEdit>
#include <QGridLayout>
#include <QLabel>
#include "basedevice.h"


class Experiment : public QObject
{
    Q_OBJECT
public:
    explicit Experiment(QObject *parent = nullptr);
    virtual void initDatenanzeige(QGroupBox *parent, QLabel *ExperimentImage);
    virtual void deinitDatenanzeige(QGroupBox *parent);

    QPalette getValueDisplayPalette();
    virtual QLineEdit *getValueDisplayWidget(QWidget *);
    void addLabelAndValueDisplay(QWidget *parent, QGridLayout *layout, QString labletext, QString name,  int fromRow, int rowSpan);

    virtual QPointF dataToPlotXY(data_t data);
    virtual void displayData(data_t data);

    QString xlabel, ylabel;
    QRectF initialViewPort;

protected:
    QMap<QString, QLineEdit *> valueFields;
    QMap<QString, double> valueSums;
    QMap<QString, int> valueCounter;

signals:
};

#endif // EXPERIMENT_H
