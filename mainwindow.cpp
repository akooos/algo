#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeView->setModel(Algos->model());
    Utils->moveToCenter(this);
    ui->treeView->expandAll();
    setWindowState(Qt::WindowMaximized);

    bool check = QObject::connect(ui->tabWidget,SIGNAL(tabCloseRequested(int)),this,SLOT(onTabCloseRequested(int)));

    Q_ASSERT(check);

    Q_UNUSED(check);

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit(0);
}

void MainWindow::on_treeView_clicked(const QModelIndex &index)
{
    ui->actionRun->setEnabled(index.isValid());
    ui->actionRun->setIcon(QIcon::fromTheme("media-playback-start",QIcon(":/media-playback-start")));
    ui->actionRun->setText(QObject::tr("Futtatás"));

    if ( !index.isValid())
        return;

    TreeItem<QVariant>* ti = static_cast<TreeItem<QVariant>*>(index.internalPointer());
    Algo::Algorythm *algo = 0;

    ui->actionRun->setEnabled(ti->dataCount() == 2);
    if ( ti->dataCount() == 2 )
    {
        algo = VPtr<Algo::Algorythm>::asPtr( ti->data(1));

        if ( !algo)
            return;

        if ( algo->state() == Algo::AlgorythmStates::Running)
        {
            ui->actionRun->setIcon(QIcon::fromTheme("media-playback-stop",QIcon(":/media-playback-stop")));
            ui->actionRun->setText(QObject::tr("Stop"));
        } else
        {

            ui->actionRun->setIcon(QIcon::fromTheme("media-playback-start",QIcon(":/media-playback-start")));
            ui->actionRun->setText(QObject::tr("Futtatás"));
        }
    }
}

void MainWindow::on_treeView_doubleClicked(const QModelIndex &index)
{

    if ( !index.isValid())
        return;

    TreeItem<QVariant>* ti = static_cast<TreeItem<QVariant>*>(index.internalPointer());
    Algo::Algorythm *alg = 0;
    if ( ti->dataCount() == 2 )
    {
        alg = VPtr<Algo::Algorythm>::asPtr( ti->data(1));

    }else if ( ti->dataCount() == 1 ){
        alg = Algos->createAlgorythm(ti);

        if ( alg ){
            bool check = QObject::connect(alg,SIGNAL(stateChanged(Algo::AlgorythmStates::AlgorythmState)),this,SLOT(onStateChanged(Algo::AlgorythmStates::AlgorythmState)));

            Q_ASSERT(check);

            Q_UNUSED(check);
        }

    }
    if ( alg ){
        ui->actionRun->setEnabled(index.isValid());
        int i = ui->tabWidget->indexOf(alg->widget());
        if ( i > -1 )
        {

            if ( alg->state() == Algo::AlgorythmStates::Running)
            {
                ui->actionRun->setIcon(QIcon::fromTheme("media-playback-stop",QIcon(":/media-playback-stop")));
                ui->actionRun->setText(QObject::tr("Stop"));
            } else
            {
                ui->actionRun->setIcon(QIcon::fromTheme("media-playback-start",QIcon(":/media-playback-start")));
                ui->actionRun->setText(QObject::tr("Futtatás"));
            }
        } else{
            ui->tabWidget->addTab(alg->widget(),ti->data(0).toString());
            ui->actionRun->setIcon(QIcon::fromTheme("media-playback-start",QIcon(":/media-playback-start")));
            ui->actionRun->setText(QObject::tr("Futtatás"));
        }
        ui->tabWidget->setCurrentWidget(alg->widget());
    }
}



void MainWindow::on_actionRun_triggered()
{
    QModelIndexList sells = ui->treeView->selectionModel()->selectedIndexes();
    if ( !sells.isEmpty() )
    {
        QModelIndex ind = sells.first();
        TreeItem<QVariant> *ti = static_cast<TreeItem<QVariant>*>(ind.internalPointer());
        if ( ti->dataCount() == 2)
        {
            Algo::Algorythm* algo = VPtr<Algo::Algorythm>::asPtr( ti->data(1) );
            if ( algo )
            {
                if ( algo->state() == Algo::AlgorythmStates::Running)
                {
                    // ui->actionRun->setIcon(QIcon::fromTheme("media-playback-start"));
                    algo->stop();
                } else
                {
                    // ui->actionRun->setIcon(QIcon::fromTheme("media-playback-stop"));
                    algo->start();


                }

            }
        }
    }
}

void MainWindow::onStateChanged(Algo::AlgorythmStates::AlgorythmState state)
{
    Q_UNUSED(state);

    Algo::Algorythm *algo = static_cast<Algo::Algorythm*>(QObject::sender());
    if ( algo )
    {

        QModelIndexList sells = ui->treeView->selectionModel()->selectedIndexes();
        if ( !sells.isEmpty() )
        {

            QModelIndex ind = sells.first();
            TreeItem<QVariant> *ti = static_cast<TreeItem<QVariant>*>(ind.internalPointer());
            if ( ti->dataCount() == 2)
            {

                Algo::Algorythm* algo_sel = VPtr<Algo::Algorythm>::asPtr( ti->data(1) );
                if ( algo_sel && algo_sel ==algo )
                {

                    if ( algo->state() == Algo::AlgorythmStates::Running)
                    {
                        ui->actionRun->setIcon(QIcon::fromTheme("media-playback-stop",QIcon(":/media-playback-stop")));
                        ui->actionRun->setText(QObject::tr("Stop"));

                    } else
                    {
                        ui->actionRun->setIcon(QIcon::fromTheme("media-playback-start",QIcon(":/media-playback-start")));
                        ui->actionRun->setText(QObject::tr("Futtatás"));
                    }

                }
            }
        }

    }
}

void MainWindow::onTabCloseRequested(int index)
{


  Q_UNUSED(index)

  QModelIndexList sells = ui->treeView->selectionModel()->selectedIndexes();
  if ( !sells.isEmpty() )
  {
      QModelIndex ind = sells.first();
      TreeItem<QVariant> *ti = static_cast<TreeItem<QVariant>*>(ind.internalPointer());
      Algos->deleteAlgorythm(ti);
  }

}
