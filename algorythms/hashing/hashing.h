#ifndef HASHING_H
#define HASHING_H

#include "algorythm.h"
#include "util.h"

#include <QPrinter>
#include <QFileDialog>
#include <QDir>
#include <QtCore/qmath.h>
#include <math.h>

#include <ctime>


using namespace Algo;

namespace Ui{

  class HashingWidget;

}

/*
struct NeptunId{

    char id[6];


    explicit NeptunId(){}

    explicit NeptunId(const QString &id){
       setId(id);
    }

    NeptunId(const NeptunId &other){
      for( int i = 0; i < 6 ; ++i)
          this->id[i] = other.id[i];
    }


    const NeptunId & operator=(const NeptunId &other) {
        if ( this == &other )
            return *this;

        for( int i = 0; i < 6 ; ++i)
            this->id[i] = other.id[i];

        return *this;
    }
    void setId(const QString &id){

        for(int i =0; i < 6 ;++i)
            this->id[i] = 0;

        for(int i =0; i < qMin<int>(id.length(),6) ;++i)
            this->id[i] = id.at(i).toLatin1();
    }

    operator QString() const{

        return QString::fromLatin1(id);
    }

    QString toString() const{
        return QString::fromLatin1(id);
    }

    bool operator==(const NeptunId &other) const{
        for( int i = 0; i < 6 ; ++i)
            if ( this->id[i] != other.id[i] )
                return false;
        return true;
    }

    static bool isValid(const NeptunId &nid){

        QRegExp rexp("^[A-Z0-9]{6}$");
        QString str = nid;
        return str.contains(rexp);
    }


};*/

class Hashing :   public Algo::Algorythm, public FactoryItem<Hashing>
{
    Q_OBJECT

public:
    typedef unsigned int HashKey;
    typedef unsigned int Count;

    Hashing();
    virtual ~Hashing();

private:


    qreal _KnutValue;

/*


    qreal  inline fraction(qreal value){
        return  value - qFloor(value);
    }

*/

    qreal inline fraction(qreal value){

        double int_part;
       return modf(value,&int_part);
    }

    static bool isPrime(int number){

        if(number < 2)
            return false;
        if(number == 2)
            return true;
        if(number % 2 == 0)
            return false;

        for(int i=3; (i*i)<=number; i+=2){
            if(number % i == 0 )
                return false;
        }
        return true;

    }

    static bool isValid(const QString &str){
        if ( str.length() != 6 )
             return false;

        QRegExp rexp("^[A-Z0-9]{6}$");
        return str.contains(rexp);

    }

    struct HashEntry{
        QString   value;
        QString   key;
      //  Count collision_count;
        HashEntry(){
       //     collision_count = 0;
        }

        bool operator==(const QString &str) const{
            return key == str;
        }
    };
    QVector<HashEntry*> tbl1;
    QVector<HashEntry*> tbl2;
    QVector<HashEntry*> tbl3;


    QStringList neptun_ls;
    Count table_size1;
    Count table_size2;
    Count table_size3;

    int hashOf(const QString &id, const QVector<HashEntry*> &tbl ) const;
    void initHTs();
    void freeHTs();
    void initHT(QVector<HashEntry*> &tbl, Count table_size);
    void freeHT(QVector<HashEntry*> &tbl);

    void print2Doc();

    void hashIn1(const QString &id );
    void hashIn2(const QString &nid );
    void hashIn3(const QString &nid );

    Ui::HashingWidget *ui;

private slots:
    void printPDF();

public slots:

    virtual void start();
    virtual void stop();

};

#endif // HASHING_H
