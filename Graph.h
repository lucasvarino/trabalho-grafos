#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "Node.h"

class Graph
{
private:
    int order;
    int numberOfEdges;
    Node *firstNode;
    Node *lastNode;
    bool weightedEdges;
    bool weightedNodes;
    bool directed;

public:
    Graph(int order, bool directed, bool weightedEdges, bool weightedNodes);
    ~Graph();

    int getOrder();
    int getNumberOfEdges();
    Node *getFirstNode();
    Node *getLastNode();
    bool isWeightedEdges();
    bool isWeightedNodes();
    bool isDirected();

    void printGraph(string filename);

    void addNode(int id);
    void addEdge(int id, int targetId, float weight);
    Node *searchNode(int id);
    void removeNode(int id);
    void removeEdge(int id, int targetId);
};

#endif // GRAPH_H_INCLUDED