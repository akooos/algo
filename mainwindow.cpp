#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mdl = new TreeItemModel();
    ui->treeView->setModel(mdl);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete mdl;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit(0);
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    ui->actionRun->setEnabled(index.isValid());
}

void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{
    ui->actionRun->setEnabled(index.isValid());
    ui->actionRun->trigger();
}
