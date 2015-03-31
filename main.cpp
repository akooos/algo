
#include <QApplication>
#include "mainwindow.h"
#include "mutable_priority_queue.h"

#include <iostream>

#include "colorizer.h"

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int rc = a.exec();
    Algos->deleteRegs();
    return rc;
}
