#-------------------------------------------------
#
# Project created by QtCreator 2016-07-23T01:50:42
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DetectarTexto-OpenCV
TEMPLATE = app

INCLUDEPATH += /usr/local/include/opencv-3.1.0
LIBS += `pkg-config opencv --libs --cflags` -lopencv_text

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
