QT       -= core gui
TARGET = client4
CONFIG += console
CONFIG -= app_bundle qt
TEMPLATE = app

INCLUDEPATH += \
   ../../include

LIBS += \
   -lboost_system

SOURCES += \
    ../../tests/socket/client4.cpp
