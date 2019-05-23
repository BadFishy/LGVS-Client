QT       += core gui xml network multimedia

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = Chess

SOURCES += \
    src/main.cpp \
    src/gameboard.cpp \
    src/Chess.cpp

HEADERS += \
    src/Chess.h \
    src/gameboard.h

FORMS += \
    src/Chess.ui

RESOURCES += \
    resource.qrc
