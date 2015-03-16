#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "treeitemmodel.h"
#include "algorythm.h"
#include "util.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_treeView_clicked(const QModelIndex &index);

    void on_treeView_doubleClicked(const QModelIndex &index);

    void on_actionRun_triggered();

    void onStateChanged(Algo::AlgorythmStates::AlgorythmState state);

    void onTabCloseRequested(int index);
private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
