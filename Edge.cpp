#include "Edge.h"
#include <iostream>

using namespace std;

Edge::Edge(int targetId, int originId, float weight)
{
    this->targetId = targetId;
    this->nextEdge = nullptr;
    this->weight = weight;
    this->directed = false;
    this->originId = originId;
    this->marked = false;
}

Edge::~Edge() { this->nextEdge = nullptr; }

int Edge::getTargetId() { return this->targetId; }

Edge *Edge::getNextEdge() { return this->nextEdge; }

float Edge::getWeight() { return this->weight; }

bool Edge::isDirected() { return this->directed; }

void Edge::setNextEdge(Edge *nextEdge) { this->nextEdge = nextEdge; }

void Edge::setWeight(float weight) { this->weight = weight; }

void Edge::setDirected(bool directed) { this->directed = directed; }

int Edge::getOriginId() { return this->originId; }