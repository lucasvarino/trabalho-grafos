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

        graph->addNode(sourceId);
        graph->addNode(targetId);

        graph->addEdge(sourceId, targetId, weight);
    }

    file.close();

    return graph;
}

int main()
{
    Graph *graph = readNotDirectedWeightedEdges("complete.txt");
    graph->printGraph("graph.dot");

    cout << "Is complete: " << graph->isComplete() << endl;

    return 0;
}