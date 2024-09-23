#include "labplotwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication a(argc, argv);
    LabPlotWindow w;
    w.show();

    int returncode = a.exec();

    return returncode;
}
