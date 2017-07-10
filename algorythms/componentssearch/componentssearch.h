#ifndef COMPONENTSSEARCH_H
#define COMPONENTSSEARCH_H



#include "algorythms/graphalgorythm.h"
#include <stack>
#include <queue>
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


    struct GraphNodeComperator{
        bool operator() (GraphNode *lhs, GraphNode* rhs){
            bool ok = false;
            return lhs->value("bsz").toInt(&ok) <= rhs->value("bsz").toInt(&ok);
        }
    };

    typedef std::priority_queue< GraphNode* , std::vector<GraphNode* >,GraphNodeComperator  > PQ_DFS;
    QList<QGraphicsTextItem*> ti_classes;

    inline bool isNodeIsolated(GraphNode *n) const{
        return n->value("isolated").toBool() ;
    }
    inline bool isNodeVisited(GraphNode *n) const
    {
        return n->value("visited").toBool();
    }
    void dfs(Graph<QString,QVariant> *g, GraphNode *node, PQ_DFS &traversal, int &bsz);
    PQ_DFS create_dfs_nodes_label();


    void addTextItem(const QString &txt, QColor clr, QPointF &tipos);
protected:
    virtual void runGraphAlgorythm(GraphNode *startNode);

public:
    ComponentsSearch();
    virtual ~ComponentsSearch();


};
#endif // COMPONENTSSEARCH_H
