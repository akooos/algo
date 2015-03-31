#ifndef GRAPH_H
#define GRAPH_H


#include <vector>
#include <list>
#include <map>
#include <algorithm>

#include "defs.h"


namespace AdjacencyList {


enum GraphType{
    Directed, Undirected
};
/**
 * @brief The Graph class is adjacency list representation
 * Szomszédsági lista reprezentácó
 */
template <class KeyType, class ValueType>
struct Graph
{

    class Node;

    class Edge{

      std::map<KeyType,ValueType>   weights;
      Node*  end_node;
    public:
      Edge(Node* endNode){
          this->end_node = endNode;
      }

      std::vector<KeyType> keys() const{

          std::vector<KeyType> result;

          for(typename std::map<KeyType,ValueType>::const_iterator it=weights.begin();it!=weights.end();++it){
              result.push_back(it->first);
          }

          return result;
      }
      std::vector<ValueType> values() const{

          std::vector<KeyType> result;

          for(typename std::map<KeyType,ValueType>::const_iterator it=weights.begin();it!=weights.end();++it){
              result.push_back(it->second);
          }

          return result;
      }

      ValueType  value(KeyType key){
          return weights[key];
      }
      void  value(KeyType key,ValueType value){
          weights[key] = value;
          //weights.insert(std::make_pair(key,value));
      }

       Node* endNode() {
          return end_node;
      }


/*
      ValueType& operator[] (KeyType key) {
          return weights[key];
      }*/

    };

    class Node{

        std::map<KeyType,ValueType>   weights;
        std::list<Edge*> edges;


        ValueType     nodelabel;
    public:
        std::vector<KeyType> keys() const{

            std::vector<KeyType> result;

            for(typename std::map<KeyType,ValueType>::const_iterator it=weights.begin();it!=weights.end();++it){
                result.push_back(it->first);
            }

            return result;
        }
        std::vector<ValueType> values() const{

            std::vector<KeyType> result;

            for(typename std::map<KeyType,ValueType>::const_iterator it=weights.begin();it!=weights.end();++it){
                result.push_back(it->second);
            }

            return result;
        }

        ValueType  value(KeyType key){
            return weights[key];
        }
        void  value(KeyType key,ValueType value){
            weights[key]=value;
        }


        Edge * findEdge(Node *endNode){
            for ( typename std::list<Edge * >::iterator it = edges.begin();it!= edges.end();){
                if( (*it)->endNode() == endNode ){
                    Edge *edge = *it;
                    return edge;
                } else
                    ++it;
            }

            return 0;
        }

        Edge *operator [](size_t index){
            typename std::list<Edge*>::iterator it = edges.begin();
            std::advance(it,index);
            return *it;
        }
        Edge *at(size_t index){
            typename std::list<Edge*>::iterator it = edges.begin();
            std::advance(it,index);
            return *it;
        }

        size_t  edgesCount() const{
            return edges.size();
        }
        ValueType label() const{
            return nodelabel;
        }
        bool removeEdge(Node *endNode){

            for ( typename std::list<Edge * >::iterator it = edges.begin();it!= edges.end();){
                if( (*it)->endNode() == endNode ){
                    Edge *edge = *it;
                    it = edges.erase(it);
                    delete edge;
                    return true;
                } else
                    ++it;
            }

            return false;
        }

        bool hasEdge(Node *endNode){
            for ( typename std::list<Edge * >::iterator it = edges.begin();it!= edges.end();++it){
                if( (*it)->endNode() == endNode )
                   return true;

            }

                return false;
        }

        Edge * createEdge(Node* endNode){

            ITT("Model:Creating edge " << nodelabel.toString() << " -> " << endNode->label().toString())

            Edge *edge = new Edge(endNode);
            edges.push_back(edge);
            return edge;
        }



        Node(ValueType label){
            this->nodelabel = label;
        }
        Node * copyOnlyNode(bool copyweights = false){
            Node *result = new Node(nodelabel);
            if ( copyweights )
                result->weights = weights;
            return result;

        }

        ~Node(){

            while( !edges.empty()){
                Edge *edge = edges.back();
                edges.pop_back();
                delete edge;

            }
        }

    };

    typename std::list<Node*>::iterator findNode(ValueType label){

        for(typename std::list<Node*>::iterator it = nodes.begin() ;it!=nodes.end(); ++it){

            if ( (*it)->label() == label )
                return it;
        }

        return nodes.end();
    }
    Node * findNodeByValue(ValueType label){

        for(typename std::list<Node*>::iterator it = nodes.begin() ;it!=nodes.end(); ++it){

            Node *node = *it;
          //  ITT(node->label())
            if ( node->label() == label )
                return *it;
        }

        return 0;
    }
    Edge * addEdge(Node *beginNode,Node *endNode, Edge **masik_e ) {

        typename std::list<Node*>::iterator it = std::find(nodes.begin(),nodes.end(),beginNode);

        if ( it == nodes.end() )
            return 0;

        it =  std::find(nodes.begin(),nodes.end(),endNode);

        if ( it == nodes.end() )
            return 0;


        if ( gt == Undirected ){

            *masik_e = endNode->createEdge(beginNode);

            ITT("Masik created")

        }



        return beginNode->createEdge(endNode);

    }

    Edge * findEdge(Node *beginNode,Node *endNode) {

        Edge * e = beginNode->findEdge(endNode);

        if ( !e && gt == Undirected )
            e = endNode->findEdge(beginNode);

        return e;
    }



    Edge * findEdge(ValueType beginNodeLabel,ValueType endNodeLabel) {
        Node *beginNode = findNodeByValue(beginNodeLabel);
        Node *endNode = findNodeByValue(endNodeLabel);

        return findEdge(beginNode,endNode);
    }

    Edge * addEdge(ValueType beginNodeLabel,ValueType endNodeLabel , Edge **masik_e ) {
        Node *beginNode = findNodeByValue(beginNodeLabel);
        Node *endNode = findNodeByValue(endNodeLabel);

        return addEdge(beginNode,endNode,masik_e);
    }
    Node * addNode(ValueType label){

        if ( findNodeByValue(label))
            return 0;

        Node * node = new Node(label);
        nodes.push_back(node);

        ITT( "Added node:" + label.toString())
        return node;
    }


/*
    size_t countEdges(ValueType beginNodeLabel,ValueType endNodeLabel){

        Node * beginNode = findNodeByValue(beginNodeLabel);
        Node * endNode = findNodeByValue(endNodeLabel);

        if ( beginNode && endNode ){


        }

        return 0;
    }
*/
    bool hasEdge(ValueType beginNodeLabel,ValueType endNodeLabel) {

        Node * beginNode = findNodeByValue(beginNodeLabel);
        Node * endNode = findNodeByValue(endNodeLabel);

        if ( beginNode && endNode ){

            if ( beginNode->hasEdge(endNode) )
                return true;

          //  return (  gt == Undirected ) && ( endNode->hasEdge(beginNode) );
        }

        return false;
    }

    bool removeEdge(ValueType beginNodeLabel,ValueType endNodeLabel){

        typename std::list<Node*>::iterator it_node = findNode(beginNodeLabel);
        Node *endNode = findNodeByValue(endNodeLabel);


        if ( it_node != nodes.end() && endNode ){

            Node *node = *it_node;
            ITT( QString("Remove edge: %1 - %2").arg(beginNodeLabel.toString()).arg(endNodeLabel.toString()));
            bool result= node->removeEdge(endNode);

            if (  gt == Undirected)
                endNode->removeEdge(node) ;

            return result;

        }
        return false;
    }
    bool removeNode(ValueType label){

        typename std::list<Node*>::iterator it_node = findNode(label);

        if ( it_node != nodes.end() ){

            Node *node = *it_node;

            for( typename std::list<Node*>::iterator it = nodes.begin();it!=nodes.end() ;++it){
               Node *n =(*it);
                n->removeEdge(node);
            }
            ITT( QString("Remove node: %1 ").arg(label.toString()));
            nodes.erase(it_node);
            delete node;
            return true;
        }

        return false;

    }

    std::vector<Edge*> findEdgesByNode(Node *n){

        std::vector<Edge*> result;

        for(typename std::list<Node*>::iterator it = nodes.begin() ;it!=nodes.end(); ++it){

                Node *cn = *it;
                Edge *e = cn->findEdge(n);
                if ( e )
                    result.push_back(e);

        }

        for( size_t i = 0; i < n->edgesCount(); ++i){
            result.push_back(n->at(i));
        }

        return result;

    }

    Node * at(size_t index){
        typename std::list<Node*>::iterator it = nodes.begin();
        std::advance(it,index);

        return *it;
    }
    Node * operator[] (size_t index) {
        typename std::list<Node*>::iterator it = nodes.begin();
        std::advance(it,index);

        return *it;
    }
    size_t nodesCount() const {
        return nodes.size();
    }

    Graph(GraphType gt = Undirected){
        this->gt = gt;
    }
    GraphType graphType() const{
        return gt;
    }

    ~Graph(){
        while( !nodes.empty()){
            Node * node = nodes.back();
            delete node;
            nodes.pop_back();
        }
    }

    Graph* transponation() {
        Graph *result = new Graph;
        Node *nn; //new node
        Node *on; //original node
        Edge *oe;
        Node *en; //endnode
        for ( size_t i = 0 ; i < nodesCount() ;++i){
            on = at(i);
            nn = result->findNodeByValue(on->label());

            if ( !nn ){
                nn = on->copyOnlyNode();
                result->nodes.push_back(nn);
            }

            for( size_t j = 0; j < on->edgesCount();++j){
                oe = on->at(j);
                en = result->findNodeByValue(oe->endNode()->label());

                if ( !en ){
                    en = oe->endNode()->copyOnlyNode();
                    result->nodes.push_back(en);
                }
                //itt történik a transzponálás fordítva "kötöm be"
                en->createEdge(nn);

            }
        }

        return result;

    }

private:

    //gt = graph type ;  undirected esetén sincs duplán tárolva
    GraphType  gt;
    std::list<Node*> nodes;


};

typedef Graph<QString,QVariant> ::Node  GraphNode;
typedef Graph<QString,QVariant> ::Edge  GraphEdge;
}

#endif // GRAPH_H
