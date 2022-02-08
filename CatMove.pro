#-------------------------------------------------
#
# Project created by QtCreator 2022-01-27T16:26:07
#
#-------------------------------------------------

QT       += core gui concurrent serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CatMove
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        Src/Gui/CatMove.cpp \
        Src/Pub/SlamtecRadar.cpp \
        Src/SerialPort/PortReadThread.cpp \
        Src/SerialPort/PortWriteThread.cpp \
        Src/SerialPort/SerialPort.cpp \
        Src/SerialPort/SerialPortRun.cpp \
        Src/Pub/Public_Function.cpp

HEADERS += \
        Src/Gui/CatMove.h \
        Src/Pub/SlamtecRadar.h \
        Src/SerialPort/PortReadThread.h \
        Src/SerialPort/PortWriteThread.h \
        Src/SerialPort/SerialPort.h \
        Src/SerialPort/SerialPortRun.h \
        Src/Pub/Public_Function.h

include(Src/Pub/QZXing/source/QZXing.pri)

INCLUDEPATH += ./Inc/include \
               ./Inc/src

LIBS += -L$$PWD/Lib/Debug/ -lrplidar_sdk
#Release: LIBS += -L $$PWD/Lib/Release -Lrplidar_sdk
#Debug: LIBS += -L $$PWD/Lib/Debug -lrplidar_sdk

#debug#LIBS += -L $$PWD/Lib -lsl_lidar_sdk

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
