#ifndef GRAPHICSVIEW_EDGE_H
#define GRAPHICSVIEW_EDGE_H

#include <QGraphicsObject>
#include <QGraphicsEllipseItem>

#include "defs.h"

class QPainter;

class GraphicsView_Node;



class GraphicsView_Edge : public QGraphicsObject
{
    struct Label : public QGraphicsTextItem{
        explicit Label(GraphicsView_Edge * parent);
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    };

    struct ArrowHead: public QGraphicsPolygonItem{        
        explicit ArrowHead(GraphicsView_Edge * parent);
        void adjust(GraphicsView_Edge * parent);
        void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;


        static const int    ArrowSize;

    };

    struct HandlePoint: public QGraphicsEllipseItem{

            bool  calc;
            float taux;
            float tauy;
            //float tau;
            explicit HandlePoint(GraphicsView_Edge * parent);
            virtual QVariant itemChange(GraphicsItemChange change, const QVariant &value);
            virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
        private:
            const float  CurvePointCircleRadius = 10;
    };

    Q_OBJECT
    //Q_PROPERTY ( QSizeF size READ size WRITE setSize )
    Q_PROPERTY ( QColor color READ color WRITE setColor )
  //  QSizeF    p_size;

    HandlePoint *hp;
    ArrowHead   *ah;

    GraphicsView_Node  *srcNode;
    GraphicsView_Node  *dstNode;
    QPointF srcPoint;
    QPointF dstPoint;

    QColor p_clr;
    QPainterPath pp;
    Label *lb;

protected:
  virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

public:

    void adjust(bool adjustHandlePoints = true);
    GraphicsView_Edge(GraphicsView_Node  *srcNode, GraphicsView_Node  *dstNode,bool drawArrow ,QGraphicsItem * parent = 0);
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


    void setText(const QString &txt){
        lb->setHtml(txt);
    }


};

#endif // GRAPHICSVIEW_EDGE_H
