#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include <iostream>
#include "Edge.h"

using namespace std;

class Node
{
private:
    Edge *firstEdge;
    Edge *lastEdge;
    int id;
    unsigned int inDegree; // Caso o grafo não seja direcionado, inDegree = outDegree
    unsigned int outDegree;
    float weight;
    Node *nextNode;

public:
    Node(int id);
    ~Node();

    Edge *getFirstEdge();
    Edge *getLastEdge();
    int getId();
    unsigned int getInDegree();
    unsigned int getOutDegree();
    float getWeight();
    Node *getNextNode();

    void setNextNode(Node *nextNode);
    void setWeight(float weight);

    Edge *searchEdge(int targetId);

    void addEdge(Node *target, bool directed, float weight);
    void removeEdge(Node *target);
};

#endif // NODE_H_INCLUDED