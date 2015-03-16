#include "dijkstra.h"

RegisterFactoryItem(Dijkstra,AlgorythmFactory,"Gráf algoritmusok/Dijkstra")


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

}
