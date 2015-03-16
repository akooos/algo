#include "bucketgraphicsitem.h"

BucketGraphicsItem::BucketGraphicsItem(QSizeF size, QGraphicsItem * parent ):QGraphicsObject(parent)
{
 this->m_size = size;

    //setFlag(ItemIsMovable | ItemSendsGeometryChanges,);
}

BucketGraphicsItem::~BucketGraphicsItem()
{

}

QRectF BucketGraphicsItem::boundingRect() const
{
  return QRectF(-8.0,-8.0,m_size.width()+8,m_size.height()+8);
}

void BucketGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->setRenderHint(QPainter::Antialiasing);

    QPen pen(Qt::black);
    painter->setPen(pen);
    QFont f = painter->font();
    f.setBold(true);
    f.setPointSize(18);

    painter->setPen(pen);
    painter->setFont(f);
    QFontMetrics fm(painter->font());
    QColor clr = Qt::lightGray;
    clr.setAlpha(150);
    painter->setBrush(clr);

    painter->drawRoundRect(0,20,m_size.width(),m_size.height()-20,10,10);

    painter->drawText( m_size.width()/2-(fm.width(title)/2),15,title);

}

void BucketGraphicsItem::setTitle(const QString &ch)
{
  this->title = ch;
}

