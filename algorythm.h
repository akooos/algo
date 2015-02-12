#ifndef ALGORYTHM_H
#define ALGORYTHM_H


#include <QString>
#include <QList>

#include "csingleton.h"
#include "tree.h"

namespace Algo{

class Algorythm
{
     QString name;
public:
    Algorythm();
    ~Algorythm();

    const QString & operator=(const Algo::Algorythm &other);
};


class AlgorythmFactory:public CSingleton<AlgorythmFactory>{

  QList<Algo::Algorythm*> algos;


  void reg( Algo::Algorythm &other){

      algos.append(&other);
  }


};

#define Algos AlgorythmFactory::instance()

}

#endif // ALGORYTHM_H
