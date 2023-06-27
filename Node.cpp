#include <iostream>
#include <vector>
#include "Node.h"

using namespace std;

Node::Node(int id)
{
    this->firstEdge = nullptr;
    this->lastEdge = nullptr;
    this->auxFirstEdge = nullptr;
    this->auxLastEdge = nullptr;
    this->id = id;
    this->inDegree = 0;
    this->outDegree = 0;
    this->weight = 0;
    this->nextNode = nullptr;
    this->numberOfEdges = 0;
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

    // Destruir as arestas auxiliares relacionadas ao Node
    currentEdge = this->auxFirstEdge;
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

Edge *Node::getAuxFirstEdge()
{
    return this->auxFirstEdge;
}

Edge *Node::getAuxLastEdge()
{
    return this->auxLastEdge;
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

void Node::setNextNode(Node *nextNode)
{
    this->nextNode = nextNode;
}

void Node::setWeight(float weight)
{
    this->weight = weight;
}

void Node::addEdge(Node *target, bool directed, float weight)
{
    Edge *newEdge = new Edge(target->getId(), weight);

    if (this->firstEdge == nullptr)
    {
        this->firstEdge = newEdge;
        this->lastEdge = newEdge;
    }
    else
    {
        this->lastEdge->setNextEdge(newEdge);
        this->lastEdge = newEdge;
    }

    if (directed)
    {
        newEdge->setDirected(true);
    }

    addAuxEdge(target, directed, weight);
}

void Node::addAuxEdge(Node *target, bool directed, float weight)
{
    Edge *newEdge = new Edge(target->getId(), weight);

    if (this->auxFirstEdge == nullptr)
    {
        this->auxFirstEdge = newEdge;
        this->auxLastEdge = newEdge;
    }
    else
    {
        this->auxLastEdge->setNextEdge(newEdge);
        this->auxLastEdge = newEdge;
    }

    if (directed)
    {
        newEdge->setDirected(true);
    }
}

    Edge *Node::searchEdge(int targetId)
{
    Edge *currentEdge = this->firstEdge;

    while (currentEdge != nullptr)
    {
        if (currentEdge->getTargetId() == targetId)
        {
            return currentEdge;
        }

        currentEdge = currentEdge->getNextEdge();
    }

    return nullptr;
}

void Node::removeEdge(Node *target)
{
    Edge *currentEdge = this->firstEdge;
    Edge *previousEdge = nullptr;

    while (currentEdge != nullptr)
    {
        if (currentEdge->getTargetId() == target->getId())
        {
            if (previousEdge == nullptr)
            {
                this->firstEdge = currentEdge->getNextEdge();
            }
            else
            {
                previousEdge->setNextEdge(currentEdge->getNextEdge());
            }

            if (currentEdge->getNextEdge() == nullptr)
            {
                this->lastEdge = previousEdge;
            }

            if (currentEdge->isDirected())
            {
                this->outDegree--;
                target->inDegree--;
            }
            else
            {
                this->outDegree--;
                this->inDegree--;
                target->outDegree--;
                target->inDegree--;
            }

            delete currentEdge;
            return;
        }

        previousEdge = currentEdge;
        currentEdge = currentEdge->getNextEdge();
    }
    removeAuxEdge(target);
}

void Node::removeAuxEdge(Node *target)
{
    Edge *currentEdge = this->auxFirstEdge;
    Edge *previousEdge = nullptr;

    while (currentEdge != nullptr)
    {
        if (currentEdge->getTargetId() == target->getId())
        {
            if (previousEdge == nullptr)
            {
                this->auxFirstEdge = currentEdge->getNextEdge();
            }
            else
            {
                previousEdge->setNextEdge(currentEdge->getNextEdge());
            }

            if (currentEdge->getNextEdge() == nullptr)
            {
                this->auxLastEdge = previousEdge;
            }

            if (currentEdge->isDirected())
            {
                this->outDegree--;
                target->inDegree--;
            }
            else
            {
                this->outDegree--;
                this->inDegree--;
                target->outDegree--;
                target->inDegree--;
            }

            delete currentEdge;
            return;
        }

        previousEdge = currentEdge;
        currentEdge = currentEdge->getNextEdge();
    }
}

void Node::removeEdge(int id)
{
    Edge *currentEdge = this->firstEdge;
    Edge *previousEdge = nullptr;

    while (currentEdge != nullptr)
    {
        if (currentEdge->getTargetId() == id)
        {
            if (previousEdge == nullptr)
            {
                this->firstEdge = currentEdge->getNextEdge();
            }
            else
            {
                previousEdge->setNextEdge(currentEdge->getNextEdge());
            }

            if (currentEdge->getNextEdge() == nullptr)
            {
                this->lastEdge = previousEdge;
            }

            if (currentEdge->isDirected())
            {
                this->outDegree--;
            }
            else
            {
                this->outDegree--;
                this->inDegree--;
            }

            delete currentEdge;
            return;
        }

        previousEdge = currentEdge;
        currentEdge = currentEdge->getNextEdge();
    }
}

void Node::removeAuxEdge(int id)
{
    Edge *currentEdge = this->auxFirstEdge;
    Edge *previousEdge = nullptr;

    while (currentEdge != nullptr)
    {
        if (currentEdge->getTargetId() == id)
        {
            if (previousEdge == nullptr)
            {
                this->auxFirstEdge = currentEdge->getNextEdge();
            }
            else
            {
                previousEdge->setNextEdge(currentEdge->getNextEdge());
            }

            if (currentEdge->getNextEdge() == nullptr)
            {
                this->auxLastEdge = previousEdge;
            }

            if (currentEdge->isDirected())
            {
                this->outDegree--;
            }
            else
            {
                this->outDegree--;
                this->inDegree--;
            }

            delete currentEdge;
            return;
        }

        previousEdge = currentEdge;
        currentEdge = currentEdge->getNextEdge();
    }
}

void Node::removeAllEdges()
{
    Edge *currentEdge = this->firstEdge;

    while (currentEdge != nullptr)
    {
        Edge *nextEdge = currentEdge->getNextEdge();
        delete currentEdge;
        currentEdge = nextEdge;
    }

    this->firstEdge = nullptr;
    this->lastEdge = nullptr;
    this->inDegree = 0;
    this->outDegree = 0;
}

void Node::removeAllAuxEdges()
{
    Edge *currentEdge = this->auxFirstEdge;

    while (currentEdge != nullptr)
    {
        Edge *nextEdge = currentEdge->getNextEdge();
        delete currentEdge;
        currentEdge = nextEdge;
    }

    this->auxFirstEdge = nullptr;
    this->auxLastEdge = nullptr;
    this->inDegree = 0;
    this->outDegree = 0;
}

void Node::incrementDegree(bool directed)
{
    if (directed)
    {
        this->outDegree++;
    }
    else
    {
        this->outDegree++;
        this->inDegree++;
    }
}

int Node::getNumberOfEdges()
{
    Edge *currentEdge = this->firstEdge;

    while (currentEdge != nullptr)
    {
        this->numberOfEdges++;
        currentEdge = currentEdge->getNextEdge();
    }

    return this->numberOfEdges;
}

int Node::getNumberOfAuxEdges()
{
    Edge *currentEdge = this->auxFirstEdge;

    while (currentEdge != nullptr)
    {
        this->numberOfAuxEdges++;
        currentEdge = currentEdge->getNextEdge();
    }

    return this->numberOfAuxEdges;
}

void Node::incrementNumberOfEdges()
{
    this->numberOfEdges++;
}

void Node::incrementNumberOfAuxEdges()
{
    this->numberOfAuxEdges++;
}

void Node::decrementNumberOfEdges()
{
    this->numberOfEdges--;
}

void Node::decrementNumberOfAuxEdges()
{
    this->numberOfAuxEdges--;
}

bool Node::isMarked()
{
    return this->marked;
}

void Node::setMarked(bool marked)
{
    this->marked = marked;
}

void Node::updateAuxEdges(){
    Edge *currentEdge = this->firstEdge;
    Edge *auxCurrentEdge = this->auxFirstEdge;

    while (currentEdge != nullptr)
    {
        auxCurrentEdge = currentEdge;
        currentEdge = currentEdge->getNextEdge();
        auxCurrentEdge->getNextEdge();
    }

    this->numberOfAuxEdges = this->numberOfEdges;
}