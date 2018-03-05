#-------------------------------------------------
#
# Project created by QtCreator 2018-03-05T15:02:59
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cefDemo1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mycefview.cpp

HEADERS  += mainwindow.h \
    mycefview.h

FORMS    += mainwindow.ui

LIBS += user32.lib

# lib qcefview
win32: LIBS += -L$$PWD/QCefViewSDK/lib/Release/ -lQCefView

INCLUDEPATH += $$PWD/QCefViewSDK/include
DEPENDPATH += $$PWD/QCefViewSDK/include
