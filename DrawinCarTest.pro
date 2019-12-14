#-------------------------------------------------
#
# Project created by QtCreator 2019-12-13T22:08:45
#
#-------------------------------------------------

CONFIG += c++11
QMAKE_CXXFLAGS_CXX11    = -std=c++1y

QT       += testlib

TARGET = tst_drawincartest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_drawincartest.cpp \
    RobotState.cpp \
    RobotStateHistory.cpp

DEFINES += SRCDIR=\\\"$$PWD/\\\"

HEADERS += \
    RobotState.h \
    RobotStateHistory.h
