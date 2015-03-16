#ifndef DIJKSTRA_H
#define DIJKSTRA_H


#include "algorythms/graphalgorythm.h"


using namespace Algo;
using namespace AdjacencyList;


class DijkstraGraphModel:public GraphModel{





public:

    DijkstraGraphModel(): GraphModel(){

    }

};


class Dijkstra: public  GraphAlgorythm, public FactoryItem<Dijkstra>
{
    Q_OBJECT
protected:
    virtual void runGraphAlgorythm(Graph<QString,QVariant> ::Node *startNode);
public:
    Dijkstra();
    virtual ~Dijkstra();


};

#endif // DIJKSTRA_H
