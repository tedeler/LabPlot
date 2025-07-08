cmake_minimum_required(VERSION 3.16)
project(Labplot LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

add_executable(Labplot
    src/main.cpp
    src/labplotwindow.cpp
    src/labplotwindow.h
    src/labplotwindow.ui
    src/mainwindow.cpp
    src/mainwindow.h
    src/experiment.cpp
    src/experiment.h
    src/expdiodekennlinie.cpp
    src/expdiodekennlinie.h
    src/exptransistorausgangskennlinie.cpp
    src/exptransistorausgangskennlinie.h
    src/labjack.cpp
    src/labjack.h
    src/testdevice.cpp
    src/testdevice.h
    src/basedevice.cpp
    src/basedevice.h
    src/res/schaltungsbilder.qrc
)

target_include_directories(Labplot PRIVATE src)

target_link_libraries(Labplot PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)
