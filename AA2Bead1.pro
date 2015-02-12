#-------------------------------------------------
#
# Project created by QtCreator 2015-02-12T17:49:00
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AA2Bead1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treeitemmodel.cpp \
    algorythm.cpp

HEADERS  += mainwindow.h \
    csingleton.h \
    tree.h \
    treeitem.h \
    treeitemmodel.h \
    algorythm.h \
    util.h

FORMS    += mainwindow.ui
