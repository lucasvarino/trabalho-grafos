#include <iostream>
#include <fstream>
#include "Graph.h"

using namespace std;

Graph *readNotDirectedWeightedEdges(string filename)
{
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        return nullptr;
    }

    string order;

    getline(file, order);

    Graph *graph = new Graph(0, false, true, false, 0);

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
        // TODO: Voltar o peso para i mod 3 + 1

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

Graph *readMatrixMarket(string filename)
{
    srand(time(NULL));
    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        return nullptr;
    }

    string header;
    getline(file, header); // Skip the header line

    int numberOfNodes, numberOfNodesAgain, numberOfEdges;
    file >> numberOfNodes >> numberOfNodesAgain >> numberOfEdges;

    Graph *graph = new Graph(0, false, false, false, numberOfEdges);

    cout << "Starting reading graph..." << endl;

    for (int i = 0; i < numberOfEdges; i++)
    {
        int sourceId, targetId;

        file >> sourceId >> targetId;

        graph->addNode(sourceId, sourceId % 3 + 1);
        graph->addNode(targetId, targetId % 3 + 1);

        graph->addEdge(sourceId, targetId, 0);
    }

    if (numberOfNodes == graph->getOrder() && numberOfEdges == graph->getNumberOfEdges())
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

int main(int argc, char const *argv[])
{
    // Verificação se todos os parâmetros do programa foram entrados
    if (argc != 3)
    {

        cout << "ERROR: Expecting: ./<program_name> <input_file> <output_file> " << endl;
        return 1;
    }
    string program_name = argv[0];
    string input_file = argv[1];
    string output_file = argv[2];

    Graph *graph = readNotDirectedWeightedEdges(input_file);
    float alphas[5] = {0.05, 0.1, 0.15, 0.3, 0.5};
    // tirar tudo antes da / e depois do .
    input_file = input_file.substr(input_file.find_last_of("/") + 1);
    input_file = input_file.substr(0, input_file.find_last_of("."));
    // graph->printReativeHeuristic(alphas, 5, 1000, output_file, input_file);
    cout << "Instance: " << input_file << endl;
    // graph->printRelativeHeuristic(output_file, input_file);
    //  graph->printRandomizedHeuristic(alphas, 30, 1000, output_file, input_file);
    // graph->printReativeHeuristic(alphas, 10, 5000, output_file, input_file);
    graph->printGraph("visualizeGraph.dot");
    vector<int> nodes;
    nodes.push_back(1);
    nodes.push_back(2);
    nodes.push_back(5);
    Graph *subgraph = graph->getInducedSubgraph(nodes);
    subgraph->printGraph("visualizeGraph2.dot");
    return 0;
}