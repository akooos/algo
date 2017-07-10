#include "componentssearch.h"

RegisterFactoryItem(ComponentsSearch,AlgorythmFactory,"Gráf algoritmusok/Erős komponensek keresése")

#include <vector>
#include <queue>

#include "graphscene.h"
#include "graphicsview_node.h"
#include "graphicsview_edge.h"
#include "util.h"
#include "colorizer.h"
void ComponentsSearch::runGraphAlgorythm(GraphNode *startNode)
{

    while(!ti_classes.isEmpty()){
        QGraphicsTextItem *ti = ti_classes.takeLast();
        scene->removeItem(ti);
        delete ti;
    }

    if ( !startNode ){
        if ( gm->nodesCount() == 0){
            printLine("<font color=\"red\">Kérem adjon meg egy gráfot!</font>");
            return;
        }

        startNode = gm->at(0);
    }

    QApplication::processEvents(QEventLoop::ExcludeUserInputEvents);

    QString lines = QString("Kiindulva a \"%1\" csúcsból:").arg(startNode->label().toString());

    printLine(lines);

    GraphNode *n;

    for ( size_t i = 0; i < gm->nodesCount() ; ++i ){
        n = (*gm)[i];
        gm->setNodeWeight(n,"visited",false);
        gm->setNodeWeight(n,"componentcolor",QVariant::fromValue<QColor>(Qt::black));        
    }

    printLine("1. Gráf mélységi bejárása, végleg átlépett csomópontok verembe gyűjtése.<br> A verem tetején a legutoljára átlépett csúcs lesz.");
    //Végig megyek a gráfon mélységi bejárással, közben egy címke stacket építek
    //amely a bejárt csúcsokat tartallmazza bejárási számsor szerint csökkenően
    PQ_DFS first_dfs = create_dfs_nodes_label();
    printLine("2. Gráf megfordítása (transzponálás/élek irányának megfordítása)");
    //Gráf forditottjának elkészítése
    Graph<QString,QVariant> *gm_trasp = gm->transponation();
    printLine("3. Megfordított gráf mélységi bejárása 1. lépésben lévő veremben lévő pontok alapján:");
    //2. DFS - Mélységi bejárás
    PQ_DFS second_dfs;

    ColorModels::HSV startClr;
    startClr.value = 0.5;
    startClr.saturation = 0.7;
    Colorizer colorizer( startClr, gm->nodesCount() );

    QColor comp_clr;
    ushort  comp_cnt = 0;

    QPointF tipos = QPointF(5,10);

    while ( !first_dfs.empty() )
    {
       n = gm_trasp->findNodeByValue( first_dfs.top()->label() );

       if ( isNodeVisited(n)){
           first_dfs.pop();
           continue;
       }

       ITT("Forditott gráf vizsgálata" << n->label().toString() << " csomópontból");
       printLine("Fordított gráf mélységi bejárása " + n->label().toString() + " csomópontból. " );

       int bsz = 0;
       //depth first search
       dfs(gm_trasp,n,second_dfs, bsz);

       ITT("Új komponens! tagok száma:" << second_dfs.size())
       lines = QString("&#9830; C<sub>%1</sub> = { ").arg(comp_cnt);
       comp_clr = colorizer.nextQColor();
       ++comp_cnt;

       while ( !second_dfs.empty() ){

           GraphNode* nt = second_dfs.top();

           ITT("Tag: " << nt->label().toString())
           lines += QString( "%1 ").arg(nt->label().toString());

           n = gm->findNodeByValue( nt->label() );
           gm->setNodeWeight(n,"componentcolor",comp_clr);

           second_dfs.pop();

           if ( !second_dfs.empty())
            lines += ", ";
       }

       lines += " } ";

       addTextItem(lines,comp_clr,tipos);

       printLine(lines);
       first_dfs.pop();
    }

   delete gm_trasp;

}

ComponentsSearch::ComponentsSearch():GraphAlgorythm( new ComponentsSearchGraphModel() )
{
}

ComponentsSearch::~ComponentsSearch()
{
 qDeleteAll(ti_classes);
 ti_classes.clear();
}
bool ComponentsSearchGraphModel::insertEdge(const QString &srcLabel, const QString &dstLabel, GraphicsView_Edge *edge)
{
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

bool ComponentsSearchGraphModel::setEdgeWeight(Graph::Edge *e, QString key, QVariant value)
{
    bool ok = GraphModel::setEdgeWeight(e,key,value);

    GraphicsView_Edge *ge = VPtr<GraphicsView_Edge>::asPtr( e->value("gedge") );



    if ( ge && key == "componentcolor" )
        ge->setColor(value.value<QColor>());



    return ok;
}

void ComponentsSearchGraphModel::setNodeWeight(Graph::Node *n, QString key, QVariant value)
{
    GraphModel::setNodeWeight(n,key,value);

    GraphicsView_Node *gn = VPtr<GraphicsView_Node>::asPtr( n->value("gnode") );
    if ( key == "componentcolor" && n->value("visited").toBool()  ){
        gn->setColor(value.value<QColor>());
        gn->setTextColor(Qt::white);
    } else
        {
            gn->setTextColor(Qt::blue);
            gn->setBrush(Qt::white);
            gn->setPen(QPen(Qt::black));
        }
}

void ComponentsSearch::dfs(Graph<QString, QVariant> *g, GraphNode *node, PQ_DFS &traversal,int &bsz)
{

    node->value("visited",true);    
    ITT("dfs() Visited: " << node->label().toString())            
    GraphEdge *e;
    GraphNode *nn = 0 ;
    for( size_t i = 0; i< node->edgesCount();++i){
        e = node->at(i);
        nn = e->endNode();
        if ( !isNodeVisited( nn ) ){
            dfs(g,nn,traversal,bsz);
        }
    }

    ++bsz;
    node->value("bsz",bsz);
    traversal.push(node);
}

ComponentsSearch::PQ_DFS ComponentsSearch::create_dfs_nodes_label()
{
    PQ_DFS result;
    GraphNode * n;
    int bsz = 0;
    for ( size_t i = 0; i < gm->nodesCount() ; ++i ){        

        n = (*gm)[i];

        if ( !isNodeVisited( n ))
           dfs(gm,n,result,bsz);

    }

    return result;
}

void ComponentsSearch::addTextItem(const QString &txt, QColor clr,QPointF &tipos)
{
    QGraphicsTextItem *ti = new QGraphicsTextItem();
    scene->addItem(ti);
    ti->setPos( ui->graphicsView->mapToScene(tipos.toPoint()) );
    ti->setHtml(txt);
    ti->setDefaultTextColor(clr);
    QFont f = ti->font();
    f.setPointSize(12);
    ti->setFont(f);
    ti->setFlag(QGraphicsItem::ItemIsMovable);
    tipos += QPointF(0,ti->boundingRect().size().height());

    ti_classes.append(ti);
}
