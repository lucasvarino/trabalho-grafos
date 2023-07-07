#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "Node.h"
#include <vector>
#include <queue>
#include <chrono>
#include <map>

/**
 * Struct para comparar valores com base no valor float do par para se obter uma min heap
*/
struct Compare
{
    bool operator()(const std::pair<float, int> &a, const std::pair<float, int> &b)
    {
        // Comparação com base no valor float do par
        return a.first > b.first; // '>' para obter uma min heap
    }
};

/*
 * Struct para gerar métricas dos algoritmos e auxiliar na criação do relatório
*/
struct Metric
{
    float time;
    int numberOfNodes;
    int totalWeight;
    int bestAlpha;
};

class Graph
{
private:
    int order;
    int numberOfEdges;
    int totalOfEdges;
    int uncoveredEdges;

    Node *firstNode;
    Node *lastNode;
    Node *removedNode;
    bool weightedEdges;
    bool weightedNodes;

    bool directed;
    map<int, Node *> nodeMap;
    map<int, vector<int>> openNeighborhoodMap;
    vector<pair<float, int>> *candidates;

public:
    Graph(int order, bool directed, bool weightedEdges, bool weightedNodes, int totalOfEdges);
    ~Graph();

    //métodos relacionados a informações do grafo
    int getOrder();
    int getNumberOfEdges();
    int getGraphDegree();
    vector<int> getDegreeSequence();
    Node *getFirstNode();
    Node *getLastNode();
    pair<int, int> getNodeDegree(int id);

    //métodos relacionados a caracteristicas do grafo
    bool isWeightedEdges();
    bool isWeightedNodes();
    bool isDirected();
    bool isTrivialGraph();
    bool isNullGraph();
    bool isMultigraph();
    bool isBipartite();
    bool isEuclerian();
    bool isComplete();


    void printGraph(string filename);
    void printGraphProperties();
    Graph* getInducedSubgraph(vector<int> nodes);
    Graph* getComplementGraph();

    //métodos relacionados a manipulação do grafo
    void addNode(int id, float weight);
    void addEdge(int id, int targetId, float weight);
    Node *searchNode(int id);
    void removeNode(int id);
    void removeEdge(int id, int targetId);
    void removeAllEdges(int id);
    void resetMarks();

    bool isKRegular(int k);
    bool isIsolated(); // Grafo sem arestas
    vector<int> getOpenNeighborhood(int id);
    vector<int> getClosedNeighborhood(int id);
    vector<int> indirectTransitiveClosure(int id);
    void auxIndirectTransitiveClosure(int id, vector<int>& visited);

    int getNumberOfUnmarkedEdges(Node *node);
    vector<int> getNeighbors(int id);

    vector<int> depthSearch(int id);
    vector<int> directTransitiveClosure(int id);

    // Trabalho 2 - Algoritmos Gulosos
    //Métodos relacionados a lista de candidatos
    void createCandidates();
    void updateCandidates(int removedNodeId);

    void printRelativeVector();

    void imprimeNoEArestas();

    void markNode(Node *node);

    //Métodos relacionados a heurística do peso relativo
    Metric relativeHeuristic();
    Metric randomizedHeuristic(float alpha, int numIter); // Retorna o conjunto solução usando a heuristica do peso relativo
    Metric reativeHeuristic(float alphas[], int numIter); // Imprime o conjunto solução usando a heuristica do peso relativo

    //metodos utilizados na heuristica
    int randomRange(int min, int max);
    float chooseAlpha(vector<float> probabilities, float alphas[]);
    void updateProbabilities(vector<float> *probabilities, vector<int> &bestSolutionVector, float alphas[], int bestWeight, vector<pair<float, int>> avgWeights);
    void updateAvgWeights(vector<pair<float, int>> *avgWeights, float alphas[], float alpha, int auxWeight);
    
    //prints
    void printRelativeHeuristic(string filename, string instanceName);
    void printRandomizedHeuristic(float alphas[], int size, int numIter, string filename, string instanceName);
    void printReativeHeuristic(float alphas[], int size, int numInter, string filename, string instanceName);
    
    void createNeighborhoodMap()
    {
        Node *aux = firstNode;
        while (aux != nullptr)
        {
            vector<int> neighborhood = getOpenNeighborhood(aux->getId());
            openNeighborhoodMap.insert(pair<int, vector<int>>(aux->getId(), neighborhood));
            aux = aux->getNextNode();
        }
    }
};

#endif // GRAPH_H_INCLUDED