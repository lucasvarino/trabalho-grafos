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

Graph::Graph(int order, bool directed, bool weightedEdges, bool weightedNodes, int totalOfEdges)
{
    this->order = order;
    this->numberOfEdges = 0;
    this->firstNode = nullptr;
    this->lastNode = nullptr;
    this->weightedEdges = weightedEdges;
    this->weightedNodes = weightedNodes;
    this->directed = directed;
    this->totalOfEdges = totalOfEdges;
    this->uncoveredEdges = totalOfEdges;
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
                if (currentEdge->getTargetId() > currentNode->getId())
                {
                    file << currentNode->getId() << " -- " << currentEdge->getTargetId()
                         << endl;
                }
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


/*
Método responsável por encontrar um nó no grafo com base no id do nó
*/

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

/*
Método responsável por adicionar um nó no grafo
*/
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

/*
Método responsável por adicionar uma aresta no grafo
*/
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

/*
Método responsável remover um nó do grafo
*/
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

/*
Método responsável por remover uma aresta do grafo
*/
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

/*
Método responsável por remover todos as arestas do grafo
*/
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

/*
Método responsável por verificar se um grafo é k-regular
*/
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

/*
Método que retorna a vizinhaça aberta de um nó
*/
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

/*
Método que retorn a vizinhaça fechada de um no
*/
vector<int> Graph::getClosedNeighborhood(int id)
{
    vector<int> neighborhood = this->getOpenNeighborhood(id);
    neighborhood.push_back(id);

    return neighborhood;
}

/*
Método responsável por verificar se um grafo é completo
*/
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

/*
Busca em profundidade com base no id de um nó
*/
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

/*
Nessa função é percorrido os nós do grafo e verificar se cada nó está presente no conjunto do fecho transitivo direto,
se o nó nao estiver presente, podemos adiciona-lo ao fecho transitivo indireto.
*/
vector<int> Graph::indirectTransitiveClosure(int id)
{ 
    vector<int> closure;
    vector<int> visited(this->getOrder() + 1, -1);

    auxIndirectTransitiveClosure(id, visited);

        // Retorna o fecho transitivo indireto como um vetor
    for (int i = 1; i <= order; i++)
    {
        if (visited[i] != -1)
            closure.push_back(visited[i]);
    }

    return closure;
}

void Graph::auxIndirectTransitiveClosure(int id, vector<int>& visited)
{
    Node* startNode = searchNode(id);

    if(startNode != nullptr){
        // Busca por qualquer nó que possua uma aresta apontando para o nó atual
        for (Node* aux = firstNode; aux != nullptr; aux = aux->getNextNode())
        {
            if (visited[aux->getId()] == -1 && aux->searchEdge(id))
            {
                visited[aux->getId()] = aux->getId();
                auxIndirectTransitiveClosure(aux->getId(), visited);
            }
        }
    }    
}

/*
Nessa função é percorrido os nós do grafo e adicionado cada grau ao vetor, ao final é ordenado de forma decrescente 
e a função retorna a sequencia de graus em um vetor int;
*/
vector<int> Graph::getDegreeSequence()
{
    vector<int> degreeSequence;
    for(Node* current = this->getFirstNode(); current != nullptr; current = current->getNextNode()){
        int degree = current->getInDegree();
        degreeSequence.push_back(degree);
    }

    //Ordena em ordem não crescente
    sort(degreeSequence.rbegin(), degreeSequence.rend());
    return degreeSequence;
}

/*
Método que remove todas marcações dos nós do grafo
*/
void Graph::resetMarks()
{
    int count = 0;
    Node *node = this->firstNode;

    while (node != nullptr)
    {
        node->setMarked(false);

        Edge *edge = node->getFirstEdge();
        while (edge != nullptr)
        {
            count++;
            edge->setMarked(false);
            edge = edge->getNextEdge();
        }
        node->setNumberOfUnmarkedEdges(count);
        count = 0;
        node = node->getNextNode();
    }

    this->uncoveredEdges = this->numberOfEdges;
}

/*
 * Função para marcar um vértice e suas arestas
 *
 * Recebe um ponteiro para o vértice a ser marcado
 * Marca o vértice e todas as arestas que saem dele
 */

void Graph::markNode(Node *node)
{
    node->setMarked(true);
    node->setNumberOfUnmarkedEdges(0);
    const vector<int> &neighbors = this->openNeighborhoodMap[node->getId()];

    int size = neighbors.size();
    int i = 0;
    for (int neighbor : neighbors)
    {
        Node *neighborNode = this->nodeMap[neighbor];
        if (!neighborNode->isMarked())
        {
            neighborNode->decrementUnmarkedEdges();
            this->uncoveredEdges--;
        }
    }
}

/*
Obtém os vizinhos de um nó, recebe o id do nó como parametro e retorna um vetor com os vizinhos do nó
*/
vector<int> Graph::getNeighbors(int id)
{
    Node *node = nodeMap[id];

    if (node == nullptr)
    {
        return vector<int>();
    }

    vector<int> neighbors;
    Edge *edge = node->getFirstEdge();

    while (edge != nullptr)
    {
        neighbors.push_back(edge->getTargetId());
        edge = edge->getNextEdge();
    }

    return neighbors;
}

/*
 * Função para verificar se a solução é viável
 * Verifica se todas as arestas tem pelo menos um vértice marcado
 *
 * Retorna true se a solução é viável e false caso contrário
 */

bool Graph::isIsolated()
{
    return this->uncoveredEdges == 0;
}

/*
 * Função para verificar se o grafo é trivial
 * Verifica se o numero de vertices é igual a 1, se o primeiro nó nao é nulo, se o primeiro nó é igual ao ultimo nó
 * e se o grau do primeiro nó é igual a zero
 *
 * Retorna true se todas verificações forem true e false caso alguma não seja.
 */
bool Graph::isTrivialGraph()
{
    return (this->getOrder() == 1 &&
                this->getFirstNode() != nullptr &&
                this->getFirstNode() == this->getLastNode() &&
                this->getFirstNode()->getInDegree() == 0 );
}


 /*
 * Função para verificar o grau de um determinado nó do grafo
 * Recebe como parametro o id do nó e retorna um par de grau de entrada e de saída do grafo
 * caso o nó nao exista retorna um par -1, -1.
 */
pair<int, int> Graph::getNodeDegree(int id)
{
    Node* node = this->searchNode(id);
    if (node == nullptr){
        return make_pair(-1,-1);
    }

    return make_pair(node->getInDegree(), node->getOutDegree());
}

/*
* Função para verificar se o grafo é nulo
* percorre todos os nós e verifica se existe uma aresta adjacente a ele, caso exista alguma retorna false.
* caso contrário retorna true
*/
bool Graph::isNullGraph() {
    for(Node *current = this->getFirstNode(); current != nullptr; current = current->getNextNode()){
        if(current->getFirstEdge() != nullptr)
            return false;
    }

    return true;
}

/*
* Função para verificar se o grafo é multigrafo
* percorre todos os nós e para cada nó conta o numero de arestas para cada nó adjacente usando um map.
 *se for encontrado algum par de nó com mais de uma aresta significa que é multigrafo e retorna true
* caso contrário retorna false
*/
bool Graph::isMultigraph()  {
    for(Node *current = this->getFirstNode(); current != nullptr; current = current->getNextNode()){
        map<int, int> edgeCount; //mapeia o id do nó adjacente para o número de arestas.

        for(Edge *edge = current->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
            int targetId = edge->getTargetId();
            edgeCount[targetId]++;
        }

        for(auto& pair : edgeCount){
            if(pair.second > 1){
                //O grafo possui multiplas arestas entre os mesmos nós
                return true;
            }
        }
    }

    return false;
}

/*
* Função para verificar se o grafo é bipartite
* Usa uma abordagem de busca em largura para percorrer o grafo atribuindo cores aos nós que sao visitados
 *se durante a travessia for encontrado um nó adjacente com a mesma cor que o nó atual o grafo nao é bipartido e retorna false
* caso contrário retorna false, se todos os nós puderem ser coloridos sem que haja nós adjacentes com a mesmo cor, o grafo é bipartido
* e retorna true;
*/
bool Graph::isBipartite()  {
    if(this->getOrder() == 0){
        //grafo vazio, considerado bipartido
        return true;
    }

    map<int, bool> visited; //mapa para rastrear
    map<int, bool> colors; //mapa para atribuir cores aos nós

    //inicializa os mapas de visitados e cores
    for(Node* current = this->getFirstNode(); current != nullptr; current = current->getNextNode()){
        visited[current->getId()] = false;
        colors[current->getId()] = false;
    }

    //percorre todos os nós do grafo
    for(Node* current = this->getFirstNode(); current != nullptr; current = current->getNextNode()){
        //verifica apenas se o nó atual ainda nao foi visitado
        if(!visited[current->getId()]){
            queue<Node *> queue;
            queue.push(current);
            visited[current->getId()] = true;
            colors[current->getId()] = true;

            //executa a busca em largura
            while(!queue.empty()){
                Node * node = queue.front();
                queue.pop();

                //percorre todas as arestas do nó atual
                for(Edge* edge = node->getFirstEdge(); edge != nullptr; edge = edge->getNextEdge()){
                    int adjacentNodeId = edge->getTargetId();
                    Node* adjacentNode = this->searchNode(adjacentNodeId);

                    //verifica se o nó adjacente nao foi visitado
                    if(!visited[adjacentNode->getId()]){
                        visited[adjacentNode->getId()] = true;
                        colors[adjacentNode->getId()] = !colors[node->getId()]; //atribui a cor oposta ao nó adjacente
                        queue.push(adjacentNode);
                    }
                    //se o nó adjacente ja foi visitado e possui a mesma cor que o nó atual o grafo nao é bipartido
                    else if(colors[adjacentNode->getId()]  ==  colors[node->getId()])
                        return false;
                }
            }
        }
    }
    //se todas as travessias com busca em largura nao encontraram nós com cores iguais entao o grafo é bipartido
    return true;
}

/*
 * Função que percorre todos os nós do grafo e a cada nó com grau maior ele atualiza o maxDegree
 *
 * Retorna o grau do grafo
 */

int Graph::getGraphDegree()
{
    int maxDegree = 0;
    for(Node* current = this->getFirstNode(); current != nullptr; current = current->getNextNode()){
        int degree = max(current->getInDegree(), current->getOutDegree());

        if(degree > maxDegree){
            maxDegree = degree;
        }
    }

    return maxDegree;
}

/*
A função a seguir cria um novo objeto Grafo que representa o subgrafo vertice induzido
pelos vértices fornecidos. Ele percorre os vértices informados, adicionando ao subgrafo e 
adiciona as arestas pertencentes ao conjunto 

A função retorna um objeto do tipo Graph, que contém o subgrafo induzido pelo conjunto de vértices fornecidos
*/
Graph* Graph::getInducedSubgraph(vector<int> nodes)
{
    Graph *subgraph = new Graph(0, false, true, false, 0);

    for(int id: nodes){
        Node* node = searchNode(id);
        if(!node){
            cout << "ERROR : node not found ! " << endl;
            return nullptr;
        }

        subgraph->addNode(id, node->getWeight());
    }

    for(int id: nodes){
        //obtém os vizinhos do nó
        vector <int> neighbors = this->getOpenNeighborhood(id);
        for(int neighbor : neighbors){
            auto it = find(nodes.begin(), nodes.end(), neighbor);
            if(it != nodes.end()){
                subgraph->addEdge(id, neighbor, 0);
            }
        }
    }

    return subgraph;
}

/*
    * Função que retorna o grafo complementar do grafo atual
    * O grafo complementar possui os mesmos vértices do grafo original, mas possui uma aresta entre dois vértices
    * se e somente se o grafo original não possui uma aresta entre esses dois vértices  
    * retorna o grafo complementar
*/
Graph* Graph::getComplementGraph()
{
    //Cria um novo grafo complementar com o mesmo número de vértices
    Graph* complementGraph = new Graph(this->getOrder(), this->isDirected(), this->isWeightedEdges(), this->isWeightedNodes(), this->getNumberOfEdges());

    // Adiciona todos os vértices do grafo original no grafo complementar
    Node* currentNode = this->getFirstNode();
    while (currentNode != nullptr)
    {
        complementGraph->addNode(currentNode->getId(), currentNode->getWeight());
        currentNode = currentNode->getNextNode();
    }

    //percorre todos os pares de vértices e adiciona uma aresta no grafo complementar
    for(int i = 1; i <= this->getOrder(); i++){
        for(int j = 1; j <= this->getOrder(); j++){
            if(i != j){
                Node* nodeI = this->searchNode(i);
                Node* nodeJ = this->searchNode(j);


                //verifica se já existe uma aresta entre os vértices
                if(!nodeI->hasEdgeTo(j)){
                    complementGraph->addEdge(i, j, 0);
                }
            }
        }
    } 

    return complementGraph;
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
 * Colocamos os vértices em um vector ordenado, em que o vértice com menor peso relativo sempre esteja no topo
 *
 * O vector criado é alocado no atributo candidates
 */

void Graph::createCandidates()
{
    delete this->candidates;
    vector<pair<float, int>> *candidates = new vector<pair<float, int>>();
    this->candidates = candidates;
    Node *currentNode = this->firstNode;
    Node *relativeWeightNode = currentNode;

    while (currentNode != nullptr)
    {
        if (currentNode->isMarked())
        {
            currentNode = currentNode->getNextNode();
            continue;
        }

        candidates->push_back(make_pair(currentNode->getWeight() / currentNode->getNumberOfUnmarkedEdges(), currentNode->getId()));
        currentNode = currentNode->getNextNode();
    }
    sort(candidates->begin(), candidates->end(),
         [](pair<float, int> &a, pair<float, int> &b)
         {
             return a.first < b.first;
         });
}

/*
 * Função que imprime o vetor de pesos relativos
 */

void Graph::printRelativeVector()
{
    ofstream myfile;
    myfile.open("relativeVector.txt", ios::trunc);
    for (int i = 0; i < candidates->size(); i++)
    {
        myfile << (*candidates)[i].first << " " << (*candidates)[i].second << endl;
    }
    myfile.close();
}

/*
 * Função que atualiza o vetor de pesos relativos
 * Quando um vértice é removido, o peso relativo dos seus vizinhos é atualizado
 *
 * O vetor é ordenado novamente
 */

void Graph::updateCandidates(int removedNodeId)
{
    if (candidates->size() == 0)
    {
        return;
    }
    // Percorre o vetor de vizinhos
    const vector<int> &neighbors = this->openNeighborhoodMap[removedNodeId];

    for (int i = 0; i < neighbors.size(); i++)
    {
        int neighborId = neighbors[i];

        // Encontra o índice do vizinho no vetor de pesos relativos
        auto it = find_if(candidates->begin(), candidates->end(),
                          [neighborId](const pair<float, int> &nodeWeight)
                          {
                              return nodeWeight.second == neighborId;
                          });

        if (it != candidates->end())
        {
            // Obtém o índice do vizinho
            int index = distance(candidates->begin(), it);

            // Atualize o peso relativo do vizinho
            candidates->at(index).first = nodeMap[neighborId]->getWeight() / nodeMap[neighborId]->getNumberOfUnmarkedEdges();
        }
    }

    // Ordena novamente o vetor de pesos relativos em ordem decrescente
    sort(candidates->begin(), candidates->end(),
         [](pair<float, int> &a, pair<float, int> &b)
         {
             return a.first < b.first;
         });
}

/*
 * Algoritmo Guloso Construtivo
 * Utiliza a heurística do peso relativo para construir uma solução viável
 *
 * Retorna um vetor com os ids dos vértices que compõem a solução
 */
Metric Graph::relativeHeuristic()
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

    createCandidates();

    bool viable = false;
    int firstHeuristcNode = candidates->front().second;
    float totalWeight = 0;
    int iterations = 0;
    while (!candidates->empty())
    {
        // Coloca o vértice na solução
        Node *node = nodeMap[firstHeuristcNode];
        solution[firstHeuristcNode] = true;
        solutionVector.push_back(firstHeuristcNode);
        totalWeight += node->getWeight();

        // Marca o vértice
        this->markNode(node);

        // Verifica se a solução é viável
        if (this->isIsolated())
        {
            viable = true;
            break;
        }

        candidates->erase(candidates->begin());
        candidates->shrink_to_fit();

        if (!candidates->empty())
        {
            updateCandidates(firstHeuristcNode);
            firstHeuristcNode = candidates->front().second;
        }
    }

    end = chrono::high_resolution_clock::now();
    float elapse_time = chrono::duration_cast<chrono::milliseconds>(end - start).count();

    Metric metric;
    metric.time = elapse_time;
    metric.totalWeight = totalWeight;
    metric.numberOfNodes = solutionVector.size();
    // Reseta os vértices para não marcados para gerar outras soluções
    this->resetMarks();
    return metric;
}

void Graph::printRelativeHeuristic(string filename, string instanceName)
{
    Metric metric = relativeHeuristic();
    ofstream file;
    file.open(filename);
    file << "=============================" << endl;
    file << "Algoritimo Guloso Construtivo - " + instanceName << endl;
    file << "Tamanho da solução: " << metric.numberOfNodes << endl;
    file << "Peso total da solução: " << metric.totalWeight << endl;
    file << "Tempo de execução (ms): " << metric.time << endl;
    file << "=============================" << endl;

    file << endl;
    file.close();
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
    return min + rand() % (max - min + 1);
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

        createCandidates();

        // Percorre a fila de candidatos até a posição desejada
        int pos = this->randomRange(0, static_cast<int>((candidates->size() - 1) * alpha));
        int firstHeuristcNode = candidates->at(pos).second;

        while (!candidates->empty())
        {
            // Coloca o vértice na solução
            Node *node = this->nodeMap[firstHeuristcNode];
            solution[firstHeuristcNode] = true;
            auxSolutionVector.push_back(firstHeuristcNode);
            auxWeight += node->getWeight();

            // Marca o vértice
            this->markNode(node);

            // Verifica se a solução é viável
            if (this->isIsolated())
            {
                viable = true;
                break;
            }

            // Atualiza a lista de candidatos
            candidates->erase(candidates->begin() + pos);
            candidates->shrink_to_fit();

            if (!candidates->empty())
            {
                updateCandidates(firstHeuristcNode);

                pos = this->randomRange(0, static_cast<int>((candidates->size() - 1) * alpha));

                firstHeuristcNode = candidates->at(pos).second;
            }
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
    metric.time = elapse_time;
    metric.totalWeight = bestWeight;
    metric.numberOfNodes = bestSolutionVector.size();
    return metric;
}
void Graph::printRandomizedHeuristic(float alphas[], int size, int numInter, string filename, string instanceName)
{
    ofstream file;
    file.open(filename);
    file << "=============================" << endl;
    file << "Algoritimo Guloso Randomizado Adaptativo - " << instanceName << endl;
    file << "=============================" << endl;
    for (int i = 0; i < size; i++)
    {
        file << "=============================" << endl;
        file << "Iteração " << i + 1 << endl;
        file << "-----------------------------" << endl;
        for (int i = 0; i < 5; i++)
        {
            Metric metric = this->randomizedHeuristic(alphas[i], numInter);
            file << "Alfa = " << alphas[i] << " para " << numInter << " iterações" << endl
                 << "Tempo (s): " << metric.time << endl
                 << "Peso total: " << metric.totalWeight << endl
                 << "Tamanho da solução: " << metric.numberOfNodes << endl;
            file << "-----------------------------" << endl;
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
    createNeighborhoodMap();
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = chrono::high_resolution_clock::now();

    // Tamanho do bloco
    int block = numIter * 0.1;

    // Lista de Probabilidades
    vector<float> probabilities(5, 1.0 / 5);

    // Média dos pesos para cada valor de alpha
    vector<pair<float, int>> avgWeights(5, make_pair(0, 0));

    vector<int> auxSolutionVector, bestSolutionVector;
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
        for (int j = 1; j < this->order; j++)
        {
            solution.insert(make_pair(i, false));
        }

        // Criando a lista de candidatos ordenados pelo peso relativo
        int pos = 0;
        createCandidates();
        int firstHeuristcNode = this->candidates->front().second;

        while (!this->candidates->empty())
        {
            // Coloca o vértice na solução
            Node *node = this->nodeMap[firstHeuristcNode];
            solution[firstHeuristcNode] = true;
            auxSolutionVector.push_back(firstHeuristcNode);
            auxWeight += node->getWeight();

            // Marca o vértice
            this->markNode(node);

            // Verifica se a solução é viável
            if (this->isIsolated())
            {
                viable = true;
                break;
            }

            candidates->erase(candidates->begin() + pos);
            candidates->shrink_to_fit();

            // Atualiza a lista de candidatos
            updateCandidates(firstHeuristcNode);
            // Pegando o próximo vértice de acordo com o alpha
            pos = this->randomRange(0, static_cast<int>((candidates->size() - 1) * alpha));
            firstHeuristcNode = this->candidates->at(pos).second;
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
    for (int k = 0; k < 5; k++)
    {
        if (avgWeights[k].second > bestAlpha)
        {
            bestAlpha = k;
        }
    }

    Metric metric;
    metric.time = elapse_time;
    metric.totalWeight = bestWeight;
    metric.numberOfNodes = bestSolutionVector.size();
    metric.bestAlpha = bestAlpha;
    return metric;
}

void Graph::printReativeHeuristic(float alphas[], int size, int numInter, string filename, string instanceName)
{
    ofstream file;
    file.open(filename);
    file << "=============================" << endl;
    file << "Instância: " << instanceName << endl;
    for (int i = 0; i < size; i++)
    {
        file << "=============================" << endl;
        file << "Iteração " << i + 1 << endl;
        file << "-----------------------------" << endl;

        Metric metric = this->reativeHeuristic(alphas, numInter);
        file << "Numero de iterações : " << numInter << endl
             << "Tempo (s): " << metric.time << endl
             << "Peso total: " << metric.totalWeight << endl
             << "Melhor alfa: " << alphas[metric.bestAlpha] << endl
             << "Tamanho da solução: " << metric.numberOfNodes << endl;
        file << "-----------------------------" << endl;
    }

    file.close();
}
