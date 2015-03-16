#ifndef GRAPHICSVIEW_EDGE_H
#define GRAPHICSVIEW_EDGE_H

#include <QGraphicsObject>
#include <QGraphicsEllipseItem>

#include "defs.h"

class QPainter;

class GraphicsView_Node;



class GraphicsView_Edge : public QGraphicsObject
{
    Q_OBJECT

    //Q_PROPERTY ( QSizeF size READ size WRITE setSize )
    Q_PROPERTY ( QColor color READ color WRITE setColor )

    constant int adjust_size = 10;

  //  QSizeF    p_size;

    QGraphicsEllipseItem *gei;
    QGraphicsEllipseItem *gei2;

    GraphicsView_Node  *srcNode;
    GraphicsView_Node  *dstNode;
    QPointF srcPoint;
    QPointF dstPoint;
    qreal arrowSize ;
    QColor p_clr;
    QPainterPath pp;
protected:
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

public:

    void adjust();
    GraphicsView_Edge(GraphicsView_Node  *srcNode, GraphicsView_Node  *dstNode, QGraphicsItem * parent = 0);
    virtual ~GraphicsView_Edge();

    enum { Type = UserType + 561 };
    virtual int type() const Q_DECL_OVERRIDE { return Type; }

    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;

    const QColor &color() const{
        return p_clr;
    }

    void setColor(const QColor & color){
        p_clr = color;
        update();
    }
    GraphicsView_Node  * sourceNode(){
        return srcNode;
    }
    GraphicsView_Node  * destNode(){
        return dstNode;
    }
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &label) Q_DECL_OVERRIDE;

    virtual QPainterPath shape() const;


};

#endif // GRAPHICSVIEW_EDGE_H
