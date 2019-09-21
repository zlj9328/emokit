#-------------------------------------------------
#
# Project created by QtCreator 2015-05-15T17:45:56
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = UIFrameWork
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    titlewidget.cpp \
    QCustomPlot/qcustomplot.cpp \
    QLog/qlog.cpp \
    IcoHelper/iconhelper.cpp \
    msgbox.cpp \
    QExtSerialPort/posix_qextserialport.cpp \
    QExtSerialPort/qextserialbase.cpp \
    QExtSerialPort/qextserialport.cpp \
    SerialPort/qserialporthelper.cpp \
    Emotiv/emotiv.cpp \
    Emotiv/processemotivdata.cpp \
    Emotiv/emotivprocessthread.cpp \
    UDPSocket/UDPSocket.cpp

HEADERS  += mainwindow.h \
    titlewidget.h \
    QCustomPlot/qcustomplot.h \
    QLog/qlog.h \
    IcoHelper/iconhelper.h \
    msgbox.h \
    QExtSerialPort/posix_qextserialport.h \
    QExtSerialPort/qextserialbase.h \
    QExtSerialPort/qextserialport.h \
    SerialPort/qserialporthelper.h \
    Emotiv/emotiv.h \
    Emotiv/processemotivdata.h \
    Emotiv/emotivprocessthread.h \
    UDPSocket/UDPSocket.h

LIBS += /usr/lib/libmcrypt.so \
        /usr/local/lib/libhidapi-hidraw.so \
        /usr/local/fftw3/lib/libfftw3.so


FORMS    += \
    titlewidget.ui \
    mainwindow.ui \
    msgbox.ui

RESOURCES += \
    res.qrc
