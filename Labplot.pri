include( C:\Qwt-6.3.0-dev\features\qwt.prf )
QWT_DLL_RELEASE = C:/Qwt-6.3.0-dev/lib/qwt.dll
QWT_DLL_DEBUG = C:/Qwt-6.3.0-dev/lib/qwtd.dll
LIBS += -LC:/LabJack/Drivers/64bit -lLabJackUD
INCLUDEPATH += C:\LabJack\Drivers


DEFINES += Version_MAJOR=1
DEFINES += Version_MINOR=0
DEFINES += Version_DATESTR=\\\"$$system( C:\tools\msys64\usr\bin\sh.exe -c \"/usr/bin/date +%d.%m.%Y\")\\\"
DEFINES += Version_GITID=\\\"$$system(git rev-parse --short=6 HEAD)\\\"

QT       += core gui widgets
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	$$PWD\src\basedevice.cpp\
	$$PWD\src\labjack.cpp\
	$$PWD\src\labplotwindow.cpp\
	$$PWD\src\main.cpp\
	$$PWD\src\mainwindow.cpp\
	$$PWD\src\expdiodekennlinie.cpp\
	$$PWD\src\experiment.cpp

HEADERS += \
	$$PWD\src\basedevice.h\
	$$PWD\src\labjack.h\
	$$PWD\src\labplotwindow.h\
	$$PWD\src\mainwindow.h\
	$$PWD\src\LJUD_DynamicLinking.h\
	$$PWD\src\expdiodekennlinie.h\
	$$PWD\src\experiment.h

FORMS    += $$PWD\src\labplotwindow.ui

release:QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$QWT_DLL_RELEASE) $$quote($$OUT_PWD/release/)$$escape_expand(\n\t)
debug:QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$QWT_DLL_DEBUG) $$quote($$OUT_PWD/debug/)$$escape_expand(\n\t)


RESOURCES += \
    $$PWD\src\res\schaltungsbilder.qrc

DISTFILES += \
    $$PWD\src\myapp.rc

RC_FILE = $$PWD\src\myapp.rc
