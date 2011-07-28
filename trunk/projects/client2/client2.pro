QT       -= core gui
TARGET = client2
CONFIG += console
CONFIG -= app_bundle qt
TEMPLATE = app

INCLUDEPATH += \
   ../../include

LIBS += \
   -lboost_system

SOURCES += \
    ../../tests/socket/client2.cpp
