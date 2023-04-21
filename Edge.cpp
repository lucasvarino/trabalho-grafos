#include <iostream>
#include "Edge.h"

using namespace std;

Edge::Edge(int targetId)
{
    this->targetId = targetId;
    this->nextEdge = nullptr;
    this->weight = 0;
    this->directed = false;
}

Edge::~Edge()
{
    this->nextEdge = nullptr;
}

int Edge::getTargetId()
{
    return this->targetId;
}

Edge *Edge::getNextEdge()
{
    return this->nextEdge;
}

float Edge::getWeight()
{
    return this->weight;
}

bool Edge::isDirected()
{
    return this->directed;
}

void Edge::setNextEdge(Edge *nextEdge)
{
    this->nextEdge = nextEdge;
}

void Edge::setWeight(float weight)
{
    this->weight = weight;
}

void Edge::setDirected(bool directed)
{
    this->directed = directed;
}
