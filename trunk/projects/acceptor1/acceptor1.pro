QT       -= core gui
TARGET = acceptor1
CONFIG += console
CONFIG -= app_bundle qt
TEMPLATE = app

INCLUDEPATH += \
   ../../include

LIBS += \
   -lboost_system

SOURCES += \
    ../../tests/acceptor/acceptor1.cpp
