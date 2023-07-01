#include "Graph.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <map>
#include <queue>
#include <cmath>
#include <random>

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

void Graph::addNode(int id, float weight)
{
    if (this->searchNode(id) != nullptr)
    {
        return;
    }

    Node *newNode = new Node(id);
    newNode->setWeight(weight);

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

    if (node->searchEdge(targetId) != nullptr)
    {
        return;
    }
    node->addEdge(targetNode, this->directed, weight);
    node->incrementDegree(this->directed);
    node->incrementNumberOfEdges();

    if (!this->directed)
    {
        if (targetNode->searchEdge(id) == nullptr)
        {
            targetNode->addEdge(node, this->directed, weight);
            targetNode->incrementDegree(this->directed);
        }
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

    Node *targetNode = this->searchNode(targetId);
    node->removeEdge(targetNode);
    targetNode->removeEdge(node);
    node->decrementNumberOfEdges();
    targetNode->decrementNumberOfEdges();
    this->numberOfEdges--;
}

void Graph::removeAllEdges(int id)
{
    Node *node = this->searchNode(id);

    if (node == nullptr)
    {
        return;
    }

    Edge *currentEdge = node->getFirstEdge();
    Edge *aux = nullptr;

    while (currentEdge != nullptr)
    {
        aux = currentEdge;
        currentEdge = currentEdge->getNextEdge();
        this->removeEdge(id, aux->getTargetId());
    }
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

bool Graph::isComplete()
{
    // Verificar se todos os vértices tem grau n - 1 e se o número de arestas é n(n-1)/2

    int n = this->getOrder();

    Node *currentNode = this->getFirstNode();

    while (currentNode != nullptr)
    {
        if (this->isDirected())
        {
            if (currentNode->getInDegree() != n - 1 || currentNode->getOutDegree() != n - 1)
            {
                return false;
            }
        }
        else if (currentNode->getInDegree() != n - 1)
        {
            return false;
        }

        currentNode = currentNode->getNextNode();
    }

    return true;
}

vector<int> Graph::depthSearch(int id)
{
    Node *node = this->searchNode(id);

    if (node == nullptr)
    {
        return vector<int>();
    }
    vector<int> visited;
    stack<Node *> stack;
    stack.push(node);

    while (!stack.empty())
    {
        node = stack.top();
        stack.pop();

        if (find(visited.begin(), visited.end(), node->getId()) == visited.end())
        {
            visited.push_back(node->getId());

            Edge *edge = node->getFirstEdge();

            while (edge != nullptr)
            {
                stack.push(this->searchNode(edge->getTargetId()));
                edge = edge->getNextEdge();
            }
        }
    }

    return visited;
}

vector<int> Graph::directTransitiveClosure(int id)
{
    return this->depthSearch(id);
}

void Graph::resetMarks()
{
    Node *node = this->firstNode;

    while (node != nullptr)
    {
        node->setMarked(false);
        node = node->getNextNode();
    }
}

bool Graph::isIsolated()
{
    Node *currentNode = this->firstNode;

    while (currentNode != nullptr)
    {
        Edge *edge = currentNode->getFirstEdge();

        while (edge != nullptr)
        {
            Node *targetNode = this->searchNode(edge->getTargetId());

            if (!targetNode->isMarked() && !currentNode->isMarked())
            {
                return false;
            }

            edge = edge->getNextEdge();
        }

        currentNode = currentNode->getNextNode();
    }

    return true;
}

int Graph::getNumberOfUnmarkedEdges(Node *node)
{
    Edge *edge = node->getFirstEdge();
    int numberOfUnmarkedEdges = 0;

    while (edge != nullptr)
    {
        Node *targetNode = this->searchNode(edge->getTargetId());
        if (!targetNode->isMarked() && !node->isMarked())
        {
            numberOfUnmarkedEdges++;
        }

        edge = edge->getNextEdge();
    }

    return numberOfUnmarkedEdges;
}

priority_queue<pair<float, int>, vector<pair<float, int>>, Compare> *Graph::relativeWeight()
{
    // Retorna o id do vértice com menor peso relativo
    // O peso relativo é calculado pelo peso do vértice dividido pelo número de arestas que ele possui

    Node *currentNode = this->firstNode;
    Node *relativeWeightNode = currentNode;

    // Inicializa a min heap, utilizar para sempre ter o menor peso relativo no topo
    priority_queue<pair<float, int>, vector<pair<float, int>>, Compare> minHeap;

    while (currentNode != nullptr)
    {
        if (currentNode->isMarked())
        {
            currentNode = currentNode->getNextNode();
            continue;
        }

        // int relative = ceil(currentNode->getWeight() / this->getNumberOfUnmarkedEdges(currentNode));

        minHeap.push(make_pair(currentNode->getWeight() / this->getNumberOfUnmarkedEdges(currentNode), currentNode->getId()));
        currentNode = currentNode->getNextNode();
    }

    priority_queue<pair<float, int>, vector<pair<float, int>>, Compare> *minHeapPtr = new priority_queue<pair<float, int>, vector<pair<float, int>>, Compare>(minHeap);
    return minHeapPtr;
}

vector<int> Graph::relativeHeuristc()
{
    // Conjunto solução inicial
    map<int, bool> solution;
    vector<int> solutionVector;

    for (int i = 0; i < this->getOrder(); i++)
    {
        solution.insert(make_pair(i, false));
    }

    priority_queue<pair<float, int>, vector<pair<float, int>>, Compare> *minHeap = this->relativeWeight();

    bool viable = false;
    int firstHeuristcNode = minHeap->top().second;
    float totalWeight = 0;

    minHeap->pop();

    while (!minHeap->empty())
    {
        // this->imprimeNoEArestas();
        // Coloca o vértice na solução
        Node *node = this->searchNode(firstHeuristcNode);
        solution[firstHeuristcNode] = true;
        solutionVector.push_back(firstHeuristcNode);
        totalWeight += node->getWeight();

        // Marca o vértice
        node->setMarked(true);

        // Verifica se a solução é viável
        if (this->isIsolated())
        {
            viable = true;
            break;
        }

        // Atualiza a min heap
        delete minHeap;
        minHeap = this->relativeWeight();
        firstHeuristcNode = minHeap->top().second;
        minHeap->pop();
    }

    cout << "Tamanho da solução: " << solutionVector.size() << endl;
    cout << "Peso total da solução: " << totalWeight << endl;
    this->resetMarks();
    // cout << "Conjunto solução: " << endl;

    // for (int i = 0; i < solutionVector.size(); i++)
    // {
    //     cout << solutionVector[i] << " ";
    // }

    if (!viable)
    {
        return vector<int>();
    }
    return solutionVector;
}

void Graph::imprimeNoEArestas()
{
    cout << "=============================" << endl;
    cout << "Imprimindo nós e arestas" << endl;
    cout << "=============================" << endl;
    cout << endl;
    Node *currentNode = this->firstNode;

    while (currentNode != nullptr)
    {
        cout << "Nó: " << currentNode->getId() << endl;
        Edge *currentEdge = currentNode->getFirstEdge();

        while (currentEdge != nullptr)
        {
            currentEdge = currentEdge->getNextEdge();
        }

        currentNode = currentNode->getNextNode();
    }
}

int Graph::randomRange(int min, int max)
{
    std::mt19937 gen(1220);
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(gen);
}

vector<pair<float, int>> Graph::relativeWeightVector()
{
    Node *currentNode = this->firstNode;
    Node *relativeWeightNode = currentNode;

    vector<pair<float, int>> sortedVector;

    while (currentNode != nullptr)
    {
        if (currentNode->isMarked())
        {
            currentNode = currentNode->getNextNode();
            continue;
        }

        sortedVector.push_back(make_pair(currentNode->getWeight() / this->getNumberOfUnmarkedEdges(currentNode), currentNode->getId()));
        std::sort(sortedVector.begin(), sortedVector.end());

        currentNode = currentNode->getNextNode();
    }

    return sortedVector;
}

Metric Graph::randomizedHeuristic(float alpha, int numInter)
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = chrono::high_resolution_clock::now();

    vector<int> auxSolutionVector;
    vector<int> bestSolutionVector;
    map<int, bool> solution;

    int auxWeight = 0, bestWeight = 0, i = 1;
    bool viable = false;

    while (i <= numInter)
    {
        for (int i = 0; i < this->getOrder(); i++)
        {
            solution.insert(make_pair(i, false));
        }

        vector<pair<float, int>> candidates = this->relativeWeightVector();

        int pos = this->randomRange(0, static_cast<int>((candidates.size() - 1) * alpha));

        candidates.erase(candidates.begin(), candidates.begin() + pos);
        int firstHeuristcNode = candidates.begin()->second;

        while (!candidates.empty())
        {
            Node *node = this->searchNode(firstHeuristcNode);
            solution[firstHeuristcNode] = true;
            auxSolutionVector.push_back(firstHeuristcNode);
            auxWeight += node->getWeight();

            node->setMarked(true);

            if (this->isIsolated())
            {
                viable = true;
                break;
            }

            candidates = this->relativeWeightVector();

            pos = this->randomRange(0, static_cast<int>((candidates.size() - 1) * alpha));

            candidates.erase(candidates.begin(), candidates.begin() + pos);
            firstHeuristcNode = candidates.begin()->second;
        }

        this->resetMarks();

        if ((i == 1 || auxWeight < bestWeight) && !auxSolutionVector.empty())
        {
            bestSolutionVector = auxSolutionVector;
            bestWeight = auxWeight;
        }

        solution.clear();
        auxSolutionVector.clear();
        auxWeight = 0;
        i++;
    }

    end = chrono::high_resolution_clock::now();
    float elapse_time = chrono::duration_cast<chrono::seconds>(end - start).count();

    Metric metric;
    metric.time = elapse_time;
    metric.totalWeight = bestWeight;
    metric.numberOfNodes = bestSolutionVector.size();
    return metric;
}

void Graph::printRandomizedHeuristic(float alphas[], int size, int numInter, string filename)
{
    ofstream file;
    file.open(filename);
    for (int i = 0; i < size; i++)
    {
        file << "=============================" << endl;
        file << "Iteração " << i + 1 << endl;
        file << "---------------------------" << endl;
        for (int i = 0; i < 5; i++)
        {
            Metric metric = this->randomizedHeuristic(alphas[i], numInter);
            file << "Alfa = " << alphas[i] << " para " << numInter << " iterações" << endl
                 << "Tempo (s): " << metric.time << endl
                 << "Peso total: " << metric.totalWeight << endl
                 << "Tamanho da solução: " << metric.numberOfNodes << endl;
            file << "---------------------------" << endl;
        }
    }

    file.close();
}