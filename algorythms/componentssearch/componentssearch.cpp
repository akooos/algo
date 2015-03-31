#include "componentssearch.h"

RegisterFactoryItem(ComponentsSearch,AlgorythmFactory,"Gráf algoritmusok/Erős komponensek keresése")

#include <vector>


#include "graphscene.h"
#include "graphicsview_node.h"
#include "graphicsview_edge.h"
#include "util.h"
#include "colorizer.h"
void ComponentsSearch::runGraphAlgorythm(GraphNode *startNode)
{
    if ( !startNode ){
        printLine("<font color=\"red\">Kérem jelöljön ki egy <b>kezdő</b> csúcsot!</font>");
        return;
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

    while(!ti_classes.isEmpty()){
        QGraphicsTextItem *ti = ti_classes.takeLast();
        scene->removeItem(ti);
        delete ti;
    }


    printLine("1. Gráf mélységi bejárása, végleg átlépett csomópontok verembe gyűjtése.<br> A verem tetején a legutoljára átlépett csúcs lesz.");
    //Végig megyek a gráfon mélységi bejárással, közben egy címke stacket építek
    //amely a bejárt csúcsokat tartallmazza bejárási számsor szerint csökkenően
    std::stack<QVariant> dfs_labels = create_dfs_nodes_label();
    printLine("2. Gráf megfordítása (transzponálás/élek irányának megfordítása)");
    //Gráf forditottjának elkészítése
    Graph<QString,QVariant> *gm_trasp = gm->transponation();
    printLine("3. Megfordított gráf mélységi bejárása 1. lépésben lévő veremben lévő pontok alapján.");
    //2. DFS - Mélységi bejárás
    std::stack<QVariant> dfs_comp_labels;

    ColorModels::HSV startClr;
    startClr.value = 0.5;
    startClr.saturation = 0.7;
    Colorizer colorizer( startClr, gm->nodesCount() );

    QColor comp_clr;
    ushort  comp_cnt = 0;
    QGraphicsTextItem *ti = 0;
    QPointF tipos = QPointF(5,50);

    while ( !dfs_labels.empty() )
    {
       n = gm_trasp->findNodeByValue( dfs_labels.top() );

       if ( isNodeVisited(n)){
           dfs_labels.pop();
           continue;
       }

       ITT("Forditott gráf vizsgálata" << dfs_labels.top().toString() << " csomópontból");
       printLine("Fordított gráf vizsgálata " + dfs_labels.top().toString() + " csomópontból. ");

       dfs(gm_trasp,n,dfs_comp_labels);

       ITT("Új komponens! tagok száma:" << dfs_comp_labels.size())
       lines = QString("&#9830; C<sub>%1</sub> = { ").arg(comp_cnt);
       comp_clr = colorizer.nextColor();
       ++comp_cnt;


       while ( !dfs_comp_labels.empty() ){

           ITT("Tag: " << dfs_comp_labels.top().toString())
           lines += QString( "%1 ").arg(dfs_comp_labels.top().toString());
           n = gm->findNodeByValue( dfs_comp_labels.top() );

           gm->setNodeWeight(n,"componentcolor",comp_clr);

           dfs_comp_labels.pop();
       }

       lines += " } ";

       ti = new QGraphicsTextItem();
       scene->addItem(ti);
       ti->setPos(tipos);
       ti->setHtml(lines);
       ti->setDefaultTextColor(comp_clr);
       QFont f = ti->font();
       f.setPointSize(14);
       ti->setFont(f);
       ti->setFlags(QGraphicsItem::ItemIsMovable);
       tipos += QPointF(0,ti->boundingRect().size().height());

       printLine(lines);
       dfs_labels.pop();
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
    if ( key == "componentcolor" )
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

void ComponentsSearch::dfs(Graph<QString, QVariant> *g, GraphNode *node, std::stack<QVariant> &traversal)
{
    node->value("visited",true);
    ITT("dfs() Visited: " << node->label().toString())
    GraphEdge *e;
    GraphNode *nn;
    for( size_t i = 0; i< node->edgesCount();++i){
        e = node->at(i);
        nn = e->endNode();
        if ( !isNodeVisited( nn ) )
            dfs(g,nn,traversal);

    }

    traversal.push(node->label());

}

std::stack<QVariant> ComponentsSearch::create_dfs_nodes_label()
{
    std::stack<QVariant>  result;
    GraphNode * n;
    for ( size_t i = 0; i < gm->nodesCount() ; ++i ){
        n = (*gm)[i];
        if ( !isNodeVisited( n ))
           dfs(gm,n,result);
    }

    return result;
}
