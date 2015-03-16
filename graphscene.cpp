
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include "graphscene.h"
#include "graphicsview_node.h"
#include "graphicsview_edge.h"


GraphScene::GraphScene(IModelCtrl  *_mintf, QObject * parent ):QGraphicsScene(parent),mintf(_mintf),gli(new QGraphicsLineItem()),srcNode(0),dstNode(0)
{    
  addItem(gli);
  gli->setVisible(false);
  gli->setZValue(-1);
  gli->setCacheMode(QGraphicsItem::NoCache);
}
GraphScene::~GraphScene()
{
  delete gli;
}
void GraphScene::setMode(GraphScene::GraphMode mode)
{
    this->mode = mode;
    if ( mode == NoAction ){
        gli->setVisible(false);
        gli->update();
    }
}
void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton )
        return;

    QPointF p = mouseEvent->scenePos();

    QList<QGraphicsItem*> si = selectedItems();

    switch(mode){
    case InsNode:{

        if(!items(p).empty())
            break;

        QString label;
        GraphicsView_Node *node = new GraphicsView_Node;

        if ( mintf->insertNode(label,node) ){
            this->clearSelection();

            this->addItem(node);
            node->setSize(QSizeF(40,40));
            p.setX(p.x()-20);
            p.setY(p.y()-20);
            node->setPos(p);
            node->setValue(label);
            node->setSelected(true);

            node->setNodeShape(GraphicsView_Node::Circle);
            node->setBrush(Qt::white);
            node->setTextColor(Qt::blue);
        } else
            delete node;

        break;}
    case DelNode:{
        QList<QGraphicsItem*> lgi =items(p);

        dstNode = 0;
        srcNode = 0;
        if(lgi.empty())
            break;

        foreach(QGraphicsItem* gi, lgi){
            GraphicsView_Node* node = dynamic_cast<GraphicsView_Node*>(gi);
            if ( node && mintf->deleteNode( node->label().toString() ) ){
                removeItem(node);
                delete node;
            }
        }

        break;}
    case InsEdge:{

        if(items(p).empty()){
            srcNode = 0;
            gli->setVisible(false);
            break;
        }

        GraphicsView_Node* node = 0;
        QList<QGraphicsItem*> lgi =items(p);
        foreach(QGraphicsItem* gi, lgi){
            node = dynamic_cast<GraphicsView_Node*>(gi);
            if ( node )
                break;

            node = 0;
        }

        if ( !node ){
            gli->setVisible(false);
            srcNode = 0;
            break;
        }

        if ( !srcNode ){
            srcNode = node;
            break;
        }

        dstNode = node;
        GraphicsView_Edge *edge = new GraphicsView_Edge(srcNode,dstNode);

        if ( mintf->insertEdge(srcNode->label().toString(),dstNode->label().toString(),edge) ){
            this->clearSelection();

            this->addItem(edge);
            edge->adjust();
        } else{
            delete edge;
        }

        srcNode = dstNode;
        dstNode = 0;

        break;}
    case DelEdge:{

        if(items(p).empty()){

            break;
        }

        GraphicsView_Edge* edge = 0;
        QList<QGraphicsItem*> lgi =items(p);
        foreach(QGraphicsItem* gi, lgi){
            edge = dynamic_cast<GraphicsView_Edge*>(gi);
            if ( edge )
                break;

            edge = 0;
        }

        if ( edge && mintf->deleteEdge(edge->sourceNode()->label().toString(),edge->destNode()->label().toString()) ){
            removeItem(edge);
            delete edge;
        }

        break;
        }
    case NoAction:
    default:{
        dstNode = 0;
        srcNode = 0;

        gli->setVisible(false);
        gli->update();

        break;
    }

    };

    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if ( srcNode && mode == InsEdge ){
        gli->setVisible(true);
        QPointF p = mouseEvent->scenePos();

        gli->setLine(QLineF(srcNode->pos(),p));

    }
    QGraphicsScene::mouseMoveEvent(mouseEvent);
}

void GraphScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GraphScene::keyReleaseEvent(QKeyEvent *keyEvent)
{
    if ( keyEvent->key() == Qt::Key_Escape)
    {
        setMode(NoAction);
    }
    QGraphicsScene::keyReleaseEvent(keyEvent);
}

