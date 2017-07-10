#include <QtCore/qmath.h>
#include <QPainter>
#include <QVector2D>
#include "graphicsview_node.h"
#include "graphicsview_edge.h"
#include "graphscene.h"

const int GraphicsView_Edge::ArrowHead::ArrowSize = 10;

void GraphicsView_Edge::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsObject::mouseReleaseEvent(event);
    //adjust();
}

void GraphicsView_Edge::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{

    QGraphicsObject::mouseMoveEvent(event);
    //update();
}

void GraphicsView_Edge::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mousePressEvent(event);
    //update();
}

void GraphicsView_Edge::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    GraphScene *s = dynamic_cast<GraphScene*>(scene());

    if (s){
         s->userQueryWeight(this);
    }

    QGraphicsObject::mouseDoubleClickEvent(event);
    //update();
}

void GraphicsView_Edge::adjust(bool adjustHandlePoints)
{


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
  //  if ( ah ){
        QVector2D  v = QVector2D(_srcPoint) - QVector2D(_dstPoint);
        float lambda = dstNode->size().width()/3;
        v.normalize();
        v *= lambda;
        QPointF dp = v.toPointF();
        _dstPoint+=dp;

        v *= -1;
        lambda = srcNode->size().width()/3;
        v.normalize();
        v *= lambda;
        dp = v.toPointF();
        _srcPoint += dp;

  //  }
   /* if ( adjustHandlePoints ){
        float dx =  _srcPoint.x()- srcPoint.x() +  _dstPoint.x() - dstPoint.x();
        float dy =  _srcPoint.y() - srcPoint.y() + _dstPoint.y() - dstPoint.y();
        //QVector2D d(_dstPoint-_srcPoint);

       // ITT( hp->taux << "TAUX")
       // ITT( hp->tauy << "TAUY" )

        //dx *= hp->taux;
        //dy *= hp->tauy;

      //  ITT("Edge adjust")
        //d *= hp->tau;
        //QPointF pf = _srcPoint + d.toPointF();
       // QPointF pf;
       // pf.setX( (1-hp->taux)*_srcPoint.x() + (hp->taux*_dstPoint.x()) );
       // pf.setY( (1-hp->tauy)*_srcPoint.y() + (hp->tauy*_dstPoint.y()) );

      //  hp->calc = false;
        //hp->setPos(pf);

//        ITT(hp->tau << +" TAU")

        QPointF gti_p = lb->pos();
        gti_p += QPointF(dx,dy);
        lb->setPos(gti_p);
    }*/
    srcPoint = _srcPoint;
    dstPoint = _dstPoint;


    //lb->setPos( QPointF( srcPoint.x() + ( (( dstPoint.x() - srcPoint.x() )) /2 ) , srcPoint.y() +(( ( dstPoint.y() - srcPoint.y() ) /2) ) )  );
    hp->setPos( QPointF( srcPoint.x() + (( dstPoint.x() - srcPoint.x() ) /2) , srcPoint.y() +( ( dstPoint.y() - srcPoint.y() ) /2 ) ) );

    //QFontMetrics fm (lb->font());
    lb->setPos( QPointF( srcPoint.x() + ( (( dstPoint.x() - srcPoint.x() )) /2 ) , srcPoint.y() +(( ( dstPoint.y() - srcPoint.y() ) /2) ) )  );
    pp = QPainterPath();
    pp.moveTo(_srcPoint);
    pp.lineTo(_dstPoint);


    //pp.quadTo(hp->pos(),_dstPoint);


//    ITT("Edge adjusted" << srcNode->label().toString() << " <-> " << dstNode->label().toString() ) ;
    update();

}

GraphicsView_Edge::GraphicsView_Edge(GraphicsView_Node *srcNode, GraphicsView_Node *dstNode, bool drawArrow, QGraphicsItem *parent):QGraphicsObject(parent)
{
    p_clr = Qt::black;
    this->srcNode = srcNode;
    this->dstNode = dstNode;

    hp = new HandlePoint(this);

    lb = new Label(this);
    ah = drawArrow ? new ArrowHead(this) : 0 ;

    setFlag(ItemIsSelectable);

    setCacheMode(NoCache);
    setZValue(-1);

    srcNode->addEdge(this);
    dstNode->addEdge(this);

   // hp->calc = true;
    hp->setPos( QPointF( srcPoint.x() + (( dstPoint.x() - srcPoint.x() ) /2) , srcPoint.y() +( ( dstPoint.y() - srcPoint.y() ) /2 ) ) );

    //QFontMetrics fm (lb->font());
    lb->setPos( QPointF( srcPoint.x() + ( (( dstPoint.x() - srcPoint.x() )) /2 ) , srcPoint.y() +(( ( dstPoint.y() - srcPoint.y() ) /2) ) )  );

    adjust();

}

GraphicsView_Edge::~GraphicsView_Edge()
{
    srcNode->removeEdge(this);
    dstNode->removeEdge(this);
    delete hp;

}

QRectF GraphicsView_Edge::boundingRect() const
{
    if (!srcNode || !dstNode)
        return QRectF();

    qreal penWidth = 1;
    qreal extra = (penWidth + ArrowHead::ArrowSize) / 2.0;

    QPolygonF p;

    p << srcPoint;
    p << dstPoint;
    p << hp->pos();

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

    if ( isSelected() )
    {
        QPen pen(Qt::darkGray);
        pen.setWidth(1);
        pen.setStyle(Qt::DotLine);
        painter->setPen(pen);
        painter->drawRect(boundingRect());
    }

    painter->setPen(QPen(p_clr, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter->drawPath(shape());

    if(ah){
       ah->adjust(this);
       ah->setBrush(p_clr);
       ah->setPen(p_clr.darker());
    }

}

QVariant GraphicsView_Edge::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &label)
{
    if ( change == QGraphicsItem::ItemSelectedHasChanged ){
        hp->setVisible(isSelected());
    }

    return QGraphicsObject::itemChange(change,label);
}

QPainterPath GraphicsView_Edge::shape() const
{



    return pp;
}
GraphicsView_Edge::HandlePoint::HandlePoint(GraphicsView_Edge *parent):QGraphicsEllipseItem(parent)
{

    setVisible(false);
    setRect(-CurvePointCircleRadius/2,-CurvePointCircleRadius/2,CurvePointCircleRadius,CurvePointCircleRadius);
    setZValue(10);
    setBrush(Qt::lightGray);
    setFlag(ItemIsMovable,true);
    setCacheMode(QGraphicsItem::NoCache);
    setFlag(ItemSendsScenePositionChanges);
   // tau = 0;
    calc = false;
}

QVariant GraphicsView_Edge::HandlePoint::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
/*
    if ( change == QGraphicsItem::ItemPositionHasChanged ) {
        GraphicsView_Edge* e =qgraphicsitem_cast<GraphicsView_Edge*>(parentItem());
      //  if ( calc ){

            //taux =subf(pos().x(),e->srcPoint.x()) / subf(e->dstPoint.x(),e->srcPoint.x());
            //tauy =subf(pos().y(),e->srcPoint.y()) / subf(e->dstPoint.y(),e->srcPoint.y());
            //QVector2D q = QVector2D( QPointF(pos() - e->srcNode->pos()));
            //QVector2D p = QVector2D(e->dstNode->pos() - e->srcNode->pos());
            //tau = p.length()/q.length();
       //      ITT(taux << " TAUx")
            //         ITT(tauy << " TAUy")
         //   ITT("YOOO")
      //  }
      //  calc = true;
       // e->adjust(false);
    }
*/
    return QGraphicsEllipseItem::itemChange(change,value);
}

void GraphicsView_Edge::HandlePoint::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

   GraphicsView_Edge* e =qgraphicsitem_cast<GraphicsView_Edge*>(parentItem());
   e->setSelected(true);
  // update(); QGraphicsEllipseItem::mousePressEvent(event);
}
GraphicsView_Edge::ArrowHead::ArrowHead(GraphicsView_Edge *parent):QGraphicsPolygonItem(parent)
{
  setBrush(Qt::black);
  setVisible(true);
}

void GraphicsView_Edge::ArrowHead::adjust(GraphicsView_Edge *parent)
{
    //QLineF helperLine(parent->dstPoint,parent->hp->pos());
    //double angle = ::asin(helperLine.dy() / helperLine.length());

  // if (helperLine.dx() >= 0)
    //       angle = Pi - angle;

  // angle= 0;

    QLineF line(parent->srcPoint,parent->dstPoint);
    double angle = ::acos(line.dx() / line.length());
    if (line.dy() >= 0)
        angle = TwicePi - angle;


    QPointF destArrowP1 = parent->dstPoint  + QPointF(sin(angle - Pi / 3) * ArrowSize,
                                              cos(angle - Pi / 3) * ArrowSize);
    QPointF destArrowP2 = parent->dstPoint  + QPointF(sin(angle - Pi + Pi / 3) * ArrowSize,
                                              cos(angle - Pi + Pi / 3) * ArrowSize);
    setPolygon( QPolygonF() << parent->dstPoint << destArrowP1 << destArrowP2 );

}

void GraphicsView_Edge::ArrowHead::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    GraphScene *s = dynamic_cast<GraphScene*>(scene());

    if (s){
         s->userQueryWeight(qgraphicsitem_cast<GraphicsView_Edge*>(parentItem()));
    }
    QGraphicsPolygonItem::mouseDoubleClickEvent(event);
    update();
}


GraphicsView_Edge::Label::Label(GraphicsView_Edge *parent):QGraphicsTextItem(parent)
{
    setPlainText("");

    QFont f  = font();
    f.setPointSize(f.pointSize()+2);
    setFont(f);
    setFlag(ItemIsMovable);
    setCacheMode(NoCache);
    setDefaultTextColor(Qt::black);
    setVisible(true);
}

void GraphicsView_Edge::Label::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    GraphScene *s = dynamic_cast<GraphScene*>(scene());

    if (s){
         s->userQueryWeight(qgraphicsitem_cast<GraphicsView_Edge*>(parentItem()));
    }

    QGraphicsTextItem::mouseDoubleClickEvent(event);
   // update();
}

