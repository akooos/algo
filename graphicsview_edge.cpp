#include <QtCore/qmath.h>
#include <QPainter>
#include "graphicsview_node.h"
#include "graphicsview_edge.h"
#include "graphscene.h"

static const double Pi = 3.14159265358979323846;
static const double TwicePi = 2* Pi;
static const float  CurvePointCircleRadius = 10;


void GraphicsView_Edge::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsObject::mouseReleaseEvent(event);
    update();
}

void GraphicsView_Edge::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsObject::mouseMoveEvent(event);
    update();
}

void GraphicsView_Edge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
QGraphicsObject::mousePressEvent(event);
    update();
}

void GraphicsView_Edge::adjust()
{
    arrowSize = 3;

    if (!srcNode || !dstNode)
        return;

    QPointF ps = mapFromItem(srcNode, 0, 0);
    QPointF pe = mapFromItem(dstNode, 0, 0);

    ps.setX(ps.x()+(srcNode->size().width()/2 ) ) ;
    ps.setY(ps.y()+(srcNode->size().height()/2 ) ) ;

    pe.setX(pe.x()+(dstNode->size().width()/2 ) ) ;
    pe.setY(pe.y()+(dstNode->size().height()/2 ) ) ;

    QLineF line(ps, pe);
    qreal length = line.length();

    prepareGeometryChange();
    QPointF _srcPoint;
    QPointF _dstPoint;

    if (length > qreal(20.)) {
        QPointF edgeOffset((line.dx() * 10) / length, (line.dy() * 10) / length);
        _srcPoint = line.p1() + edgeOffset;
        _dstPoint = line.p2() - edgeOffset;
    } else {
        _srcPoint = _dstPoint = line.p1();
    }


    qreal dx1 =  _srcPoint.x() - srcPoint.x();
    qreal dy1 =  _srcPoint.y() - srcPoint.y();

    qreal dx2 = _dstPoint.x() - dstPoint.x();
    qreal dy2 = _dstPoint.y() - dstPoint.y();

    QPointF pf = gei->pos();
    pf.setX(pf.x()+dx1+dx2);
    pf.setY(pf.y()+dy1+dy2);

    gei->setPos(pf);

    pf = gei2->pos();
    pf.setX(pf.x()+dx1+dx2);
    pf.setY(pf.y()+dy1+dy2);

    gei2->setPos(pf);

    srcPoint = _srcPoint;
    dstPoint = _dstPoint;


    pp = QPainterPath();
    pp.moveTo(srcPoint);

    pp.cubicTo(gei->pos(),gei2->pos(),dstPoint);
    update();

}

GraphicsView_Edge::GraphicsView_Edge(GraphicsView_Node *srcNode, GraphicsView_Node *dstNode, QGraphicsItem *parent):QGraphicsObject(parent)
{
    p_clr = Qt::black;
    this->srcNode = srcNode;
    this->dstNode = dstNode;

   gei = new QGraphicsEllipseItem(this);
   gei2 = new QGraphicsEllipseItem(this);
   setCacheMode(QGraphicsItem::NoCache);
   setFiltersChildEvents(true);
    setFlag(ItemIsSelectable,true);


    setCacheMode(NoCache);
    setZValue(-1);

    srcNode->addEdge(this);
    dstNode->addEdge(this);
    adjust();

    gei->setPos( QPointF( srcPoint.x() + (( dstPoint.x() - srcPoint.x() ) /3) , srcPoint.y() +( ( dstPoint.y() - srcPoint.y() ) /3 ) ) );
    gei2->setPos( QPointF( srcPoint.x() + (2* ( dstPoint.x() - srcPoint.x() ) /3) , srcPoint.y() +( 2* ( dstPoint.y() - srcPoint.y() ) /3 ) ) );

    gei->setVisible(false);
    gei->setRect(-CurvePointCircleRadius/2,-CurvePointCircleRadius/2,CurvePointCircleRadius,CurvePointCircleRadius);
    gei->setZValue(10);
    gei->setBrush(Qt::lightGray);
    gei->setFlag(ItemIsMovable,true);
    gei->setCacheMode(QGraphicsItem::NoCache);

    gei2->setVisible(false);
    gei2->setRect(-CurvePointCircleRadius/2,-CurvePointCircleRadius/2,CurvePointCircleRadius,CurvePointCircleRadius);
    gei2->setZValue(10);
    gei2->setBrush(Qt::lightGray);
    gei2->setFlag(ItemIsMovable,true);
    gei2->setCacheMode(QGraphicsItem::NoCache);

}

GraphicsView_Edge::~GraphicsView_Edge()
{
  srcNode->removeEdge(this);
  dstNode->removeEdge(this);
  delete gei;
  delete gei2;
}

QRectF GraphicsView_Edge::boundingRect() const
{
    if (!srcNode || !dstNode)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + arrowSize) / 2.0;

    QPolygonF p;

    p << srcPoint;
    p << dstPoint;
    p << gei->pos();
    p << gei2->pos();

    return p.boundingRect()
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}
void GraphicsView_Edge::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (!srcNode || !dstNode)
        return;

    QLineF line(srcPoint, dstPoint);
    if (qFuzzyCompare(line.length(), qreal(0.)))
        return;

    painter->setRenderHint(QPainter::Antialiasing, true);

    if ( isSelected() )
    {
        QPen pen(Qt::darkGray);
        pen.setWidth(1);
        pen.setStyle(Qt::DotLine);
        painter->setPen(pen);
        painter->drawRect(boundingRect());
    }

    painter->setPen(QPen(p_clr, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));



    //painter->drawLine(line);
    painter->drawPath(shape());

    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle =  - angle;

    QPointF sourceArrowP1 = srcPoint + QPointF(sin(angle + Pi / 3) * arrowSize,
                                                  cos(angle + Pi / 3) * arrowSize);
    QPointF sourceArrowP2 = srcPoint + QPointF(sin(angle + Pi - Pi / 3) * arrowSize,
                                                  cos(angle + Pi - Pi / 3) * arrowSize);
    QPointF destArrowP1 = dstPoint + QPointF(sin(angle - Pi / 3) * arrowSize,
                                              cos(angle - Pi / 3) * arrowSize);
    QPointF destArrowP2 = dstPoint + QPointF(sin(angle - Pi + Pi / 3) * arrowSize,
                                              cos(angle - Pi + Pi / 3) * arrowSize);

    painter->setBrush(Qt::black);
    painter->drawPolygon(QPolygonF() << line.p1() << sourceArrowP1 << sourceArrowP2);
    painter->drawPolygon(QPolygonF() << line.p2() << destArrowP1 << destArrowP2);

}

QVariant GraphicsView_Edge::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &label)
{
    if ( change == QGraphicsItem::ItemSelectedHasChanged ){

        gei->setVisible(isSelected());
        gei2->setVisible(isSelected());
    }

    return QGraphicsObject::itemChange(change,label);
}

QPainterPath GraphicsView_Edge::shape() const
{
    return pp;
}






