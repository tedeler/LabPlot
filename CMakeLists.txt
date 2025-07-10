cmake_minimum_required(VERSION 3.16)
project(Labplot LANGUAGES C CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Gui Widgets)

find_library(QWT_LIBRARY NAMES qwt-qt5 qwt REQUIRED)
find_library(LABJACK_LIBRARY NAMES labjackusb REQUIRED)


find_path(QWT_INCLUDE_DIR NAMES qwt.h PATHS "C:/Qwt-6.3.0-dev/include" PATH_SUFFIXES qwt REQUIRED)


if(NOT QWT_LIBRARY OR NOT QWT_INCLUDE_DIR)
    message(FATAL_ERROR "Qwt not found. Please install libqwt-qt5-dev")
endif()

set(Version_MAJOR 1)
set(Version_MINOR 0)

execute_process(
    COMMAND git rev-parse --short=6 HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE Version_GITID
    OUTPUT_STRIP_TRAILING_WHITESPACE
)


# Check Git status (dirty or clean)
execute_process(
    COMMAND git status --porcelain
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_STATUS_OUTPUT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

if(GIT_STATUS_OUTPUT)
    set(Version_GITID "${Version_GITID}-dirty")
endif()


# Get current date (ISO 8601)
string(TIMESTAMP Version_DATESTR "%Y-%m-%dT%H:%M:%S" UTC)

message(STATUS "Git ID: ${Version_GITID}")
message(STATUS "Build Date: ${Version_DATESTR}")


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
    src/testdevice.cpp
    src/testdevice.h
    src/basedevice.cpp
    src/basedevice.h
    src/res/schaltungsbilder.qrc
    src/u6.c
    src/u6.h
    src/labjack.cpp src/labjack.h
)

target_include_directories(Labplot PRIVATE src)

target_link_libraries(Labplot PRIVATE Qt5::Core Qt5::Gui Qt5::Widgets)

target_include_directories(Labplot PRIVATE ${QWT_INCLUDE_DIR})
target_link_libraries(Labplot PRIVATE ${QWT_LIBRARY} ${LABJACK_LIBRARY})

target_compile_definitions(Labplot PRIVATE
    Version_MAJOR=${Version_MAJOR}
    Version_MINOR=${Version_MINOR}
    Version_DATESTR=\"${Version_DATESTR}\"
    Version_GITID=\"${Version_GITID}\"
)
