#ifndef LangBuilder_H
#define LangBuilder_H


#include "algorythms/graphalgorythm.h"


using namespace Algo;
using namespace AdjacencyList;


class LangBuilderGraphModel:public GraphModel{
public:

    LangBuilderGraphModel(): GraphModel(AdjacencyList::Directed){

    }
    virtual bool insertEdge(const QString &srcLabel, const QString &dstLabel, GraphicsView_Edge *edge);
//    bool setEdgeWeight(Graph::Edge *e, QString key, QVariant value);
//    void setNodeWeight(Graph::Node *n, QString key, QVariant value);
};


class LangBuilder: public GraphAlgorythm, public FactoryItem<LangBuilder>
{
    Q_OBJECT

protected:
    virtual void runGraphAlgorythm(Graph<QString,QVariant> ::Node *startNode);

public:
    LangBuilder();
    virtual ~LangBuilder();


};

#endif // LangBuilder_H
