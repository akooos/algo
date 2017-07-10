#include "graphicsview_node.h"
#include "graphicsview_edge.h"

#include "graphscene.h"

GraphicsView_Node::GraphicsView_Node(QGraphicsItem * parent):QGraphicsObject(parent)
{  

  ti = new QGraphicsTextItem(this);
  setFlag(ItemIsSelectable,true);
  setFlag(ItemIsMovable,true);
  setFlag(ItemSendsGeometryChanges,true);

  setCacheMode(DeviceCoordinateCache);
  setZValue(1);
  setSize( QSizeF(30,30) );
  setNodeShape(Rectangle);

}

GraphicsView_Node::~GraphicsView_Node()
{
    delete ti;
    qDeleteAll(edgeList.begin(),edgeList.end());
    edgeList.clear();
}

void GraphicsView_Node::addEdge(GraphicsView_Edge *edge)
{
    edgeList.append( edge );
    edge->adjust();
}

void GraphicsView_Node::removeEdge(GraphicsView_Edge *edge)
{
     edgeList.removeOne(edge);

}
QRectF GraphicsView_Node::boundingRect() const
{
    qreal rw = p_size.width() ;
    qreal rh = p_size.height();

    qreal pad = adjust_size/2;
    return QRectF(-pad,-pad,rw+pad,rh+pad);
}
void GraphicsView_Node::posCenterText()
{
   QRectF rect = ti->boundingRect();
   qreal x = ( p_size.width()  - rect.width()  ) / 2 ;
   qreal y = ( p_size.height() - rect.height() ) / 2 ;
   ti->setPos(QPointF(x,y));

}
void GraphicsView_Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    Q_UNUSED(option)

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    if ( isSelected() )
    {
        QPen pen(Qt::darkGray);
        pen.setWidth(1);
        pen.setStyle(Qt::DotLine);
        painter->setPen(pen);
        painter->drawRect(boundingRect());
        QPen p = p_pen;
        p.setWidth(3);
        painter->setPen(p);
    }else
        painter->setPen(p_pen);

    painter->setBrush(p_brush);
    painter->drawPath(pp);
    painter->restore();
}

QPainterPath GraphicsView_Node::shape() const
{    
   return pp;
}

QVariant GraphicsView_Node::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    switch (change) {
    case ItemPositionHasChanged:
        foreach (GraphicsView_Edge *edge, edgeList)
            edge->adjust();

        break;
    default:
        break;
    };

    return QGraphicsItem::itemChange(change, value);
}

void GraphicsView_Node::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseMoveEvent(event);
/*

    if (x() < 0)
      {
          setPos(0, y());
      }
      else if (x() + boundingRect().right() > scene()->width())
      {
          //setPos(scene()->width() - boundingRect().width(), y());
      }

      if (y() < 0)
      {
          setPos(x(), 0);
      }
      else if ( y()+ boundingRect().bottom() > scene()->height())
      {
         // setPos(x(), scene()->height() - boundingRect().height());
      }
*/
    update();

}

void GraphicsView_Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mousePressEvent(event);
    //update();

}

void GraphicsView_Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseReleaseEvent(event);
    //update();

}

const QSizeF &GraphicsView_Node::size() const
{
    return p_size;
}

void GraphicsView_Node::setSize(const QSizeF &value)
{
    if ( value != p_size ){
        prepareGeometryChange();
        p_size = value;
        QFont f = ti->font();
        f.setPixelSize(p_size.height() * 0.5);
        ti->setFont(f);
        ti->setTextWidth(p_size.width()-ti_padding);
        posCenterText();
    }
}
QVariant GraphicsView_Node::label() const
{
    return QVariant(ti->toPlainText());
}
void GraphicsView_Node::setValue(const QVariant &value)
{
    if ( value != ti->toPlainText() ){
        prepareGeometryChange();
        ti->setHtml("<center>"+value.toString()+"</center>");
        if ( ti->boundingRect().size().height() > this->boundingRect().size().height() || ti->boundingRect().size().width() > this->boundingRect().size().width()){
            QSizeF nsize = p_size *1.5;
            setSize(nsize);
        }
        posCenterText();
    }
}
void GraphicsView_Node::setNodeShape(const NodeShape value)
{
    if ( value != m_nodeshape ){
        prepareGeometryChange();
        m_nodeshape = value;
        pp = QPainterPath();
        switch(  m_nodeshape ){
            case GraphicsView_Node::Circle :{
                QRectF rect;
                rect.setSize(p_size);
                pp.addEllipse(rect);
                break;
            }
            case GraphicsView_Node::Rectangle :{
                QRectF rect;
                rect.setSize(p_size);
                pp.addRect(rect);
                break;
            }
            case GraphicsView_Node::RoundedRectangle:{
                QRectF rect;
                rect.setSize(p_size);
                pp.addRoundRect(rect,rect_round_radius,rect_round_radius);
                break;

            }
            case GraphicsView_Node::Triangle:{
                QRectF rect;
                rect.setSize(p_size);
                QPolygonF polygon = QPolygonF() << QPointF(rect.width()/2.0,rect.top()) << rect.bottomLeft() << rect.bottomRight();
                pp.addPolygon(polygon);
                break;

            }
            default:{
                QRectF rect;
                rect.setSize(p_size);
                pp.addEllipse(rect);
            }
        }
        update();
    }
}
GraphicsView_Node::NodeShape GraphicsView_Node::nodeShape() const
{
    return m_nodeshape;
}

void GraphicsView_Node::setBrush(const QBrush &value)
{
    p_brush = value;
    update();
}

const QBrush &GraphicsView_Node::brush() const
{
    return p_brush;
}

void GraphicsView_Node::setTextColor(const QColor &value)
{
   p_textColor = value;
   ti->setDefaultTextColor(p_textColor);
   update();
}

const QColor &GraphicsView_Node::textColor() const
{
    return p_textColor;
}

void GraphicsView_Node::setPen(const QPen &value)
{
    p_pen = value;
}

const QPen &GraphicsView_Node::pen() const
{
    return p_pen;
}

void GraphicsView_Node::setColor(QColor clr)
{
    clr.setAlpha(200);
    setBrush(clr);
    setPen(clr.darker());
}

