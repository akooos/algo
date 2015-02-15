#include "algorythm.h"


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
        TreeItem<QVariant> * ti = tree;
        foreach(const QString &str , ls){
            int i = 0;
            for( ; i < ti->childCount();++i ){
                if ( ti->child(i)->data(0).toString() == str )
                {
                    ti = ti->child(i);
                    continue;
                }
            }
            if ( i >= ti->childCount()){
                ti = ti->appendChild();
                ti->add(str);
            }

        }
    }

}

TreeItemModel *AlgorythmFactory::model(){
    return  mdl;
}
