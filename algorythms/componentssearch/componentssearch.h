#ifndef COMPONENTSSEARCH_H
#define COMPONENTSSEARCH_H



#include "algorythms/graphalgorythm.h"
#include <stack>
#include <QVariant>

using namespace Algo;
using namespace AdjacencyList;


struct ComponentsSearchGraphModel:public GraphModel{


    ComponentsSearchGraphModel(): GraphModel(AdjacencyList::Directed){}
    virtual bool insertEdge(const QString &srcLabel, const QString &dstLabel, GraphicsView_Edge *edge);
    virtual bool setEdgeWeight(Graph::Edge *e, QString key, QVariant value);
    virtual void setNodeWeight(Graph::Node *n, QString key, QVariant value);
};

/*
Erősen összefüggő komponensek megtalálása
 -ben , majd fordítottjában is bejárás, minden fát a még bejáratlan pontok
közül a legnagyobb befejezési számúval kezdünk (az előző bejárásból maradt ).*/
class ComponentsSearch: public  GraphAlgorythm, public FactoryItem<ComponentsSearch>
{
    Q_OBJECT

    QList<QGraphicsTextItem*> ti_classes;


    bool isNodeVisited(GraphNode *n) const
    {
        return n->value("visited").toBool();
    }
    void dfs(Graph<QString,QVariant> *g, GraphNode *node, std::stack<QVariant> &traversal);
    std::stack<QVariant> create_dfs_nodes_label();

protected:
    virtual void runGraphAlgorythm(GraphNode *startNode);

public:
    ComponentsSearch();
    virtual ~ComponentsSearch();


};
#endif // COMPONENTSSEARCH_H
