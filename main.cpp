#include <iostream>
#include <fstream>
#include "Graph.h"

using namespace std;

int main()
{
    Graph *graph = new Graph(5, false, true, false);

    graph->addNode(1);
    graph->addNode(2);
    graph->addNode(3);
    graph->addNode(4);
    graph->addNode(5);

    graph->addEdge(1, 2, 15);
    graph->addEdge(3, 4, 11);
    graph->addEdge(4, 1, 12);
    graph->addEdge(4, 5, 112);
    graph->addEdge(5, 2, 5);
    graph->addEdge(5, 1, 25);
    graph->addEdge(1, 3, 48);
    graph->addEdge(2, 4, 1);
    graph->addEdge(2, 3, 6);
    graph->addEdge(3, 5, 71);

    graph->printGraph("graph.dot");

    delete graph;

    return 0;
}