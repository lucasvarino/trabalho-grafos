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

    Graph *graph = new Graph(stoi(order), false, true, false);

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
    Graph *graph = readNotDirectedWeightedEdges("graph.txt");
    graph->printGraph("graph.dot");

    graph->removeNode(1);

    graph->printGraph("graph2.dot");

    // vector<int> openNeighborhood = graph->getOpenNeighborhood(1);

    // cout << "N(" << 1 << ") = ";
    // for (int i = 0; i < openNeighborhood.size(); i++)
    // {
    //     cout << openNeighborhood[i] << " ";
    // }

    return 0;
}