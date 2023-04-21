#include <iostream>
#include "Node.h"

using namespace std;

Node::Node(int id)
{
    this->firstEdge = nullptr;
    this->lastEdge = nullptr;
    this->id = id;
    this->inDegree = 0;
    this->outDegree = 0;
    this->weight = 0;
    this->nextNode = nullptr;
}

Node::~Node()
{
    // Destruir as arestas relacionadas ao Node
    Edge *currentEdge = this->firstEdge;

    while (currentEdge != nullptr)
    {
        Edge *nextEdge = currentEdge->getNextEdge();
        delete currentEdge;
        currentEdge = nextEdge;
    }
}

Edge *Node::getFirstEdge()
{
    return this->firstEdge;
}

Edge *Node::getLastEdge()
{
    return this->lastEdge;
}

int Node::getId()
{
    return this->id;
}

unsigned int Node::getInDegree()
{
    return this->inDegree;
}

unsigned int Node::getOutDegree()
{
    return this->outDegree;
}

float Node::getWeight()
{
    return this->weight;
}

Node *Node::getNextNode()
{
    return this->nextNode;
}
