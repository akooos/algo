
#include <QApplication>
#include <QMetaType>
#include "mainwindow.h"
#include <vector>
#include <iostream>
#include "algorythms/bucket_sorts/bucketsort.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
