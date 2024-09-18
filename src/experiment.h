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
    explicit Experiment(QWidget *parentDatenanzeige, QLabel *experimentImage, QObject *parent = nullptr);
    virtual void initDatenanzeige();
    virtual void deinitDatenanzeige();

    QPalette getValueDisplayPalette();
    virtual QLineEdit *getValueDisplayWidget();
    void addLabelAndValueDisplay(QString labletext, QString name, QString format);

    void addValue(QString name, double value);
    void displayValue(QString name, bool print_overflow);
    void displayAllValues(bool print_overflow=false);

    virtual QPointF dataToPlotXY(data_t data);

    QString xlabel, ylabel;
    QRectF initialViewPort;

protected:
    struct ValueInformation {
        double lastValue;
        double valueSums;
        int valueCounts;
        QString displayName;
        QString displayFormat;
        QLineEdit *widget;
    };

    QMap<QString, ValueInformation> valueinfo;

    QWidget *parentDatenanzeige;
    QLabel *experimentImage;
    QString experimentImageRessourceName;

signals:
};

#endif // EXPERIMENT_H
