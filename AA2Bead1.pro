#-------------------------------------------------
#
# Project created by QtCreator 2015-02-12T17:49:00
#
#-------------------------------------------------

QT       += core gui printsupport
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Algo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    treeitemmodel.cpp \
    algorythm.cpp \
    algorythms/bucket_sorts/bucketsort.cpp \
    flowlayout.cpp \
    algorythms/bucket_sorts/bucketgraphicsitem.cpp \
    algorythms/hashing/hashing.cpp \
    algorythms/bfs/bfs.cpp \
    graphicsview_edge.cpp \
    graphicsview_node.cpp \
    algorythms/dijktsra/dijkstra.cpp \
    graphscene.cpp \
    algorythms/graphalgorythm.cpp \
    mutable_priority_queue.cpp \
    algorythms/componentssearch/componentssearch.cpp \
    colorizer.cpp

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
    flowlayout.h \
    algorythms/bucket_sorts/bucketgraphicsitem.h \
    algorythms/hashing/hashing.h \
    algorythms/bfs/bfs.h \
    graph.h \
    graphicsview_edge.h \
    graphicsview_node.h \
    defs.h \
    algorythms/bfs/graphscene.h \
    algorythms/dijktsra/dijkstra.h \
    graphscene.h \
    algorythms/graphalgorythm.h \
    mutable_priority_queue.h \
    algorythms/componentssearch/componentssearch.h \
    colorizer.h

FORMS    += mainwindow.ui \
    algorythms/bucket_sorts/bucketsort.ui \
    algorythms/hashing/hashing.ui \
    graph.ui
#debug: QMAKE_CXXFLAGS += -save-temps
release: QMAKE_CXXFLAGS_RELEASE += -O3

windows:{
OTHER_FILES += \
    myapp.rc
RC_FILE = myapp.rc
}

RESOURCES += \
    icons/icons.qrc

