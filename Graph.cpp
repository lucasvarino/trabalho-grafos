#include "Graph.h"
#include <fstream>
#include <iostream>

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

int Graph::getOrder() { return this->order; }

int Graph::getNumberOfEdges() { return this->numberOfEdges; }

Node *Graph::getFirstNode() { return this->firstNode; }

Node *Graph::getLastNode() { return this->lastNode; }

bool Graph::isWeightedEdges() { return this->weightedEdges; }

bool Graph::isWeightedNodes() { return this->weightedNodes; }

bool Graph::isDirected() { return this->directed; }

void Graph::printGraph(string filename)
{
    // Função para imprimir o grafo em formato .dot

    string graphType = this->directed ? "digraph" : "graph";

    ofstream file(filename, ios::out | ios::trunc);

    if (!file.is_open())
    {
        cout << "Erro ao abrir o arquivo " << filename << endl;
        return;
    }

    file << graphType << " G {" << endl;

    Node *currentNode = this->firstNode;

    while (currentNode != nullptr)
    {
        file << currentNode->getId() << ";" << endl;
        currentNode = currentNode->getNextNode();
    }

    file << endl;

    currentNode = this->firstNode;

    if (this->weightedEdges)
    {
        while (currentNode != nullptr)
        {
            Edge *currentEdge = currentNode->getFirstEdge();

            while (currentEdge != nullptr)
            {
                file << currentNode->getId() << " -- " << currentEdge->getTargetId()
                     << " [label=" << currentEdge->getWeight() << "]" << endl;
                currentEdge = currentEdge->getNextEdge();
            }

            currentNode = currentNode->getNextNode();
        }
    }
    else
    {
        while (currentNode != nullptr)
        {
            Edge *currentEdge = currentNode->getFirstEdge();

            while (currentEdge != nullptr)
            {
                file << currentNode->getId() << " -- " << currentEdge->getTargetId()
                     << endl;
                currentEdge = currentEdge->getNextEdge();
            }

            currentNode = currentNode->getNextNode();
        }
    }

    file << "}" << endl;

    file.close();

    cout << "O grafo foi salvo no arquivo .dot" << endl;
}

Node *Graph::searchNode(int id)
{
    Node *currentNode = this->firstNode;

    while (currentNode != nullptr)
    {
        if (currentNode->getId() == id)
        {
            return currentNode;
        }

        currentNode = currentNode->getNextNode();
    }

    return nullptr;
}

void Graph::addNode(int id)
{
    if (this->searchNode(id) != nullptr)
    {
        return;
    }

    Node *newNode = new Node(id);

    if (this->firstNode == nullptr)
    {
        this->firstNode = newNode;
        this->lastNode = newNode;
    }
    else
    {
        this->lastNode->setNextNode(newNode);
        this->lastNode = newNode;
    }

    this->order++;
}

void Graph::addEdge(int id, int targetId, float weight)
{
    Node *node = this->searchNode(id);
    Node *targetNode = this->searchNode(targetId);

    if (node == nullptr || targetNode == nullptr)
    {
        cout << "Nó não encontrado" << endl;
        return;
    }

    node->addEdge(targetNode, this->directed, weight);

    if (!this->directed)
    {
        targetNode->addEdge(node, this->directed, weight);
    }
    this->numberOfEdges++;
}

void Graph::removeNode(int id)
{
    Node *currentNode = this->firstNode;
    Node *previousNode = nullptr;

    while (currentNode != nullptr)
    {

        currentNode->removeEdge(id);
        cout << "Removendo aresta do nó " << currentNode->getId() << " para o nó " << id << endl;
        if (currentNode->getId() == id)
        {
            if (previousNode == nullptr)
            {
                this->firstNode = currentNode->getNextNode();
            }
            else
            {
                previousNode->setNextNode(currentNode->getNextNode());
            }
        }
        previousNode = currentNode;
        currentNode = currentNode->getNextNode();
    }

    delete currentNode;
    this->order--;
    return;
}

void Graph::removeEdge(int id, int targetId)
{
    Node *node = this->searchNode(id);

    if (node == nullptr)
    {
        return;
    }

    node->removeEdge(this->searchNode(targetId));
    this->numberOfEdges--;
}

bool Graph::isKRegular(int k)
{
    Node *currentNode = this->firstNode;

    while (currentNode != nullptr)
    {
        if (this->isDirected())
        {
            if (currentNode->getInDegree() != k || currentNode->getOutDegree() != k)
            {
                return false;
            }
        }
        else if (currentNode->getOutDegree() != k)
        {
            return false;
        }

        currentNode = currentNode->getNextNode();
    }

    return true;
}

vector<int> Graph::getOpenNeighborhood(int id)
{
    Node *search = this->searchNode(id);

    if (search == nullptr)
    {
        return vector<int>();
    }

    Node *node = this->firstNode;
    vector<int> neighborhood;

    while (node != nullptr)
    {
        Edge *edge = node->searchEdge(id);

        if (edge != nullptr)
        {
            neighborhood.push_back(node->getId());
        }

        node = node->getNextNode();
    }

    return neighborhood;
}

vector<int> Graph::getClosedNeighborhood(int id)
{
    vector<int> neighborhood = this->getOpenNeighborhood(id);
    neighborhood.push_back(id);

    return neighborhood;
}
