#ifndef BUCKETGRAPHICSITEM_H
#define BUCKETGRAPHICSITEM_H

#include <QGraphicsItem>
#include <QPainter>
/**
 * @brief The BucketGraphicsItem class
 * Ez alapján:
 *  https://www.cs.usfca.edu/~galles/visualization/BucketSort.html
 */
class BucketGraphicsItem :public QGraphicsObject
{
    Q_OBJECT
    Q_PROPERTY(QSizeF size READ size WRITE setSize)

    QSizeF m_size;
    QString title;
public:
    explicit BucketGraphicsItem(QSizeF m_size =QSizeF(150.0,200.0), QGraphicsItem *parent=0);
    ~BucketGraphicsItem();

    QRectF boundingRect() const;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                    QWidget *widget);

    void setTitle(const QString &ch);
    void setSize(const QSizeF size){
        prepareGeometryChange();
        this->m_size = size;
    }
    QSizeF size() const{
        return m_size;
    }
};

#endif // BUCKETGRAPHICSITEM_H
