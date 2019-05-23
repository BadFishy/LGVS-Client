QT       += core gui xml network multimedia

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app
TARGET = JohnChess

SOURCES += \
    src/main.cpp \
    src/gameboard.cpp \
    src/create.cpp \
    src/connect.cpp \
    src/Chess.cpp

HEADERS += \
    src/Chess.h \
    src/gameboard.h \
    src/create.h \
    src/connect.h

FORMS += \
    src/Chess.ui \
    src/connect.ui \
    src/create.ui

RESOURCES += \
    resource.qrc
