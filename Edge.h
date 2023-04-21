#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

class Edge
{
private:
    int targetId;
    Edge *nextEdge;
    float weight;
    bool directed;

public:
    Edge(int targetId, float weight);
    ~Edge();

    int getTargetId();
    Edge *getNextEdge();
    float getWeight();
    bool isDirected();

    void setNextEdge(Edge *nextEdge);
    void setWeight(float weight);
    void setDirected(bool directed);
};

#endif // EDGE_H_INCLUDED