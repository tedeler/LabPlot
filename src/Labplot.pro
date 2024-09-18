include( C:\Qwt-6.3.0-dev\features\qwt.prf )
QWT_DLL_RELEASE = C:/Qwt-6.3.0-dev/lib/qwt.dll
QWT_DLL_DEBUG = C:/Qwt-6.3.0-dev/lib/qwtd.dll
LIBS += -LC:/LabJack/Drivers/64bit -lLabJackUD
INCLUDEPATH += C:\LabJack\Drivers


DEFINES += Version_MAJOR=1
DEFINES += Version_MINOR=0
DEFINES += Version_DATESTR=\\\"$$system(sh -c \"date +%d.%m.%Y\")\\\"
DEFINES += Version_GITID=\\\"$$system(git rev-parse --short=6 HEAD)\\\"

QT       += core gui widgets
CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
	basedevice.cpp\
	expdiodekennlinie.cpp \
	experiment.cpp \
	labjack.cpp\
	labplotwindow.cpp\
	main.cpp\
	mainwindow.cpp

HEADERS += \
	basedevice.h\
	expdiodekennlinie.h \
	experiment.h \
	labjack.h\
	labplotwindow.h\
	mainwindow.h\
	LJUD_DynamicLinking.h

FORMS    += labplotwindow.ui

#release:QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$QWT_DLL_RELEASE) $$quote($$OUT_PWD/release/) ;


RESOURCES += \
    res/schaltungsbilder.qrc

DISTFILES += \
    myapp.rc

RC_FILE = myapp.rc
