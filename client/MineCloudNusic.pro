#-------------------------------------------------
#
# Project created by QtCreator 2020-07-25T16:07:58
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MineCloudNusic
TEMPLATE = app
RC_ICONS = ./images/logo.ico

include(./netapi/netapi.pri)
INCLUDEPATH +=./netapi/

SOURCES += main.cpp\
        musicwidget.cpp \
    playlistform.cpp \
    form.cpp \
    chatdlg.cpp

HEADERS  += musicwidget.h \
    playlistform.h \
    form.h \
    chatdlg.h

FORMS    += musicwidget.ui \
    playlistform.ui \
    form.ui \
    chatdlg.ui

RESOURCES += \
    images.qrc
