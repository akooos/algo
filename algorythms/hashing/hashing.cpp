#include "hashing.h"

#include "ui_hashing.h"
RegisterFactoryItem(Hashing,AlgorythmFactory,"Hash functions")
void Hashing::hashIn1(const QString &nid )
{
    //http://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
    /*
    For each of these algorithms, let h be the output value. Set h to 0.
    Walk down the sequence of integers, adding the integers one by one to h.
    The algorithms differ in exactly how to combine an integer ki with h.
    The final return value is h mod m.

    CRC variant: Do a 5-bit left circular shift of h. Then XOR in ki. Specifically:

         highorder = h & 0xf8000000    // extract high-order 5 bits from h
                                       // 0xf8000000 is the hexadecimal representation
                                       //   for the 32-bit number with the first five
                                       //   bits = 1 and the other bits = 0
         h = h << 5                    // shift h left by 5 bits
         h = h ^ (highorder >> 27)     // move the highorder 5 bits to the low-order
                                       //   end and XOR into h
         h = h ^ ki                    // XOR h and kiű
*/
    //Count hash  = ((nid[0].toLatin1() ^ nid[1].toLatin1() ^ nid[2].toLatin1() ^ nid[3].toLatin1() ^ nid[4].toLatin1() ^ nid[5].toLatin1() ) % m);

    Count hash = 0;
    Count highorder = 0;

    for( int i = 0; i < 6;++i){
        highorder = hash & 0xf8000000;
        hash = hash << 5;
        hash = hash ^ (highorder >> 27);
        hash = hash ^ ((unsigned int)(nid[i].toLatin1()));
    }

    //OSztó módszer - Division method
    hash = hash % table_size1;

    while ( tbl1[hash] && tbl1[hash]->key.compare(nid) != 0  ){
        //tbl1[hash]->collision_count++;
        hash = (hash + 1) % table_size1;
        qDebug() << "Csomópont ezért eltolás:" << hash;
        //Sokaig tart neha, nem valaszolunk a windowsnak azt hiszi vegtelen ciklusban vagyunk
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
     }

    if ( tbl1[hash] && tbl1[hash]->key.compare(nid) == 0 )
        return;

    HashEntry *he = new HashEntry;
    he->key = nid;
    he->value = nid;
    qDebug() << "Új hash:" << hash << " -> " << he->key;
    tbl1[hash] = he;
}

void Hashing::hashIn2(const QString &nid )
{
    //http://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
    /*
PJW hash (Aho, Sethi, and Ullman pp. 434-438): Left shift h by 4 bits. Add in ki. Move the top 4 bits of h to the bottom. Specifically:

     // The top 4 bits of h are all zero
     h = (h << 4) + ki               // shift h 4 bits left, add in ki
     g = h & 0xf0000000              // get the top 4 bits of h
     if (g != 0)                     // if the top 4 bits aren't zero,
        h = h ^ (g >> 24)            //   move them to the low end of h
        h = h ^ g
     // The top 4 bits of h are again all zero
PJW and the CRC variant both work well and there's not much difference between them. We believe that the CRC variant is probably slightly better because

It uses all 32 bits. PJW uses only 24 bits. This is probably not a major issue since the final value m will be much smaller than either.
5 bits is probably a better shift value than 4. Shifts of 3, 4, and 5 bits are all supposed to work OK.
Combining values with XOR is probably slightly better than adding them. However, again, the difference is slight.
*/
    //Count hash  = ((nid[0].toLatin1() ^ nid[1].toLatin1() ^ nid[2].toLatin1() ^ nid[3].toLatin1() ^ nid[4].toLatin1() ^ nid[5].toLatin1() ) % m);

    Count hash = 0;
    Count g = 0;

    for( int i = 0; i < 6;++i){
        hash = ( hash << 4 ) + ((unsigned int)(nid[i].toLatin1()));
        g    = hash & 0xf0000000;
        if ( g ){
            hash = hash ^(g >> 24);
            hash = hash ^ g;
        }
    }

    //Knuth Variant on Division h(k) = k(k+3) mod m. Supposedly works much better than the raw division method.
    hash =( hash*(hash+3) ) % table_size2;
    int k = 1;
    Count orig_hash = hash;
    while ( tbl2[hash] && tbl2[hash]->key.compare(nid) != 0  ){
         //tbl2[hash]->collision_count++;
         hash = (orig_hash + (k*k) ) % table_size2;
         ++k;
         qDebug() << "Csomópont ezért eltolás(Quadtratic):" << hash;
         //Sokaig tart neha, nem valaszolunk a windowsnak azt hiszi vegtelen ciklusban vagyunk
         QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }
    //ugyanazt az elemet hozzáadták újra
    if ( tbl2[hash] && tbl2[hash]->key.compare(nid) == 0 )
        return;


    HashEntry *he = new HashEntry;
    he->key = nid;
    he->value = nid;
    qDebug() << "Új hash:" << hash << " -> " << he->key;
    tbl2[hash] = he;
}

void Hashing::hashIn3(const QString &nid )
{
    /*
BUZ hash: Set up a function R that takes 8-bit character values and returns random numbers. This function can be precomputed and stored in an array. Then, to add each character ki to h, do a 1-bit left circular shift of h and then XOR in the random value for ki. That is:

     highorder = h & 0x80000000    // extract high-order bit from h
     h = h << 1                    // shift h left by 1 bit
     h = h ^ (highorder >> 31)     // move them to the low-order end and
                                   // XOR into h
     h = h ^ R[ki]                 // XOR h and the random value for ki
     **/
    Count hash = 0;
    Count highorder = 0;

    for( int i = 0; i < 6;++i){

        Count ki = ((Count)(nid[i].toLatin1()));
        highorder = hash & 0x80000000;
        hash = hash << 1;
        hash = hash ^ (highorder >> 31);

        qsrand(ki);
        Count r = (Count) qrand();
        hash = hash ^ r;

    }

    /*
     Multiplication Method (Cormen). Choose m to be a power of 2. Let A be some random-looking real number. Knuth suggests M = 0.5*(sqrt(5) - 1). Then do the following:

     s = k*A
     x = fractional part of s
     h(k) = floor(m*x)

     */


    double value = _KnutValue*hash;
    double frac = fraction(value);
    hash =  qFloor( table_size3 * frac );
    //Sokaig tart neha, nem valaszolunk a windowsnak azt hiszi vegtelen ciklusban vagyunk
   // QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    int k = 1;
    Count orig_hash = hash;



    while ( tbl3[hash] && tbl3[hash]->key.compare(nid) != 0  ){

         hash = (orig_hash + (k*k)) % table_size3;
         ++k;
         qDebug() << "Csomópont ezért eltolás(Kvadratikus):" << hash;
         //Sokaig tart neha, nem valaszolunk a windowsnak azt hiszi vegtelen ciklusban vagyunk
         QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    if ( tbl3[hash] && tbl3[hash]->key.compare(nid) == 0 )
        return;
    HashEntry *he = new HashEntry;
    he->key = nid;
    he->value = nid;
    qDebug() << "Új hash:" << hash << " -> " << he->key;
    tbl3[hash] = he;

}

Hashing::Hashing():Algo::Algorythm(),ui(new Ui::HashingWidget())
{
    ui->setupUi(wg);

    bool check = QObject::connect(ui->pbSaveToPDF,SIGNAL(clicked()),this,SLOT(printPDF()));

    Q_ASSERT(check);

    Q_UNUSED(check);

    _KnutValue =  0.5*(qSqrt(5) - 1);

}

Hashing::~Hashing()
{
    freeHTs();
}

int Hashing::hashOf(const QString &id, const QVector<HashEntry*> &tbl) const
{
    for( int i = 0; i < tbl.size();++i){
        if ( tbl[i] && id == tbl[i]->key )
            return i;
    }

    return -1;

}

void Hashing::initHTs()
{
    const Count limit = 100;
    //Tábla méret (m) megválasztása
    //Prím legyen 2 hatványhoz nem közeli, na legyen csak prim
    qsrand(time(NULL));
    int size = qrand() % limit;

    while( !isPrime(size) || size <= neptun_ls.count() ){
        qsrand(time(NULL));
        size = qrand() % limit;
    }
    table_size1 = size;


    //Tábla méret (m) megválasztása
    //Prím legyen 2 hatványhoz nem közeli, na legyen csak prim
    qsrand(time(NULL));
    size = 0;

    while( !isPrime(size) || size <= neptun_ls.count() ){
        qsrand(time(NULL));
        size = qrand() % limit;
    }
    table_size2 = size;


    //Tábla méret (m) megválasztása 2 hatványának
    qsrand(time(NULL));

    QVector<qreal> powerOf2;
    int i = 1;
    qreal po2 = qPow(2,i);
    while(po2 < limit*2 ){
        powerOf2.append(po2);
        ++i;
        po2  = qPow(2,i);

    }
 qsrand(time(NULL));
    size = qRound ( powerOf2.at(qrand() % powerOf2.size() ) );

    while( size <= neptun_ls.count()){
        qsrand(time(NULL));
      size = qRound ( powerOf2.at(qrand() % powerOf2.size() ) );
    }

    table_size3 = size;


    initHT(tbl1,table_size1);
    initHT(tbl2,table_size2);
    initHT(tbl3,table_size3);
}

void Hashing::freeHTs()
{
    freeHT(tbl1);
    freeHT(tbl2);
    freeHT(tbl3);
}

void Hashing::initHT(QVector<Hashing::HashEntry *> &tbl, Count table_size)
{
    for( Count i = 0 ; i < table_size;++i)
        tbl << 0;
}

void Hashing::freeHT(QVector<Hashing::HashEntry *> &tbl)
{
    while(!tbl.isEmpty())
    {
        HashEntry *he = tbl.takeFirst();
        if (he)
            delete he;
    }
}

void Hashing::print2Doc()
{
    QString str;

    str += "<html><head></head><body>";
    str += "<center><h1>Jegyzőkönyv</h1></center>";
    str += "<center>- HASÍTÓFÜGGVÉNYEK -</center>";
    str += "<br>";
    str += "<p><b>Hash fv. 1:Lineáris próbálás(Osztó módszer)</p>";
    str += QString("Tábla méret = %1 (véletlenszerű prímszám)").arg(table_size1);
    str += "<p><b>Hash fv. 3:Kvadratikus próbálás(Osztó módszer Knut variáns)</p>";
    str += QString("Tábla méret = %1 (véletlenszerű prímszám)").arg(table_size2);
    str += "<p><b>Hash fv. 3:Szorzó módszer</p>";
    str += QString("Tábla méret = %1 (2 hatvány)").arg(table_size3);
    str += "<h2>Mérési adatok</h2><br>";
    str += "<table border=\"0.5\" width=\"100%\" cellspacing=\"0\" cellpadding=\"5\" >";
    str += "<tr>";
    str += "<td><b><center>NEPTUN kód</center></b></td>";
    str += "<td><b><center>ASCII kód</center></b></td>";
    str += "<td><b><center>Hash fv. 1</center></b></td>";
    str += "<td><b><center>Hash fv. 2</center></b></td>";
    str += "<td><b><center>Hash fv. 3</center></b></td>";
    str += "</tr>";

    foreach(const QString &nstr, neptun_ls){
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
        int hash = 0;

        str  += "<tr>";
        str  += "<td>";
        str  += nstr;
        str  += "</td>";

        str  += "<td>";
        for(int i=0; i < nstr.length();++i)
            str += i< nstr.length()-1 ? QString("%1,").arg((int)nstr.at(i).toLatin1()) : QString("%1").arg( (int) nstr.at(i).toLatin1());
        str  += "</td>";

        str  += "<td>";
        hash = hashOf(nstr,tbl1);
        if ( hash != -1 && tbl1[hash] )
            str += QString("%1").arg(hash);
        str  += "</td>";
        str  += "<td>";
        hash = hashOf(nstr,tbl2);
        if ( hash != -1 && tbl2[hash]  )
            str += QString("%1").arg(hash);
        str  += "</td>";
        str  += "<td>";
        hash = hashOf(nstr,tbl3);
        if ( hash != -1 && tbl3[hash]  )
            str += QString("%1").arg(hash);
        str  += "</td>";

        str  += "</tr>";
    }
    str  += "</table>";

     str += "<br><p><a href=\"http://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html\">";
     str += "http://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html</a></p>";
     str +="</body></html>";

    ui->textBrowser->document()->clear();
    ui->textBrowser->insertHtml(str);
}

void Hashing::printPDF()
{
    QString fn = QFileDialog::getSaveFileName(wg,"Mentés PDF",QDir::homePath(),"PDF fájlok (*.pdf);",new QString("PDF fájlok (*.pdf);"));

    if ( fn.isEmpty() )
        return;

    QPrinter printer;

    printer.setOutputFileName(fn);
    printer.setOutputFormat(QPrinter::PdfFormat);
    ui->textBrowser->document()->print(&printer);
}

void Hashing::start()
{
    setState(Algo::AlgorythmStates::Running);
    neptun_ls = ui->plainTextEdit->toPlainText().split(QRegExp("[,;\r\n]"));

    initHTs();
    //tábla feltöltése...

    for(int i = 0; i < neptun_ls.size();){
        const QString &str = neptun_ls.at(i);
        if ( !isValid(str)){
            qWarning() << "Nem valós NEPTUN azonosító(átugrom):" << str;
            neptun_ls.removeOne(str);
            continue;
        }
        hashIn1(str);
        hashIn2(str);
        hashIn3(str);
        ++i;
        //qDebug() << "Added:" << str;
        QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    print2Doc();

    freeHTs();

    setState(Algo::AlgorythmStates::Stopped);
}

void Hashing::stop()
{

}
