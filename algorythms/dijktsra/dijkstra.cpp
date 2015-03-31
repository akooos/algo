#include "dijkstra.h"

RegisterFactoryItem(Dijkstra,AlgorythmFactory,"Gráf algoritmusok/Dijkstra")


#include <vector>
#include <limits>
#include <mutable_priority_queue.h>

#include "graphscene.h"
#include "graphicsview_node.h"
#include "graphicsview_edge.h"
#include "util.h"


Dijkstra::Dijkstra():GraphAlgorythm( new DijkstraGraphModel() )
{

}

Dijkstra::~Dijkstra()
{

}

void Dijkstra::runGraphAlgorythm(Graph<QString,QVariant> ::Node *startNode)
{

    if ( !startNode ){
        printLine("<font color=\"red\">Kérem jelöljön ki egy <b>kezdő</b> csúcsot!</font>");
        return;
    }

    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    QString lines = QString("Kiindulva a \"%1\" csúcsból:<br>").arg(startNode->label().toString());

    //Algoritmusok könyvtől eltérően feldolgozando halmaz, amelyet folyamtosan epitek le.
    //(-) min priority queu std::list< Graph<QString,QVariant>::Node*> nodes;
    std::map<Graph<QString,QVariant> ::Node*,int> dist;
    std::map<Graph<QString,QVariant> ::Node *, Graph<QString,QVariant>::Node *> previous;

    //Functor!!!
    GraphNodeComparetor gncomp(dist);
    std::vector<GraphNode* > cntr;
    mutable_priority_queue < GraphNode* , std::vector<GraphNode* >,GraphNodeComparetor  > nodes(cntr,gncomp);

    //max() a végtelen inicializálom az összeset
    for( size_t i = 0;i < gm->nodesCount();++i) {
        GraphNode *n = (*gm)[i];
        previous[n] = 0;
        dist[n] = ( n == startNode ? 0 : MAX_INT);
        gm->setNodeWeight(n,"visited",n == startNode);
        nodes.push( n);

        for( size_t j = 0 ; j < n->edgesCount();++j){
            Graph<QString,QVariant> ::Edge *e = n->at(j);
            gm->setEdgeWeight(e,"shortcut",false);
        }
    }

    Graph<QString,QVariant>::Node *u = 0;
    while( !nodes.empty() ){

        //  ITT( nodes.size() << " NODESCOUNT")
        //extract_min
        u = 0;
        int min = 0;

        /*
        for ( std::list<Graph<QString,QVariant> ::Node *>::iterator it = nodes.begin() ; it != nodes.end(); ++it){
            if ( u ){
            if ( min > dist[*it] ){
               u = *it;
                min = dist[*it];
            }
            } else {

                u = *it;
                min = dist[*it];
            }
        } helyett...
        */

        u = nodes.top();
        min = dist[u];
        nodes.pop();

        ITT("Poped node:"<< u->label().toString() << " tavolsag " << min);

        //....fő ciklus
        //        std::list<Graph<QString,QVariant> ::Node *>::iterator it = std::find(nodes.begin(),nodes.end(),u);
        //        if ( it != nodes.end())
        //            nodes.erase(it);

        //  if ( min == INT_MAX ){
        //     ITT( u->label().toString() << " SKIPPED")
        //     continue;
        // }

        for( size_t i = 0; i < u->edgesCount();++i){
            GraphEdge *e = u->at(i);
            GraphNode *v = e->endNode();
            //int koltseg = dist[u] + getKoltseg(e);

            int k = getKoltseg(e);
            int alt = (min == INT_MAX ) ? INT_MAX : min + k;

            ITT( "Él " << i << ". költség " << k)
            if ( alt < dist[v] ){
                dist[v] = alt;
                previous[v] = u;
                gm->setNodeWeight(v,"visited",true);
                ITT("Beallitas csomopontot " << v->label().toString() <<" " << alt << "tavolsagra" )
                nodes.update(); //priority queue decrease priority
            }
        }

        ITT("Élek iterációjának vége.")

    }

    //kiírás
    for ( std::map<Graph<QString,QVariant> ::Node *,int>::iterator it = dist.begin() ; it != dist.end(); ++it){

        u = it->first;
        const int weight = it->second;
        lines += u->label().toString() + QString(" csúcs költség: %1").arg(weight == MAX_INT ? "&#8734;": QString::number( weight )) + "<br>";
    }
    GraphEdge *e = 0;
    GraphNode *v = 0;
    for ( std::map<Graph<QString,QVariant> ::Node *,Graph<QString,QVariant> ::Node *>::iterator it = previous.begin() ; it != previous.end(); ++it){

        u = it->first;
        v = it->second;
        if ( v && u){
            e = gm->findEdge(v,u);

            if ( e )
                gm->setEdgeWeight(e,"shortcut",true);
        }
    }

    printLine(lines);


    /*
   https://www.youtube.com/watch?v=8Ls1RqHCOPw
   http://hu.wikipedia.org/wiki/Dijkstra-algoritmus
*/


}


int Dijkstra::getKoltseg(Graph<QString,QVariant> ::Edge *e) const
{
    QVariant vr = e->value("cimke");

    bool ok = false;

    int koltseg = vr.toInt(&ok);

    if ( !ok ){
        //  ITT("Nem volt beállítva él címke, vagy helytelen formátumú. " << vr.toString() )
        koltseg = 0;
    }

    return koltseg;
}


bool DijkstraGraphModel::insertEdge(const QString &srcLabel, const QString &dstLabel, GraphicsView_Edge *edge){

    if ( srcLabel == dstLabel ){
        ITT("Hurok éleket nem lehet hozzáadni.");
        return false;
    }
    if ( hasEdge(srcLabel,dstLabel)){
        ITT("Párhuzamos éleket nem lehet hozzáadni.");
        return false;
    }
    return GraphModel::insertEdge(srcLabel,dstLabel,edge);

}

void DijkstraGraphModel::setNodeWeight(Graph::Node *n, QString key, QVariant value) {

    GraphModel::setNodeWeight(n,key,value);

    GraphicsView_Node *gn = VPtr<GraphicsView_Node>::asPtr( n->value("gnode") );

    if ( key == "visited" ){

        if ( value.toBool() ){
            gn->setBrush(Qt::black);
            gn->setTextColor(Qt::white);
        } else
        {
            gn->setBrush(Qt::white);
            gn->setTextColor(Qt::blue);
        }
    }

}
bool DijkstraGraphModel::setEdgeWeight(Graph::Edge *e, QString key, QVariant value) {


    bool ok = GraphModel::setEdgeWeight(e,key,value);

    GraphicsView_Edge *ge = VPtr<GraphicsView_Edge>::asPtr( e->value("gedge") );


    bool isShortCut = value.toBool();

    if ( key == "shortcut" ){
        if( isShortCut )
            ge->setColor(Qt::blue);
        else
            ge->setColor(Qt::black);

    }

    return ok;

}
