#ifndef GRAPHALGORYTHM_H
#define GRAPHALGORYTHM_H


#include "algorythm.h"
#include "graph.h"

#include "graphscene.h"
#include "ui_graph.h"

class GraphicsScene;

namespace Ui{
  class GraphWidget;
}


using namespace Algo;
using namespace AdjacencyList;

class GraphModel:public Graph<QString,QVariant>, public IModelCtrl{

    unsigned int cntr;
    std::string last_label;

public:

    GraphModel(GraphType graph_type = AdjacencyList::Undirected);
    virtual bool insertNode(QString &label, GraphicsView_Node* node);
    virtual bool deleteNode(const QString &label);
    virtual bool insertEdge(const QString &srcLabel,const QString &dstLabel, GraphicsView_Edge* edge);
    virtual bool deleteEdge(const QString &srcLabel,const QString &dstLabel);
    virtual void setNodeWeight(Graph<QString,QVariant>::Node *n , QString key, QVariant value);
    virtual bool setEdgeWeight(Graph<QString,QVariant>::Edge *e, QString key, QVariant value);
    virtual bool setEdgeWeight(Graph<QString,QVariant>::Node *src,Graph<QString,QVariant>::Node *dest, QString key, QVariant value);
    virtual bool setEdgeWeight(const QString &srcLabel,const QString &dstLabel, QString key, QVariant value);
    virtual bool isDirected() const;
};


class GraphAlgorythm: public Algo::Algorythm
{
     Q_OBJECT

    GraphEdge *cur_e ;
protected:
    Ui::GraphWidget *ui;
    GraphModel *  gm;
    GraphScene *scene;

    void printLine(const QString &line);
    virtual void runGraphAlgorythm(Graph<QString,QVariant> ::Node *startNode) = 0;
public:
    GraphAlgorythm(GraphModel *gm);
    virtual ~GraphAlgorythm();

private slots:
    void onActionTriggered(bool checked);
    void onSceneSelectionChanged();
    void onTextChanged(const QString & text);
public slots:

    virtual void start();
    virtual void stop();
};

#endif // GRAPHALGORYTHM_H
