#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "Node.h"
#include <vector>
#include <queue>
#include <chrono>
#include <map>

struct Compare
{
    bool operator()(const std::pair<float, int> &a, const std::pair<float, int> &b)
    {
        // Comparação com base no valor float do par
        return a.first > b.first; // '>' para obter uma min heap
    }
};

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
    Node *firstNode;
    Node *lastNode;
    Node *removedNode;
    bool weightedEdges;
    bool weightedNodes;
    bool directed;
    map<int, Node *> nodeMap;
    vector<pair<float, int>> *candidates;
    int totalOfEdges;
    int uncoveredEdges;

public:
    Graph(int order, bool directed, bool weightedEdges, bool weightedNodes, int totalOfEdges);
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
    void resetMarks();

    bool isKRegular(int k);
    bool isIsolated(); // Grafo sem arestas
    vector<int> getOpenNeighborhood(int id);
    vector<int> getClosedNeighborhood(int id);

    bool isComplete();
    int getNumberOfUnmarkedEdges(Node *node);
    vector<int> getNeighbors(int id);

    vector<int> depthSearch(int id);
    vector<int> directTransitiveClosure(int id);
    

    // Trabalho 2 - Algoritmos Gulosos
    void createCandidates();
    void updateCandidates(int removedNodeId);
    void printRelativeVector();

    Metric relativeHeuristic();
    void printRelativeHeuristic(string filename);


    int randomRange(int min, int max);
    Metric randomizedHeuristic(float alpha, int numIter); // Retorna o conjunto solução usando a heuristica do peso relativo
    void printRandomizedHeuristic(float alphas[], int size, int numIter, string filename);

    float chooseAlpha(vector<float> probabilities, float alphas[]);
    void updateProbabilities(vector<float> *probabilities, vector<int> &bestSolutionVector, float alphas[], int bestWeight, vector<pair<float, int>> avgWeights);
    void updateAvgWeights(vector<pair<float, int>> *avgWeights, float alphas[], float alpha, int auxWeight);
    Metric reativeHeuristic(float alphas[], int numIter); // Imprime o conjunto solução usando a heuristica do peso relativo
    void printReativeHeuristic(float alphas[], int size, int numInter, string filename, string instanceName);
    void imprimeNoEArestas();

    void markNode(Node *node);
};

#endif // GRAPH_H_INCLUDED