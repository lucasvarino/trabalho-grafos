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

    Graph *graph = new Graph(0, false, true, false);

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

    Graph *graph = new Graph(0, false, false, false);

    cout << "Starting reading graph..." << endl;

    while (!file.eof())
    {
        char aux;
        int sourceId, targetId;

        file >> aux >> sourceId >> targetId;

        graph->addNode(sourceId, sourceId % 3 + 1);
        graph->addNode(targetId, targetId % 3 + 1);

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

    Graph *graph = readGreedy(input_file);

    vector<int> solution = graph->relativeHeuristc();
    return 0;
}