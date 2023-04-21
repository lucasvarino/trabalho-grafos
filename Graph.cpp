#include <iostream>
#include "Graph.h"

using namespace std;

Graph::Graph(int order, bool directed, bool weightedEdges, bool weightedNodes)
{
    this->order = order;
    this->numberOfEdges = 0;
    this->firstNode = nullptr;
    this->lastNode = nullptr;
    this->weightedEdges = weightedEdges;
    this->weightedNodes = weightedNodes;
    this->directed = directed;
}

Graph::~Graph()
{
    // Destruir os nós relacionados ao Graph
    Node *currentNode = this->firstNode;

    while (currentNode != nullptr)
    {
        Node *nextNode = currentNode->getNextNode();
        delete currentNode;
        currentNode = nextNode;
    }
}

int Graph::getOrder()
{
    return this->order;
}

int Graph::getNumberOfEdges()
{
    return this->numberOfEdges;
}

Node *Graph::getFirstNode()
{
    return this->firstNode;
}

Node *Graph::getLastNode()
{
    return this->lastNode;
}

bool Graph::isWeightedEdges()
{
    return this->weightedEdges;
}

bool Graph::isWeightedNodes()
{
    return this->weightedNodes;
}

bool Graph::isDirected()
{
    return this->directed;
}