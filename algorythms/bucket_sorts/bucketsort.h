#ifndef BUCKETSORT_H
#define BUCKETSORT_H

#include "algorythm.h"
#include "bucketgraphicsitem.h"

class QAbstractAnimation;
class QPropertyAnimation;
class QGraphicsScene;

using namespace Algo;
namespace Ui {
    class BucketSortWidget;
}

class BucketSort :  public Algorythm, public FactoryItem<BucketSort>
{
 Q_OBJECT
public:
    BucketSort();
    virtual ~BucketSort();
private:
  //felület
  Ui::BucketSortWidget *ui;

  QSizeF size;

  QPropertyAnimation *animation;
  QGraphicsScene *scene;

  //QVector<BucketGraphicsItem*> ui_buckets;
  QList<QPair<BucketGraphicsItem*,QList<QGraphicsTextItem*> > > ui_buckets;

  QGraphicsTextItem *statustext;

  void createNewNameLabel(const QString &name, int bucketIndex);
  void createNewBucket(const QString &name);

  void pushBucketOnePlace(int from, QAbstractAnimation *plus_anim = 0);
  void pushLabelOnePlace(int bucketIndex, int from, qreal push_height);

  void cleanUp();
  //algoritmushoz
  QList< QPair<QString,QList<QString> > > buckets;



public slots:
  virtual void start();
  virtual void stop();

private slots:
  void onPbtnExampleClicked();

};



#endif // BUCKETSORT_H
