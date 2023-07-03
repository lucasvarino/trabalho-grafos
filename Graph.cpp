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
    if (nodeMap[id] != nullptr)
    {
        return;
    }

    Node *newNode = new Node(id);
    newNode->setWeight(weight);
    nodeMap[newNode->getId()] = newNode;

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
    Node *node = nodeMap[id];
    Node *targetNode = nodeMap[targetId];

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
    Node *node = this->nodeMap[id];

    if (node == nullptr)
    {
        return;
    }

    Node *targetNode = this->nodeMap[targetId];
    node->removeEdge(targetNode);
    targetNode->removeEdge(node);
    node->decrementNumberOfEdges();
    targetNode->decrementNumberOfEdges();
    this->numberOfEdges--;
}

void Graph::removeAllEdges(int id)
{
    Node *node = this->nodeMap[id];

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
    Node *search = nodeMap[id];

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
    Node *node = this->nodeMap[id];

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
                stack.push(this->nodeMap[edge->getTargetId()]);
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

/*
 * Função para verificar se a solução é viável
 * Verifica se todas as arestas tem pelo menos um vértice marcado
 *
 * Retorna true se a solução é viável e false caso contrário
 */

bool Graph::isIsolated()
{
    Node *currentNode = this->firstNode;

    while (currentNode != nullptr)
    {
        Edge *edge = currentNode->getFirstEdge();

        while (edge != nullptr)
        {
            Node *targetNode = nodeMap[edge->getTargetId()];

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

/*
 * Função que conta quantas arestas não marcadas um vértice possui, para fazer o cálculo do peso relativo
 *
 * Retorna o número de arestas não marcadas
 */

int Graph::getNumberOfUnmarkedEdges(Node *node)
{
    Edge *edge = node->getFirstEdge();
    int numberOfUnmarkedEdges = 0;

    while (edge != nullptr)
    {
        Node *targetNode = nodeMap[edge->getTargetId()];
        if (!targetNode->isMarked() && !node->isMarked())
        {
            numberOfUnmarkedEdges++;
        }

        edge = edge->getNextEdge();
    }

    return numberOfUnmarkedEdges;
}

/*
 * Função que calcula o peso relativo de cada vértice
 * Colocamos os vértices em uma max heap, para que o vértice com maior peso relativo sempre esteja no topo
 *
 * Retorna a max heap com os vértices ordenados pelo peso relativo
 */

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

/*
 * Algoritmo Guloso Construtivo
 * Utiliza a heurística do peso relativo para construir uma solução viável
 *
 * Retorna um vetor com os ids dos vértices que compõem a solução
 */
vector<int> Graph::relativeHeuristc()
{
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = chrono::high_resolution_clock::now();

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
        // Coloca o vértice na solução
        Node *node = nodeMap[firstHeuristcNode];
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

        // Atualiza a max heap
        delete minHeap;
        minHeap = this->relativeWeight();
        firstHeuristcNode = minHeap->top().second;
        minHeap->pop();
    }

    end = chrono::high_resolution_clock::now();
    float elapse_time = chrono::duration_cast<chrono::seconds>(end - start).count();

    cout << "Tamanho da solução: " << solutionVector.size() << endl;
    cout << "Peso total da solução: " << totalWeight << endl;
    cout << "Tempo de execução: " << elapse_time / 60 << endl;

    // Reseta os vértices para não marcados para gerar outras soluções
    this->resetMarks();

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

/*
 * Função que gera um número aleatório dentro de um intervalo
 *
 * Recebe como parâmetro o valor mínimo e máximo do intervalo
 * Retorna um número aleatório dentro do intervalo [min, max]
 */

int Graph::randomRange(int min, int max)
{
    std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(gen);
}

/*
 * Algoritmo Guloso Randomizado Construtivo
 *
 * Utiliza a heurística do peso relativo para construir uma solução viável, agora de forma aleatória
 * A cada iteração, o algoritmo escolhe um vértice aleatório dentro de uma faixa de vértices
 *
 * Recebe como parâmetro o valor de alpha, que é o quão aleatório o algoritmo será: 0 é totalmente aleatório e 1 é totalmente guloso
 * Além disso, recebe o número de iterações que o algoritmo irá executar, cada iteração é uma solução diferente
 *
 * Retorna um vetor com os ids dos vértices que compõem a solução e as métricas de tempo e peso
 */

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
        priority_queue<pair<float, int>, vector<pair<float, int>>, Compare> *candidates = this->relativeWeight();

        // Percorre a fila de candidatos até a posição desejada
        int pos = this->randomRange(0, static_cast<int>((candidates->size() - 1) * alpha));

        for (int i = 0; i < pos; i++)
        {
            candidates->pop();
        }
        int firstHeuristcNode = candidates->top().second;

        while (!candidates->empty())
        {
            // Coloca o vértice na solução
            Node *node = this->nodeMap[firstHeuristcNode];
            solution[firstHeuristcNode] = true;
            auxSolutionVector.push_back(firstHeuristcNode);
            auxWeight += node->getWeight();

            // Marca o vértice
            node->setMarked(true);

            // Verifica se a solução é viável
            if (this->isIsolated())
            {
                viable = true;
                break;
            }

            // Atualiza a lista de candidatos
            delete candidates;
            candidates = this->relativeWeight();

            pos = this->randomRange(0, static_cast<int>((candidates->size() - 1) * alpha));

            for (int i = 0; i < pos; i++)
            {
                candidates->pop();
            }
            firstHeuristcNode = candidates->top().second;
        }

        // Reseta os vértices para não marcados para gerar outras soluções
        this->resetMarks();

        if ((i == 1 || auxWeight < bestWeight) && !auxSolutionVector.empty())
        {
            // Se for a primeira solução ou a solução atual for melhor que a melhor solução, atualiza a melhor solução
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
    metric.time = elapse_time / 60;
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
                 << "Tempo (min): " << metric.time << endl
                 << "Peso total: " << metric.totalWeight << endl
                 << "Tamanho da solução: " << metric.numberOfNodes << endl;
            file << "---------------------------" << endl;
        }
    }

    file.close();
}

/*
 * Funnção que retorna o alpha com maior probabilidade de ser escolhido
 *
 * Recebe como parâmetro um vetor de probabilidades e um vetor de alphas
 * Retorna o alpha com maior probabilidade
 */

float Graph::chooseAlpha(vector<float> probabilities, float alphas[])
{
    // get alpha according with prob.
    float alpha;
    float bigger = 0;
    for (int i = 0; i < probabilities.size(); i++)
    {
        if (probabilities[i] > bigger)
        {
            bigger = probabilities[i];
            alpha = alphas[i];
        }
    }
    return alpha;
}

/*
 * Atualiza as probabilidades de cada alpha para ser escolhido na próxima iteração
 *
 * Recebe como parâmetro um vetor de probabilidades, o vetor da melhor solução, os alphas, o peso da melhor solução e um vetor com os pesos médios de cada alpha
 * Usamos como base o slide 27 - aula 10
 */

void Graph::updateProbabilities(vector<float> *probabilities, vector<int> &bestSolutionVector, float alphas[], int bestWeight, vector<pair<float, int>> avgWeights)
{
    // vector of weight ratios
    vector<float> weightRatios;
    float sum = 0, weightRatio;

    // get the weight ratio of each alpha
    for (int i = 0; i < 5; i++)
    {
        if (avgWeights.at(i).first != 0)
        {
            weightRatio = bestWeight / (float)avgWeights.at(i).first;
        }
        else
        {
            weightRatio = 1.0 / 5;
        }
        weightRatios.push_back(weightRatio);
        sum += weightRatio;
    }

    for (int i = 0; i < 5; i++)
    {
        probabilities->at(i) = weightRatios.at(i) / sum;
    }
}

/*
 * Atualiza o vetor de pesos médios de cada alpha
 *
 * Recebe um vetor de pesos médios, um vetor de alphas, o alpha escolhido e o peso da melhor solução
 * Automaticamente atualiza o vetor de pesos médios
 */

void Graph::updateAvgWeights(vector<pair<float, int>> *avgWeights, float alphas[], float alpha, int auxWeight)
{
    if (auxWeight == 0)
        return;

    for (int i = 0; i < 5; i++)
    {
        if (alphas[i] == alpha)
        {
            avgWeights->at(i).second++;

            int quantity = avgWeights->at(i).second;
            float avgWeight = avgWeights->at(i).first;

            avgWeights->at(i).first = (avgWeight * (quantity - 1) + auxWeight) / quantity;
        }
    }
}

/*
 * Algoritmo Guloso Randomizado Reativo
 *
 * O algoritmo escolhe o alpha que tem a maior probabilidade de gerar uma solução viável de forma dinâmica
 * O tamanho do bloco é 10% do número de iterações
 *
 * Recebe um vetor de alphas e o número de iterações
 * Retorna uma struct Metric com o tempo de execução, o peso total e o tamanho da solução
 */

Metric Graph::reativeHeuristic(float alphas[], int numIter)
{

    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = chrono::high_resolution_clock::now();

    // Tamanho do bloco
    int block = numIter * 0.1;

    // Lista de Probabilidades
    vector<float> probabilities(5, 1.0 / 5);

    // Média dos pesos para cada valor de alpha
    vector<pair<float, int>> avgWeights(5, make_pair(0, 0));

    vector<int> auxSolutionVector;
    vector<int> bestSolutionVector;
    map<int, bool> solution;

    int auxWeight = 0, bestWeight = 0, i = 1;
    bool viable = false;

    float alpha;

    while (i <= numIter)
    {

        // Pegar o valor de alpha de acordo com o número de iterações e as probabilidades
        if (i <= 5)
        {
            alpha = alphas[i - 1];
        }
        else
        {
            // Procura o alpha com maior probabilidade
            alpha = chooseAlpha(probabilities, alphas);
        }

        // Para 10% das iterações, atualiza as probabilidades
        if (i % block == 0)
        {
            // Atualizando as probabilidades
            updateProbabilities(&probabilities, bestSolutionVector, alphas, bestWeight, avgWeights);
        }

        // Iniciando o Algortimo Guloso Randomizado Reativo
        for (int i = 1; i < this->order; i++)
        {
            solution.insert(make_pair(i, false));
        }

        // Criando a lista de candidatos ordenados pelo peso relativo
        priority_queue<pair<float, int>, vector<pair<float, int>>, Compare> *candidates = this->relativeWeight();

        while (!candidates->empty())
        {
            // Pegar o primeiro vértice da lista de candidatos
            int firstHeuristcNode = candidates->top().second;

            // Coloca o vértice na solução
            Node *node = this->nodeMap[firstHeuristcNode];
            solution[firstHeuristcNode] = true;
            auxSolutionVector.push_back(firstHeuristcNode);
            auxWeight += node->getWeight();

            // Marca o vértice
            node->setMarked(true);

            // Verifica se a solução é viável
            if (this->isIsolated())
            {
                viable = true;
                break;
            }

            // Atualiza a lista de candidatos
            delete candidates;
            candidates = this->relativeWeight();

            // Pegando o próximo vértice de acordo com o alpha
            int pos = this->randomRange(0, static_cast<int>((candidates->size() - 1) * alpha));

            for (int i = 0; i < pos; i++)
            {
                candidates->pop();
            }
            firstHeuristcNode = candidates->top().second;
        }

        this->resetMarks();

        // Atualizando a média dos pesos
        updateAvgWeights(&avgWeights, alphas, alpha, auxWeight);

        if ((i == 1 || auxWeight < bestWeight) && !auxSolutionVector.empty())
        {
            // Atualizando a melhor solução
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

    int bestAlpha = 0;
    for (int i = 0; i < 5; i++)
    {
        if (avgWeights[i].second > bestAlpha)
        {
            bestAlpha = i;
        }
    }

    Metric metric;
    metric.time = elapse_time;
    metric.totalWeight = bestWeight;
    metric.numberOfNodes = bestSolutionVector.size();
    metric.bestAlpha = bestAlpha;
    return metric;
}

void Graph::printReativeHeuristic(float alphas[], int size, int numInter, string filename)
{
    ofstream file;
    file.open(filename);
    for (int i = 0; i < size; i++)
    {
        file << "=============================" << endl;
        file << "Iteração " << i + 1 << endl;
        file << "---------------------------" << endl;

        Metric metric = this->reativeHeuristic(alphas, numInter);
        file << "Numero de iterações : " << numInter << endl
             << "Tempo (s): " << metric.time << endl
             << "Peso total: " << metric.totalWeight << endl
             << "Melhor alfa: " << alphas[metric.bestAlpha] << endl
             << "Tamanho da solução: " << metric.numberOfNodes << endl;
        file << "---------------------------" << endl;
    }

    file.close();
}
