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
        return a.first > b.first;
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

    bool weightedEdges;
    bool weightedNodes;
    bool directed;

    Node *firstNode;
    Node *lastNode;
    Node *removedNode;

    map<int, Node *> nodeMap;
    vector<pair<float, int>> *candidates;


public:
    //construtor/destrutor
    Graph(int order, bool directed, bool weightedEdges, bool weightedNodes, int totalOfEdges);
    ~Graph();

    //métodos relacionados a informações do grafo
    int getOrder();
    
    int getNumberOfEdges();
    bool isWeightedEdges();
    bool isWeightedNodes();
    bool isDirected();

    // metódos relacionados a manipulação de nó
    Node *getFirstNode();
    Node *getLastNode();
    void addNode(int id, float weight);
    void removeNode(int id);
    Node *searchNode(int id);
    pair<int, int> getNodeDegree(int id);
    
    //métodos relacionados a manipulação de arestas
    void addEdge(int id, int targetId, float weight);
    void removeEdge(int id, int targetId);
    void removeAllEdges(int id);
    int getNumberOfUnmarkedEdges(Node *node);

    void resetMarks();

    //caracteristicas do grafo
    bool isComplete();
    bool isKRegular(int k);
    bool isIsolated(); // Grafo sem arestas
    bool isTrivialGraph();
    bool isNullGraph();
    bool isMultigraph();

    //vizinhaça aberta e fechada 
    vector<int> getOpenNeighborhood(int id);
    vector<int> getClosedNeighborhood(int id);
    vector<int> getNeighbors(int id);

    //busca em profundidade e feixe transitivo direto
    vector<int> depthSearch(int id);
    vector<int> directTransitiveClosure(int id);
    
    //print
    void printGraph(string filename);

    // Trabalho 2 - Algoritmos Gulosos

    //Métodos relacionados a lista de candidatos
    void createCandidates();
    void updateCandidates(int removedNodeId);

    void printRelativeVector();

    void imprimeNoEArestas();

    void markNode(Node *node);

    //heuristicas
    Metric relativeHeuristic();
    Metric randomizedHeuristic(float alpha, int numIter); // Retorna o conjunto solução usando a heuristica do peso relativo
    Metric reativeHeuristic(float alphas[], int numIter); // Imprime o conjunto solução usando a heuristica do peso relativo

    //métodos para heuristica do algoritmo randomizado reativo
    int randomRange(int min, int max);
    float chooseAlpha(vector<float> probabilities, float alphas[]);
    void updateProbabilities(vector<float> *probabilities, vector<int> &bestSolutionVector, float alphas[], int bestWeight, vector<pair<float, int>> avgWeights);
    void updateAvgWeights(vector<pair<float, int>> *avgWeights, float alphas[], float alpha, int auxWeight);

    //métodos de prints dos resultados
    void printRelativeHeuristic(string filename);
    void printRandomizedHeuristic(float alphas[], int size, int numIter, string filename);
    void printReativeHeuristic(float alphas[], int size, int numInter, string filename, string instanceName);

};

#endif // GRAPH_H_INCLUDED