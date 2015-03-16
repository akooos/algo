#ifndef BFS_H
#define BFS_H

#include "algorythms/graphalgorythm.h"

using namespace Algo;
using namespace AdjacencyList;


constant QString key_visited = "visited";
constant QString key_edgetype = "edgetype";

class BFSGraphModel:public GraphModel{


public:

    BFSGraphModel();

    virtual bool insertEdge(const QString &srcLabel,const QString &dstLabel, GraphicsView_Edge* edge);

    virtual void setNodeWeight(Graph<QString,QVariant>::Node *n , QString key, QVariant value);
    virtual void setEdgeWeight(Graph<QString,QVariant>::Edge *e, QString key, QVariant value);



};

class BFS: public GraphAlgorythm, public FactoryItem<BFS>
{
      Q_OBJECT    

protected:
    virtual void runGraphAlgorythm(Graph<QString,QVariant> ::Node *startNode);

public:
    BFS();
    virtual ~BFS();


};

#endif // BFS_H
