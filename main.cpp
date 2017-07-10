
#include <QApplication>
#include "mainwindow.h"
#include "mutable_priority_queue.h"

#include <iostream>
#include <set>

#include "colorizer.h"
#include <typeinfo>



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    int rc = a.exec();
    Algos->deleteRegs();
    return rc;

      return 0;
}
