#ifndef UTIL
#define UTIL

#include <QDebug>
#include "csingleton.h"
#include <QEventLoop>
#include <QAbstractAnimation>
class Util: public CSingleton<Util>
{
    friend class CSingleton<Util>;

   Util(){
   }

public:
   bool startAndWaitForAnimation(QAbstractAnimation *anim){

     QEventLoop event_loop;

     bool check = QObject::connect(anim,SIGNAL(finished()), &event_loop, SLOT(quit()));

     Q_ASSERT(check);

     check = QObject::connect(anim,SIGNAL(destroyed()),&event_loop,SLOT(quit()));

     Q_UNUSED(check);

     anim->start(QAbstractAnimation::DeleteWhenStopped);

     event_loop.exec();
   }
};



template <class T> class VPtr
{
public:
    static T* asPtr(QVariant v)
    {
    return  (T *) v.value<void *>();
    }

    static QVariant asQVariant(T* ptr)
    {
    return qVariantFromValue((void *) ptr);
    }
};
#define Utils Util::instance()
#endif // UTIL

