#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "Node.h"
#include <vector>
#include <queue>

struct Compare
{
    bool operator()(const std::pair<float, int> &a, const std::pair<float, int> &b)
    {
        // Comparação com base no valor float do par
        return a.first > b.first; // '>' para obter uma min heap
    }
};

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

    void addNode(int id, float weight);
    void addEdge(int id, int targetId, float weight);
    Node *searchNode(int id);
    void removeNode(int id);
    void removeEdge(int id, int targetId);
    void removeAllEdges(int id);

    bool isKRegular(int k);
    bool isIsolated(); // Grafo sem arestas
    vector<int> getOpenNeighborhood(int id);
    vector<int> getClosedNeighborhood(int id);

    bool isComplete();

    vector<int> depthSearch(int id);
    vector<int> directTransitiveClosure(int id);

    // Trabalho 2 - Algoritmos Gulosos
    priority_queue<pair<float, int>, vector<pair<float, int>>, Compare> *relativeWeight(); // Retorna o vértice de menor peso relativo
    vector<int> relativeHeuristc();             // Retorna o conjunto solução usando a heuristica do peso relativo
    vector<int> greedyRandomized(double alfa, int numInter);
    void imprimeNoEArestas(); 
};

#endif // GRAPH_H_INCLUDED