#include "my.h"


#include "graphscene.h"
#include "graphicsview_node.h"
#include "graphicsview_edge.h"
#include "util.h"

RegisterFactoryItem(LangBuilder,AlgorythmFactory,"Gráf algoritmusok/Nyelv építő")


bool LangBuilderGraphModel::insertEdge(const QString &srcLabel, const QString &dstLabel, GraphicsView_Edge *edge){

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

void LangBuilder::runGraphAlgorythm(Graph<QString,QVariant> ::Node *startNode)
{

}

LangBuilder::LangBuilder():GraphAlgorythm( new LangBuilderGraphModel() )
{

}

LangBuilder::~LangBuilder()
{

}
