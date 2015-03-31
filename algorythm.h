#ifndef ALGORYTHM_H
#define ALGORYTHM_H




#include "csingleton.h"
#include "treeitemmodel.h"

#include "factorybuilder.h"

class TreeItemModel;
class QWidget;

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
    bool deleteAlgorythm(TreeItem<QVariant> *orig_ti);
    bool deleteAlgorythm(QWidget *wg);
    void load();
    TreeItemModel * model();

};
}

#define Algos Algo::AlgorythmFactory::instance()

#endif // ALGORYTHM_H
