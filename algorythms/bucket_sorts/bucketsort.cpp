#include "bucketsort.h"
#include "ui_bucketsort.h"

RegisterFactoryItem(BucketSort,AlgorythmFactory,"Edény rendezések/Edény rendezés")

BucketSort::BucketSort():Algorythm(),ui(new Ui::BucketSortWidget)
{
    ui->setupUi(wg);
    onPbtnExampleClicked();

    bool check = QObject::connect(ui->pbPeldak,SIGNAL(clicked()),this,SLOT(onPbtnExampleClicked()));

    Q_ASSERT(check);

    Q_UNUSED(check);

    lb = new QLabel(ui->scrollArea);

    lb->setText("*");
    QFont fnt = lb->font();
    fnt.setBold(true);
    fnt.setPixelSize(24);
    lb->setFont(fnt);
}

BucketSort::~BucketSort()
{
    delete ui;
}

void BucketSort::start()
{
    if ( m_state == AlgorythmStates::Running)
        return;


    setState(AlgorythmStates::Running);

    qDebug() << "BucketSort Start";
    animation = new QPropertyAnimation(lb, "geometry");
    animation->setDuration(10000);
    animation->setStartValue(QRect(0, 0, 100, 30));
    animation->setEndValue(QRect(250, 250, 100, 30));

    Utils->startAndWaitForAnimation(animation);

    if ( isStopped() ){
        qDebug() << "BucketSort stopped...";
        return;
     }

    animation = 0;

    qDebug() << "BucketSort finished";
    setState(AlgorythmStates::Stopped);
}

void BucketSort::stop()
{
    if ( m_state != AlgorythmStates::Running )
        return;

    if ( animation ){
        animation->stop();
    }

    setState(AlgorythmStates::Stopped);
}

void BucketSort::onPbtnExampleClicked()
{

    ui->plainTextEdit->clear();

    ui->plainTextEdit->appendPlainText("Nagy Attila");
    ui->plainTextEdit->appendPlainText("Morvai Mária-Júlia");
    ui->plainTextEdit->appendPlainText("Berezvai Dániel");
    ui->plainTextEdit->appendPlainText("Molnár Gábor");
    ui->plainTextEdit->appendPlainText("Szabó Dániel");
    ui->plainTextEdit->appendPlainText("Szabó Gábor");
    ui->plainTextEdit->appendPlainText("Csoór István");
    ui->plainTextEdit->appendPlainText("Hordós Bence");
    ui->plainTextEdit->appendPlainText("Székely Ádám");
    ui->plainTextEdit->appendPlainText("Balázs András");
    ui->plainTextEdit->appendPlainText("Szabó Gábor");
    ui->plainTextEdit->appendPlainText("Balogh Tamás");
    ui->plainTextEdit->appendPlainText("Laposa Sándor");
    ui->plainTextEdit->appendPlainText("Benda Ferenc");
    ui->plainTextEdit->appendPlainText("Farkas Gábor Gergely");
    ui->plainTextEdit->appendPlainText("Felbab Vukasin");
    ui->plainTextEdit->appendPlainText("GergelyGyóni Dorottya");
}




