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
    algorythm.cpp \
    algorythms/bucket_sorts/bucketsort.cpp \
    flowlayout.cpp

HEADERS  += mainwindow.h \
    csingleton.h \
    tree.h \
    treeitem.h \
    treeitemmodel.h \
    algorythm.h \
    util.h \
    algorythms/bucket_sorts/bucketsort.h \
    factoryitem.h \
    factorybuilder.h \
    flowlayout.h

FORMS    += mainwindow.ui \
    algorythms/bucket_sorts/bucketsort.ui
QMAKE_CXXFLAGS += -save-temps
release: QMAKE_CXXFLAGS_RELEASE += -O3
