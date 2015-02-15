#ifndef BUCKETSORT_H
#define BUCKETSORT_H

#include "algorythm.h"
#include "util.h"


#include <QLabel>

#include <QPropertyAnimation>

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


static bool regged;

private:
  Ui::BucketSortWidget *ui;
  QLabel *lb;
  QPropertyAnimation *animation;
public slots:
  virtual void start();
  virtual void stop();

private slots:
  void onPbtnExampleClicked();

};



#endif // BUCKETSORT_H
