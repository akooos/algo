#ifndef QGRAPHICSVIEW_NODE_H
#define QGRAPHICSVIEW_NODE_H

#include <QGraphicsItem>
#include <QRadialGradient>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QFontMetrics>
#include <QPen>


#include "defs.h"

class GraphicsView_Edge;

class GraphicsView_Node : public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY ( QSizeF size READ size WRITE setSize )
    Q_PROPERTY ( QVariant label READ label  WRITE setValue )
    Q_PROPERTY ( QBrush brush READ brush  WRITE setBrush )
    Q_PROPERTY ( QColor textcolor READ textColor WRITE setTextColor )
    Q_PROPERTY ( QPen   pen READ pen WRITE setPen )

public:

    enum NodeShape{
        RoundedRectangle = 0, Rectangle = 1,Circle = 2, Triangle = 3
     };
    explicit GraphicsView_Node(QGraphicsItem * parent = 0);
    virtual ~GraphicsView_Node();

    void addEdge(GraphicsView_Edge *edge);
    void removeEdge(GraphicsView_Edge *edge);


    enum { Type = UserType + 1001 };
    virtual int type() const Q_DECL_OVERRIDE { return Type; }

    virtual QRectF boundingRect() const Q_DECL_OVERRIDE;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) Q_DECL_OVERRIDE;
    virtual QPainterPath shape() const Q_DECL_OVERRIDE;

    virtual QVariant itemChange(GraphicsItemChange change, const QVariant &label) Q_DECL_OVERRIDE;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) Q_DECL_OVERRIDE;

    const QSizeF &size() const;
    void setSize(const QSizeF &label);

    QVariant label() const;
    void setValue(const QVariant & label);

    void setNodeShape(const NodeShape label);
    NodeShape nodeShape() const;

    void setBrush(const QBrush &label);
    const QBrush &brush() const;

    void setTextColor(const QColor &label);
    const QColor & textColor() const;

    void setPen(const QPen &label);
    const QPen &pen() const;

private:
    constant int adjust_size = 5;
    constant int ti_padding  = 5;
    constant qreal rect_round_radius = 3;

    QSizeF    p_size;
    QVariant  p_value;

    NodeShape m_nodeshape;
    bool                  m_wordwrap;

    QBrush               p_brush;
    QColor               p_textColor;
    QPen                 p_pen;

    QGraphicsTextItem     *ti;
    QPainterPath pp;

    QList<GraphicsView_Edge*> edgeList;

    void posCenterText();

};

#endif // QGRAPHICSVIEW_NODE_H
