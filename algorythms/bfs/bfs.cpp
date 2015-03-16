#include "bfs.h"
#include "graphicsview_node.h"
#include "graphicsview_edge.h"
#include "util.h"

#include <queue>


RegisterFactoryItem(BFS,AlgorythmFactory,"Gráf algoritmusok/Szélességi bejárás")

BFS::BFS():GraphAlgorythm( new BFSGraphModel() )
{
}
BFS::~BFS()
{  
}
void BFS::runGraphAlgorythm(Graph<QString,QVariant> ::Node *startNode)
{
    if ( !startNode ){
        printLine("<font color=\"red\">Kérem jelöljön ki egy <b>kezdő</b> csúcsot!</font>");
        return;
    }

    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    setState(AlgorythmStates::Running);

    std::list<Graph<QString,QVariant> ::Node *>szb;

    for( size_t i = 0; i < gm->nodesCount();++i){
        Graph<QString,QVariant> ::Node *n =  (*gm)[i];
        gm->setNodeWeight(n,key_visited,QVariant());
        for( size_t j = 0 ; j < n->edgesCount();++j){
            Graph<QString,QVariant> ::Edge *e = n->at(j);
            gm->setEdgeWeight(e,key_edgetype,1);
        }
    }

    std::queue<Graph<QString,QVariant> ::Node *> q;
    Graph<QString,QVariant> ::Node *x;
    Graph<QString,QVariant> ::Node *y;

    startNode->value(key_visited,true);

    gm->setNodeWeight(startNode,key_visited,true);

    q.push(startNode);

    while( !q.empty() ){
        x = q.front();
        ITT( "Queue dequeu " + ( x->label().toString() ) );

        szb.push_back(x);

        q.pop();



        for( size_t i = 0; i < x->edgesCount();++i){
            y = x->at(i)->endNode();
            if ( !y->value(key_visited).toBool() ){
                y->value(key_visited,true);
                //x->at(i)->value(key_treeedge,true);

                  gm->setNodeWeight(y,key_visited,true);
                  gm->setEdgeWeight(x->at(i),key_edgetype,2);
                  gm->setEdgeWeight(x->at(i)->endNode()->findEdge(x),key_edgetype,2);

                q.push(y);
                ITT("Key visited. Queue enqueu " + y->label().toString()) ;

            }
        }

    }


    QString line;
    for ( std::list< Graph<QString,QVariant> ::Node *>::iterator it = szb.begin(); it != szb.end() ;++it){
    Graph<QString,QVariant> ::Node * n = *it;
        if ( !line.isEmpty() )
            line += ", ";
        line += n->label().toString() ;
    }

    printLine(line);
}


BFSGraphModel::BFSGraphModel(): GraphModel(){}

bool BFSGraphModel::insertEdge(const QString &srcLabel, const QString &dstLabel, GraphicsView_Edge *edge){

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

void BFSGraphModel::setNodeWeight(Graph::Node *n, QString key, QVariant value) {

    GraphModel::setNodeWeight(n,key,value);

    GraphicsView_Node *gn = VPtr<GraphicsView_Node>::asPtr( n->value("gnode") );

    if ( key == key_visited ){

        if ( value.toBool() ){
            gn->setBrush(Qt::darkGreen);
            gn->setTextColor(Qt::white);
        } else
        {
            gn->setBrush(Qt::white);
            gn->setTextColor(Qt::blue);
        }
    }


}

void BFSGraphModel::setEdgeWeight(Graph::Edge *e, QString key, QVariant value) {


    GraphModel::setEdgeWeight(e,key,value);

    GraphicsView_Edge *ge = VPtr<GraphicsView_Edge>::asPtr( e->value("gedge") );

    bool ok = false;
    int ind = value.toInt(&ok);

    if ( key == key_edgetype ){
        if( ind == 1 )
            ge->setColor(Qt::darkGray);
        else if( ind == 2 )
            ge->setColor(Qt::green);
        else
            ge->setColor(Qt::black);

    }


}
