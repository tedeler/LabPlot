#include "labplotwindow.h"
#include <QApplication>
#include <QDebug>

#include <cstdarg>
#include <cstdio>


#ifdef __cplusplus
extern "C" {
#endif

void debugPrintf(const char* format, ...);

#ifdef __cplusplus
}
#endif


void debugPrintf(const char* format, ...)
{
    char buffer[1024];  // Ausreichend großer Puffer für Debug-Ausgabe
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);

    qDebug().noquote() << buffer;
}

int main(int argc, char *argv[])
{
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication a(argc, argv);
    LabPlotWindow w;
    w.show();

    int returncode = a.exec();

    return returncode;
}
