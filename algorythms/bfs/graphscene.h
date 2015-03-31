#ifndef GRAPHSCENE_H
#define GRAPHSCENE_H

#include <QGraphicsScene>
class QGraphicsSceneMouseEvent;
class QKeyEvent;

class GraphicsView_Node;
class GraphicsView_Edge;

#include "graph.h"
using namespace AdjacencyList;
struct IModelCtrl{
    virtual bool insertNode(QString &label, GraphicsView_Node* node) = 0;
    virtual bool deleteNode(const QString &label) = 0;
    virtual bool insertEdge(const QString &srcLabel,const QString &dstLabel, GraphicsView_Edge* edge) = 0;
    virtual bool deleteEdge(const QString &srcLabel,const QString &dstLabel) = 0;

    virtual void setNodeWeight(Graph<QString,QVariant>::Node *n , QString key, QVariant value) = 0;
    virtual void setEdgeWeight(Graph<QString,QVariant>::Node *src,Graph<QString,QVariant>::Node *dest, QString key, QVariant value) = 0;
  /*  virtual QColor edgeColor(const QString &srcLabel,const QString &dstLabel) = 0;
    virtual QColor nodeColor(const QString &label) = 0;*/

};


class GraphScene: public QGraphicsScene
{
    Q_OBJECT
public:
    enum GraphMode {InsNode,DelNode,InsEdge,DelEdge,NoAction};

    GraphScene(IModelCtrl *_mintf, QObject * parent = 0);
    virtual ~GraphScene();

    void setMode(GraphMode mode);

    IModelCtrl * modelCtrl(){
        return mintf;
    }

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
    virtual void keyReleaseEvent(QKeyEvent * keyEvent) Q_DECL_OVERRIDE;

private:
    GraphMode  mode;
    IModelCtrl *mintf;
    bool drawArrow;

    //összekötendő

    GraphicsView_Node * srcNode;
    GraphicsView_Node * dstNode;

};


#endif // GRAPHSCENE_H
