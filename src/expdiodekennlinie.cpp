#include "expdiodekennlinie.h"

#include <QApplication>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QWidget>

ExpDiodeKennlinie::ExpDiodeKennlinie(QObject *parent)
    : Experiment{parent}
{
    xlabel = "Ud [mV]";
    ylabel = "Id [mA]";
    initialViewPort.setBottomLeft( QPointF(0, 0) );
    initialViewPort.setTopRight(QPointF(800, 10));
}

void ExpDiodeKennlinie::initDatenanzeige(QGroupBox *parent)
{
    if (parent->layout() != nullptr) {
        QLayout *oldLayout = parent->layout();
        QLayoutItem *item;
        while ((item = oldLayout->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }
        delete oldLayout;
    }

    QVBoxLayout *vboxLayout = new QVBoxLayout;

    QHBoxLayout *hboxLayout = new QHBoxLayout;
    QLabel *label = new QLabel("UD");
    QLineEdit *lineEdit = new QLineEdit;
    QPalette palette;
    palette.setColor(QPalette::WindowText, QColor(177, 255, 129));
    palette.setColor(QPalette::Button, QColor(0, 0, 0));
    palette.setColor(QPalette::Text, QColor(100, 255, 0));
    palette.setColor(QPalette::ButtonText, QColor(100, 255, 0));
    palette.setColor(QPalette::Base, QColor(0, 0, 0));
    palette.setColor(QPalette::Window, QColor(0, 0, 0));
    palette.setColor(QPalette::Highlight, QColor(0, 0, 128));
    palette.setColor(QPalette::PlaceholderText, QColor(100, 255, 0, 128));
    lineEdit->setPalette(palette);


    hboxLayout->addWidget(label);
    hboxLayout->addWidget(lineEdit);
    vboxLayout->addLayout(hboxLayout);

    parent->setLayout(vboxLayout);
}
