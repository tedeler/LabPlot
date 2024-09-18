#include "experiment.h"

Experiment::Experiment(QObject *parent)
    : QObject{parent}
{}

void Experiment::initDatenanzeige(QGroupBox *parent, QLabel *ExperimentImage)
{
}

void Experiment::deinitDatenanzeige(QGroupBox *parent)
{
}

QPalette Experiment::getValueDisplayPalette()
{
    QPalette palette;
    QBrush brush(QColor(177, 255, 129, 255));
    brush.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
    QBrush brush1(QColor(0, 0, 0, 255));
    brush1.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Button, brush1);
    QBrush brush2(QColor(100, 255, 0, 255));
    brush2.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Text, brush2);
    palette.setBrush(QPalette::Active, QPalette::ButtonText, brush2);
    palette.setBrush(QPalette::Active, QPalette::Base, brush1);
    palette.setBrush(QPalette::Active, QPalette::Window, brush1);
    QBrush brush3(QColor(0, 0, 128, 255));
    brush3.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::Highlight, brush3);
    QBrush brush4(QColor(100, 255, 0, 128));
    brush4.setStyle(Qt::SolidPattern);
    palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush4);
    palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
    palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::Text, brush2);
    palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush2);
    palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
    palette.setBrush(QPalette::Inactive, QPalette::Highlight, brush3);
    palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush4);
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush2);
    palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Text, brush2);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush2);
    palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
    palette.setBrush(QPalette::Disabled, QPalette::Highlight, brush3);
    palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush4);
    return palette;
}

QLineEdit *Experiment::getValueDisplayWidget(QWidget *parent)
{
    QLineEdit *ln_ur3;
    ln_ur3 = new QLineEdit(parent);
    ln_ur3->setEnabled(true);
    ln_ur3->setPalette(getValueDisplayPalette());
    QFont font;
    font.setFamily(QString::fromUtf8("Courier New"));
    font.setPointSize(10);
    ln_ur3->setFont(font);
    ln_ur3->setStyleSheet(QString::fromUtf8("background: rgb(0, 0, 0); \n"
                                            "selection-background-color: rgb(0, 0, 128);\n"
                                            "color: rgb(100, 255, 0);"));
    ln_ur3->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
    ln_ur3->setReadOnly(true);
    return ln_ur3;
}
#include <QLabel>
void Experiment::addLabelAndValueDisplay(QWidget *parent, QGridLayout *layout, QString labletext, QString name,  int fromRow, int rowSpan)
{
    QLabel *label = new QLabel(parent);
    label->setText(labletext);
    layout->addWidget(label, fromRow, 0, rowSpan, 1);

    QLineEdit *lineedit = getValueDisplayWidget(parent);
    layout->addWidget(lineedit, fromRow, 1, rowSpan, 1);
    lineedit->setText(name);

    valueFields[name] = lineedit;
}

QPointF Experiment::dataToPlotXY(data_t data)
{
    return QPointF(0,0);
}

void Experiment::displayData(data_t data)
{

}
