

#include <QVector>
#include <QLabel>
#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QGraphicsTextItem>
#include <QParallelAnimationGroup>

#include "bucketsort.h"
#include "ui_bucketsort.h"
#include "util.h"

static const int LEFT_PAD =10;
static const int TOP_PAD = 70;
static const int  SPACE = 10;


RegisterFactoryItem(BucketSort,AlgorythmFactory,"Edény rendezések/Edény rendezés")

BucketSort::BucketSort():Algorythm(),ui(new Ui::BucketSortWidget)
{
    ui->setupUi(wg);

    size = QSizeF(130.0,170.0);

    scene = new QGraphicsScene(this);

    statustext = new QGraphicsTextItem();

    scene->addItem(statustext);

    ui->graphicsView->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    ui->graphicsView->setScene(scene);

    onPbtnExampleClicked();

    bool check = QObject::connect(ui->pbPeldak,SIGNAL(clicked()),this,SLOT(onPbtnExampleClicked()));

    Q_ASSERT(check);

    Q_UNUSED(check);

    statustext->setPos(QPointF(10,10));
}
BucketSort::~BucketSort()
{
    cleanUp();
    delete ui;
   /* delete statustext;
    delete scene;*/

}

void BucketSort::createNewNameLabel(const QString &name, int bucketIndex)
{

    BucketGraphicsItem *bgi = ui_buckets.at(bucketIndex).first;

    int elementIndex = 0;
    QGraphicsTextItem *textgi = new QGraphicsTextItem();
    QFont f = textgi->font();
    f.setBold(true);
    f.setPixelSize(14);
    textgi->setPos(QPointF(0, 55));
    textgi->setFont(f);
    textgi->setTextWidth(size.width()-3);
    textgi->setDefaultTextColor(Qt::black);
    textgi->setPlainText(name);
    scene->addItem(textgi);

    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    //meghatározom hogy hová kell beszúrni
    for(; elementIndex < buckets[bucketIndex].second.size() ; ++elementIndex){
        if (  buckets[bucketIndex].second.at(elementIndex).localeAwareCompare(name) > 0 )
            break;
    }

    //uj cimke y pozicioja
    qreal posy;
    const int prev_index = elementIndex-1;
    qreal lastLabelPosy;

    if ( prev_index > -1){
        //posy meghatározása, az előző címke után..
        QGraphicsTextItem *prev_tgi = ui_buckets.at(bucketIndex).second.at(prev_index);
        posy = prev_tgi->pos().y() + prev_tgi->boundingRect().height()-2;
    } else
        //alapértelmezett posy érték, ha az első név az edényben
        posy = bgi->pos().y()+20;


    //beszúrás a listába
    if ( elementIndex < buckets[bucketIndex].second.size() ) {


        //Meg kell-e növelni az edény méretét?...
        QFontMetrics fm(textgi->font());
        qreal estimated_height = textgi->boundingRect().height();   //qRound( ( fm.width(name) / ( size.width() -3 )  ) * (fm.height() ) );
        QGraphicsTextItem *last_tgi  = ui_buckets.at(bucketIndex).second.last();
        lastLabelPosy = last_tgi->pos().y()+last_tgi->boundingRect().height()-2;

        //"edény" méretét meg kell növelni ha szükséges, estimated height kiszamolhato mennyi lesz a szöveg nagysága
        if ( ( bgi->pos().y() + bgi->size().height() ) < (lastLabelPosy + estimated_height) ){
            QSizeF s_end  = bgi->size();
            s_end.setHeight(s_end.height()+estimated_height*2);
            animation = new QPropertyAnimation(bgi, "size");
            animation->setDuration(500);
            animation->setEasingCurve(QEasingCurve::OutCirc);
            animation->setStartValue(bgi->size());
            animation->setEndValue(s_end);
            Utils->startAndWaitForAnimation(animation);
            animation = 0;
        }

        //utána lévő elemek lejjebb tolása
        pushLabelOnePlace(bucketIndex,elementIndex,estimated_height);

        buckets[bucketIndex].second.insert(elementIndex,name);
        ui_buckets[bucketIndex].second.insert(elementIndex,textgi);

    } else
          {
        buckets[bucketIndex].second.append(name);
        ui_buckets[bucketIndex].second.append(textgi);
    }

    animation = new QPropertyAnimation(textgi, "pos");
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::OutCirc);
    animation->setStartValue(QPointF(0, 200));
    animation->setEndValue(QPointF(LEFT_PAD  + ( ( size.width() + SPACE ) * (bucketIndex) ) , posy ));

    Utils->startAndWaitForAnimation(animation);
    animation = 0;
}

void BucketSort::createNewBucket(const QString &name)
{
    QString letter = name.at(0);
    QPair<QString,QList<QString> > new_bucket;
    int i = 0;
    for(; i < buckets.size();++i){
        if (buckets.at(i).first.localeAwareCompare(letter) > 0)
        {
             //  qDebug()  << letter << " > " << buckets.at(i).first << QString("(%1,%2)").arg(name).arg(i);
            break;

        }
    }
    new_bucket.first = letter;

    BucketGraphicsItem *bgi = new BucketGraphicsItem(size);
    bgi->setTitle(letter);

    bgi->setPos(QPointF(100, 55));

    scene->addItem(bgi);
    animation = new QPropertyAnimation(bgi, "pos");
    animation->setDuration(500);
    animation->setEasingCurve(QEasingCurve::OutCirc);
    animation->setStartValue(QPointF(0, 55));
    animation->setEndValue(QPointF(LEFT_PAD + ( ( size.width() + SPACE ) * (i) ) ,size.height()+ TOP_PAD));

    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    QPair<BucketGraphicsItem *,QList<QGraphicsTextItem*> > item;
    item.first = bgi;

    if (i < buckets.size())
    {
        buckets.insert(i,new_bucket);
        ui_buckets.insert(i,item);
        pushBucketOnePlace(i+1);
        Utils->startAndWaitForAnimation(animation);
        createNewNameLabel(name,i) ;
    } else{
        buckets.append(new_bucket);
        ui_buckets.append(item);
        Utils->startAndWaitForAnimation(animation);
        createNewNameLabel(name,ui_buckets.size()-1) ;
    }

}

void BucketSort::pushBucketOnePlace(int from, QAbstractAnimation* plus_anim  )
{
    Q_UNUSED(plus_anim);

    QParallelAnimationGroup *panim = new QParallelAnimationGroup;

    QPropertyAnimation *anim = 0;
    for ( int j = ui_buckets.size()-1; j >=from;--j)
    {

        BucketGraphicsItem *bgi = ui_buckets.at(j).first;

        for ( int i = 0; i < ui_buckets.at(j).second.size();++i)
        {
            QGraphicsTextItem *ti = ui_buckets.at(j).second.at(i);
            anim = new QPropertyAnimation(ti, "pos");
            anim->setDuration(500);
            anim->setEasingCurve(QEasingCurve::OutCirc);
            qreal posy = ti->pos().y();
            anim->setStartValue(ti->pos());
            anim->setEndValue(QPointF(LEFT_PAD + ( ( size.width() + SPACE ) * (j) ) , posy));
            panim->addAnimation(anim);
        }

        anim = new QPropertyAnimation(bgi, "pos");
        anim->setDuration(500);
        anim->setEasingCurve(QEasingCurve::OutCirc);
        anim->setStartValue(bgi->pos());
        anim->setEndValue(QPointF(LEFT_PAD + ( ( size.width() + SPACE ) * (j) ) ,  TOP_PAD+size.height()));
        panim->addAnimation(anim);

    }

//    if ( plus_anim )
 //       panim->addAnimation(plus_anim);


    Utils->startAndWaitForAnimation(panim);

}

void BucketSort::pushLabelOnePlace(int bucketIndex,int from, qreal push_height)
{
    QParallelAnimationGroup *panim = new QParallelAnimationGroup;

    QPropertyAnimation *anim = 0;

  //  BucketGraphicsItem *bgi = ui_buckets.at(bucketIndex).first;

    for ( int i = ui_buckets.at(bucketIndex).second.size()-1; i >= from ;--i)
    {
            QGraphicsTextItem *ti = ui_buckets.at(bucketIndex).second.at(i);
            anim = new QPropertyAnimation(ti, "pos");
            anim->setDuration(500);
            anim->setEasingCurve(QEasingCurve::OutCirc);
            qreal posx = ti->pos().x();
            qreal posy = ti->pos().y()+push_height;
            anim->setStartValue(ti->pos());
            anim->setEndValue(QPointF(posx, posy));
            panim->addAnimation(anim);
    }
    if ( anim )
        Utils->startAndWaitForAnimation(panim);

}

void BucketSort::cleanUp()
{
    while(!ui_buckets.isEmpty()){
        QPair<BucketGraphicsItem*,QList<QGraphicsTextItem*> > item = ui_buckets.takeFirst();
        delete item.first;
        qDeleteAll(item.second);
        item.second.clear();
    }
    ui->pteResult->clear();

    buckets.clear();
}

void BucketSort::start()
{
    if ( m_state == AlgorythmStates::Running)
        return;

    setState(AlgorythmStates::Running);

    cleanUp();



    // hash into buckets
    QStringList ls = ui->plainTextEdit->toPlainText().split(QRegExp("[,;\r\n]"));

    foreach(const QString &str, ls){

        if ( str.isEmpty())
            continue;

        QString letter = str.at(0);

        int i = 0;
        for( ; i < buckets.size();++i)
            if ( buckets.at(i).first.startsWith(letter,Qt::CaseInsensitive) )
             break;

        if ( i < buckets.size() ){
            //létezik már ilyen edény..
            QFont f = statustext->font();
            f.setBold(true);
            f.setPointSize(18);
            statustext->setDefaultTextColor(Qt::blue);
            statustext->setFont(f);
            statustext->setPlainText(QString("\"%1\" mozgatás edénybe \"%2\"").arg(str,letter));

            createNewNameLabel(str,i);
            if ( isStopped() ){

                return;
             }

        }else{
            //létre kell hozni az edényt
            QFont f = statustext->font();
            f.setBold(true);
            f.setPointSize(18);
            statustext->setDefaultTextColor(Qt::blue);
            statustext->setFont(f);
            statustext->setPlainText(QString("Új edény \"%1\", mert még nincs hozzá \"%2\"").arg(letter,str));

            createNewBucket(str);
            if ( isStopped() ){

                return;
             }
        }
    }

    statustext->setPlainText(QString("Összefűzés. Kész."));


    for(int i = 0; i < buckets.size();++i){
        for ( int j = 0; j < buckets.at(i).second.size();++j){
            ui->pteResult->appendPlainText(buckets.at(i).second.at(j));

        }
    }


    setState(AlgorythmStates::Stopped);
}

void BucketSort::stop()
{
    if ( m_state != AlgorythmStates::Running )    
        return;

    setState(AlgorythmStates::Stopped);

    /*if ( animation ){
        animation->stop();
        animation = 0;

    }*/
    statustext->setPlainText("");    
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
    ui->plainTextEdit->appendPlainText("Gyóni Dorottya");
}




