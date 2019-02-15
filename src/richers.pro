#-------------------------------------------------
#
# Project created by QtCreator 2017-01-03T17:58:46
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = richers
TEMPLATE = app


SOURCES += main.cpp\
        richers.cpp

HEADERS  += richers.h

FORMS    += richers.ui

QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2
QMAKE_CXXFLAGS_RELEASE += -std=c++11 -Wall -Wextra -pedantic -O3

RESOURCES += \
    resources.qrc
