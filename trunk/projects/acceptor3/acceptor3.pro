QT       -= core gui
TARGET = acceptor3
CONFIG += console
CONFIG -= app_bundle qt
TEMPLATE = app

INCLUDEPATH += \
   ../../include

LIBS += \
   -lboost_system

SOURCES += \
    ../../tests/acceptor/acceptor3.cpp
