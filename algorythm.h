#ifndef ALGORYTHM_H
#define ALGORYTHM_H


#include <QString>
#include <QList>
#include <QWidget>

#include "csingleton.h"
#include "treeitemmodel.h"
#include "util.h"
#include "factorybuilder.h"

namespace  Algo{

namespace AlgorythmStates{
enum AlgorythmState{
    Stopped, Running
};
}
using namespace  AlgorythmStates;
class Algorythm : public QObject
{
 Q_OBJECT
protected:
    AlgorythmState m_state;
    QWidget *wg;

    void setState(AlgorythmStates::AlgorythmState state);


public:
    explicit Algorythm(QObject *parent= 0);
    virtual ~Algorythm();

    QWidget * widget(){
        return wg;
    }
    AlgorythmState state() const{
        return m_state;
    }

    bool isStopped() const;
    bool isRunning() const;

public slots:
    virtual void start() = 0;
    virtual void stop() = 0;
signals:
    void stateChanged(Algo::AlgorythmStates::AlgorythmState state);
};

class AlgorythmFactory :public CSingleton<AlgorythmFactory>, public FactoryBuilder<Algorythm,std::string>{

    friend class CSingleton<AlgorythmFactory>;
    Tree<QVariant> *tree;
    TreeItemModel *mdl;

private:

    AlgorythmFactory();
    virtual ~AlgorythmFactory();

public:
    Algorythm * createAlgorythm(TreeItem<QVariant> *orig_ti);
    void load();
    TreeItemModel * model();
};
}

#define Algos Algo::AlgorythmFactory::instance()

#endif // ALGORYTHM_H

/*
       tree->setCurrentItemToBeginning();

       while ( tree->current() && tree->current() != tree->root() ){
         if ( tree->current()->dataCount() == 2 && VPtr<Algo::Algorythm>::asPtr(tree->current()->data(1)) )
             return;
         tree->next();
       }
 */
