
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include "graphscene.h"
#include "graphicsview_node.h"
#include "graphicsview_edge.h"


GraphScene::GraphScene(IModelCtrl  *_mintf, QObject * parent ):QGraphicsScene(parent),mintf(_mintf),drawArrow(false),srcNode(0),dstNode(0)
{

}

GraphScene::~GraphScene()
{

}

void GraphScene::setMode(GraphScene::GraphMode mode)
{
    this->mode = mode;
}



void GraphScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if (mouseEvent->button() != Qt::LeftButton )
        return;

    QPointF p = mouseEvent->scenePos();

    QList<QGraphicsItem*> si = selectedItems();;




    switch(mode){
    case InsNode:{

        if(!items(p).empty())
            break;

        QString label;
        GraphicsView_Node *node = new GraphicsView_Node;

        if ( mintf->insertNode(label,node) ){
            this->clearSelection();

            this->addItem(node);
            node->setSize(QSizeF(30,30));
            p.setX(p.x()-25);
            p.setY(p.y()-25);
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
        dstNode = 0;
        srcNode = 0;


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
    };





    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GraphScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
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
        mode = NoAction;
    }
    QGraphicsScene::keyReleaseEvent(keyEvent);
}

