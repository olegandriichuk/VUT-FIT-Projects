#-------------------------------------------------
#
# Project created by QtCreator 2024-04-28T13:35:20
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ICP_proj
TEMPLATE = app
CONFIG += -std=c++17


SOURCES += main.cpp\
    controlledrobot.cpp \
        mainwindow.cpp \
    robot.cpp \
    obstacle.cpp \
    simscene.cpp

HEADERS  += mainwindow.h \
    controlledrobot.h \
    robot.h \
    obstacle.h \
    simscene.h

FORMS    += mainwindow.ui

DISTFILES += \
    Game.txt
