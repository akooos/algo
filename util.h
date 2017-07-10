#ifndef UTIL
#define UTIL



#include <QEventLoop>
#include <QDesktopWidget>
#include <QApplication>
#include <QAbstractAnimation>
#include <QDebug>
#include "defs.h"
#include "csingleton.h"
class Util: public CSingleton<Util>
{
    friend class CSingleton<Util>;

   Util(){
   }

public:
  void startAndWaitForAnimation(QAbstractAnimation *anim){

     if ( !anim )
         return;

     QEventLoop event_loop;

     bool check = QObject::connect(anim,SIGNAL(finished()), &event_loop, SLOT(quit()));

     Q_ASSERT(check);

     check = QObject::connect(anim,SIGNAL(destroyed()),&event_loop,SLOT(quit()));

     Q_UNUSED(check);

     anim->start(QAbstractAnimation::DeleteWhenStopped);

     event_loop.exec();


   }
   std::string numberToASCIINumber(size_t nr){

       size_t m = nr % 26;
       std::string res ;
       res = res + ((char)(65+m));

       nr = nr / 26;

       while ( nr != 0 ){
           m = nr % 26;
           res =  (char)(65+m-1) + res;
           nr = nr / 26;
       }

       return res;
   }

   void moveToCenter(QWidget *widget)
   {

           QSize size = widget->size();
           QDesktopWidget *d = QApplication::desktop();
           int snr = d->screenNumber(widget);

           QRect sgeom = d->screenGeometry(snr);

           int w = sgeom.width();   // returns screen width
           int h = sgeom.height();  // returns screen height
           int mw = size.width();
           int mh = size.height();
           int cw = (w/2) - (mw/2);
           int ch = (h/2) - (mh/2);

           widget->move(cw,ch);

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

