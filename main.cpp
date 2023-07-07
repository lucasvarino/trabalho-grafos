#include <iostream>
#include <fstream>
#include "Graph.h"

using namespace std;

Graph *readGraph(string filename, bool isDirected, bool isWeightedEdges, bool isWeightedNodes)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        return nullptr;
    }

    string order;

    getline(file, order);

    Graph *graph = new Graph(0, isDirected, isWeightedEdges, isWeightedNodes, 0);

    while (!file.eof())
    {
        int sourceId, targetId;
        float weight;

        file >> sourceId >> targetId >> weight;

        graph->addNode(sourceId, 0);
        graph->addNode(targetId, 0);

        graph->addEdge(sourceId, targetId, weight);
    }

    file.close();

    return graph;
}

Graph *readGreedy(string filename)
{
    srand(time(NULL));
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        return nullptr;
    }

    string orderS;
    string numberOfEdgesS;
    file >> orderS >> numberOfEdgesS;
    int order = stoi(orderS);
    int numberOfEdges = stoi(numberOfEdgesS);

    Graph *graph = new Graph(0, false, false, false, numberOfEdges);

    cout << "Starting reading graph..." << endl;

    while (!file.eof())
    {
        char aux;
        int sourceId, targetId;

        file >> aux >> sourceId >> targetId;

        // Mudando o peso dos vértices apenas para comparação com o outro artigo
        // A Fast and Robust Heuristic Algorithm for the Minimum Weight Vertex Cover Problem

        graph->addNode(sourceId, (sourceId % 200) + 1);
        graph->addNode(targetId, (targetId % 200) + 1);

        graph->addEdge(sourceId, targetId, 0);
    }

    if (order == graph->getOrder() && numberOfEdges == graph->getNumberOfEdges())
    {
        cout << "Graph read successfully!" << endl;
    }
    else
    {
        cout << "Error reading graph." << endl;
    }

    file.close();

    return graph;
}

void menuParte1(string input_file, string output_file, bool isDirected, bool isWeightedEdges, bool isWeightedNodes)
{
    Graph *graph = readGraph(input_file, isDirected, isWeightedEdges, isWeightedNodes);

    Graph *aux;
    pair<int, int> degree;
    int option, node, k;
    vector<int> nodes;

    cout << "========================================" << endl;
    cout << "Trabalho de Teoria dos Grafos - Parte 01" << endl;
    cout << "========================================" << endl;
    cout << "1 - Retornar o grau de um determinado nó" << endl;
    cout << "2 - Verificar k-regularidade do grafo" << endl;
    cout << "3 - Retornar a ordem do grafo" << endl;
    cout << "4 - Verificar se o grafo é trivial" << endl;
    cout << "5 - Verificar se o grafo é nulo" << endl;
    cout << "6 - Vizinhança aberta de um determinado nó" << endl;
    cout << "7 - Vizinhança fechada de um determinado nó" << endl;
    cout << "8 - Verificar se o grafo é um multigrafo" << endl;
    cout << "9 - Verificar se o grafo é completo" << endl;
    cout << "10 - Verificar se o grafo é bipartido" << endl;
    cout << "11 - Retornar o grau do grafo " << endl;
    cout << "12 - Fecho Transitivo Direto de um determiado nó" << endl;
    cout << "13 - Fecho Transitivo Indireto de um determiado nó" << endl;
    cout << "14 - Apresentar a sequência de graus" << endl;
    cout << "15 - Apresentar o subgrafo induzido de um conjunto de vértices" << endl;
    cout << "16 - Apresentar o grafo complementar" << endl;
    cout << "17 - Verificar se o grafo é euleriano" << endl;
    cout << "18 - Apresentar o raio, diâmetro, centro e periferia do grafo" << endl;
    cout << "0 - Sair" << endl;
    cout << "========================================" << endl;

    cout << "Informe a opção desejada:" << endl;
    cin >> option;
    switch (option)
    {
    case 1:
        cout << "Informe o nó desejado:" << endl;
        cin >> node;
        degree = graph->getNodeDegree(node);
        cout << "Grau de entrada: " << degree.first << endl;
        cout << "Grau de saída: " << degree.second << endl;
        break;

    case 2:
        cout << "Informe o valor de k:" << endl;
        cin >> k;
        cout << (graph->isKRegular(k) ? "O grafo é " : "O grafo não é ") << k << "-regular" << endl;
        break;
    case 3:
        cout << "Ordem do grafo: " << graph->getOrder() << endl;
        break;
    case 4:
        cout << (graph->isTrivialGraph() ? "O grafo é trivial" : "O grafo não é trivial") << endl;
        break;
    case 5:
        cout << (graph->isNullGraph() ? "O grafo é nulo" : "O grafo não é nulo") << endl;
        break;
    case 6:
        cout << "Informe o nó desejado:" << endl;
        cin >> node;
        cout << "Vizinhança aberta do nó " << node << ": ";
        for (int i = 0; i < graph->getOpenNeighborhood(node).size(); i++)
        {
            cout << graph->getOpenNeighborhood(node)[i] << " ";
        }
        cout << endl;
        break;
    case 7:
        cout << "Informe o nó desejado:" << endl;
        cin >> node;
        cout << "Vizinhança fechada do nó " << node << ": ";
        for (int i = 0; i < graph->getClosedNeighborhood(node).size(); i++)
        {
            cout << graph->getClosedNeighborhood(node)[i] << " ";
        }
        cout << endl;
        break;
    case 8:
        cout << (graph->isMultigraph() ? "O grafo é um multigrafo" : "O grafo não é um multigrafo") << endl;
        break;
    case 9:
        cout << (graph->isComplete() ? "O grafo é completo" : "O grafo não é completo") << endl;
        break;
    case 10:
        cout << (graph->isBipartite() ? "O grafo é bipartido" : "O grafo não é bipartido") << endl;
        break;
    case 11:
        cout << "Grau do grafo: " << graph->getGraphDegree() << endl;
        break;
    case 12:
        cout << "Informe o nó desejado:" << endl;
        cin >> node;
        cout << "Fecho Transitivo Direto do nó " << node << ": ";
        for (int i = 0; i < graph->directTransitiveClosure(node).size(); i++)
        {
            cout << graph->directTransitiveClosure(node)[i] << " ";
        }
        cout << endl;
        break;
    case 13:
        cout << "Informe o nó desejado:" << endl;
        cin >> node;
        cout << "Fecho Transitivo Indireto do nó " << node << ": ";
        for (int i = 0; i < graph->indirectTransitiveClosure(node).size(); i++)
        {
            cout << graph->indirectTransitiveClosure(node)[i] << " ";
        }
        cout << endl;
        break;
    case 14:
        cout << "Sequência de graus: ";
        for (int i = 0; i < graph->getDegreeSequence().size(); i++)
        {
            cout << graph->getDegreeSequence()[i] << " ";
        }
        cout << endl;
        break;
    case 15:
        cout << "Informe os nós desejados: (-1 para sair)" << endl;

        cin >> node;
        while (node != -1)
        {
            nodes.push_back(node);
            cin >> node;
        }
        cout << "Subgrafo induzido criado em " << output_file << endl;
        aux = graph->getInducedSubgraph(nodes);
        aux->printGraph(output_file);
        delete aux;
        break;
    case 16:
        cout << "Grafo complementar criado em " << output_file << endl;
        aux = graph->getComplementGraph();
        aux->printGraph(output_file);
        delete aux;
        break;
    case 17:
        cout << (graph->isEulerian() ? "O grafo é euleriano" : "O grafo não é euleriano") << endl;
        break;
    case 18:
        graph->printGraphProperties();
        break;
    case 0:
        delete graph;
        cout << "Saindo..." << endl;
        exit(0);
        break;
    default:
        cout << "Opção inválida" << endl;
        menuParte1(input_file, output_file, isDirected, isWeightedEdges, isWeightedNodes);
        break;
    }
    menuParte1(input_file, output_file, isDirected, isWeightedEdges, isWeightedNodes);
}

void menuParte2(string input_file, string output_file, int numIter, int numIterAlpha, int seed)
{
    Graph *graphGreedy = readGreedy(input_file);

    string instance = input_file.substr(input_file.find_last_of("/") + 1);
    instance = instance.substr(0, instance.find_last_of("."));

    float alphas[5] = {0.05, 0.1, 0.15, 0.3, 0.5};

    cout << "=============================" << endl;
    cout << "Trabalho de Teoria dos Grafos - Parte 02" << endl;
    cout << "Instância: " << instance << endl;
    cout << "=============================" << endl;
    cout << "1 - Guloso Construtivo" << endl;
    cout << "2 - Guloso Randomizado" << endl;
    cout << "3 - Guloso Randomizado Reativo" << endl;
    cout << "0 - Sair" << endl;
    cout << "=============================" << endl;

    int option;
    cout << "Informe a opção desejada:" << endl;
    cin >> option;
    switch (option)
    {
    case 1:
        graphGreedy->printConstructiveGreedy(output_file, instance);
        break;
    case 2:
        graphGreedy->printRandomizedGreedy(alphas, numIter, numIterAlpha, output_file, instance, seed);
        break;
    case 3:
        graphGreedy->printReativeGreedy(alphas, numIter, numIterAlpha, output_file, instance, seed);
        break;
    case 0:
        cout << "Saindo..." << endl;
        break;
    default:
        cout << "Opção inválida" << endl;
        menuParte2(input_file, output_file, numIter, numIterAlpha, seed);
    }
    delete graphGreedy;
}

int main(int argc, char const *argv[])
{
    // Verificação se todos os parâmetros do programa foram entrados
    bool isDirected, isWeightedEdges, isWeightedNodes, isGreedy;
    int numIter, numIterAlpha, seed;
    cout << "Uso para a parte 01: " << argv[0] << " <input_file> <output_file> <isDirected> <isWeightedEdges> <isWeightedNodes>" << endl;
    cout << "Uso para a parte 02: " << argv[0] << " <input_file> <output_file> <isGreedy> <numIter> <numIterAlpha> <seed>" << endl;
    if (argc == 7)
    {
        isDirected = false;
        isWeightedEdges = false;
        isWeightedNodes = true;
        isGreedy = argv[3];

        numIter = stoi(argv[4]);
        numIterAlpha = stoi(argv[5]);
        seed = stoi(argv[6]);
    }
    else if (argc == 6)
    {
        bool isDirected = argv[3];
        bool isWeightedEdges = argv[4];
        bool isWeightedNodes = argv[5];
        isGreedy = false;
        numIter = 0;
        numIterAlpha = 0;
        seed = 0;
    }
    else
    {
        cout << "Número de parâmetros inválido" << endl;
        return 1;
    }

    string program_name = argv[0];
    string input_file = argv[1];
    string output_file = argv[2];
    srand(seed);

    if (isGreedy)
    {
        menuParte2(input_file, output_file, numIter, numIterAlpha, seed);
    }
    else
    {
        menuParte1(input_file, output_file, isDirected, isWeightedEdges, isWeightedNodes);
    }

    return 0;
}