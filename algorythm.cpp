#include "algorythm.h"
#include "util.h"
#include <QWidget>
#include <QDebug>

using namespace Algo;


void Algorythm::setState(AlgorythmState state)
{
    this->m_state = state;
    emit stateChanged(state);
}

bool Algorythm::isStopped() const
{
    return m_state == AlgorythmStates::Stopped;
}

bool Algorythm::isRunning() const
{
    return m_state == AlgorythmStates::Running;
}

Algorythm::Algorythm(QObject *parent):QObject(parent),m_state(Stopped),wg(new QWidget())
{

}
Algorythm::~Algorythm()
{
 delete wg;
}
AlgorythmFactory::AlgorythmFactory(){
    tree = new Tree<QVariant>();
    tree->add("");
    mdl = new TreeItemModel(tree);
    load();
}

AlgorythmFactory::~AlgorythmFactory(){
    delete mdl;
    delete tree;
}
bool AlgorythmFactory::deleteAlgorythm(QWidget *wg)
{
    tree->setCurrentItemToBeginning();


    while( tree->current () != 0 && tree->current () != tree->root() )
    {
        TreeItem<QVariant>* ti = tree->current();



        if ( ti->dataCount() == 2 ){
            QVariant vr = ti->data(1);
            Algo::Algorythm *algo = VPtr<Algorythm>::asPtr(vr);

            if ( algo ){

                if ( algo->widget() == wg ){

                    if ( algo->state() == AlgorythmStates::Running )
                        return false;

                    algo->deleteLater();
                    ti->removeAt(1);
                    return true;
                }
            }
        }


        tree->next();
    }


    return false;


}
bool AlgorythmFactory::deleteAlgorythm(TreeItem<QVariant> *orig_ti)
{

    if ( orig_ti->dataCount() == 2 ){
        QVariant vr = orig_ti->data(1);
        Algo::Algorythm *algo = VPtr<Algorythm>::asPtr(vr);
        if ( algo ){
            if ( algo->state() == AlgorythmStates::Running )
                return false;
            algo->deleteLater();
        }
        orig_ti->removeAt(1);

        return true;
    }

    return false;
}

Algorythm *AlgorythmFactory::createAlgorythm(TreeItem<QVariant> *orig_ti)
{

  TreeItem<QVariant> *ti = orig_ti;
  QString id;
  do{
      id = id.isEmpty() ? ti->data(0).toString() : ti->data(0).toString()+"/"+id;
      ti = ti->parent();
   }while( ti != tree->root() );

   Algorythm *algo = create(id.toStdString());
   if ( algo )
   {
        orig_ti->add(VPtr<Algorythm>::asQVariant(algo));
   }

   return algo;

}

void AlgorythmFactory::load(){
    std::vector<std::string> items = AlgorythmFactory::items();
    for( size_t i = 0; i < items.size() ;++i ){
        std::string fi = items.at(i);

        QString group = QString::fromStdString(fi);
        QStringList ls = group.split("/");
        if ( ls.isEmpty())
        {
            qDebug() << QString("Cannot reg: Group=%1").arg(group);
            continue;
        }
        //insert into model's tree
        TreeItem<QVariant> * ti = tree->root();
        foreach(const QString &str , ls){

            int i = 0;
            const int ti_count = ti->childCount();
            for( ; i < ti_count;++i ){
                if ( ti->child(i)->data(0).toString() == str )
                {
                    ti = ti->child(i);
                    break;
                }
            }
            if ( i >= ti_count ){

                ti = ti->appendChild();
                ti->add(str);

            }
        }
    }

}

TreeItemModel *AlgorythmFactory::model(){
    return  mdl;
}
