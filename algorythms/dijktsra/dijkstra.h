#ifndef DIJKSTRA_H
#define DIJKSTRA_H


#include "algorythms/graphalgorythm.h"


using namespace Algo;
using namespace AdjacencyList;


class DijkstraGraphModel:public GraphModel{
public:

    DijkstraGraphModel(): GraphModel(AdjacencyList::Directed){

    }
    virtual bool insertEdge(const QString &srcLabel, const QString &dstLabel, GraphicsView_Edge *edge);
    bool setEdgeWeight(Graph::Edge *e, QString key, QVariant value);


    void setNodeWeight(Graph::Node *n, QString key, QVariant value);
};


class Dijkstra: public  GraphAlgorythm, public FactoryItem<Dijkstra>
{
    Q_OBJECT

    int getKoltseg(Graph<QString,QVariant> ::Edge *e) const;

protected:
    virtual void runGraphAlgorythm(Graph<QString,QVariant> ::Node *startNode);

public:
    Dijkstra();
    virtual ~Dijkstra();


};
class GraphNodeComparetor{

public:
    GraphNodeComparetor(const std::map<GraphNode*,int> &dist):dist(dist){}

    bool operator()( GraphNode * lhs, GraphNode* rhs){
        return dist.at(lhs) > dist.at(rhs);
    }
private:
    const std::map<GraphNode*,int> &dist;
};

#endif // DIJKSTRA_H
