#ifndef EDGE_H_INCLUDED
#define EDGE_H_INCLUDED

class Edge
{
private:
    int targetId;
    int originId;
    Edge *nextEdge;
    float weight;
    bool directed;

public:
    Edge(int targetId, int originId, float weight);
    ~Edge();

    int getTargetId();
    Edge *getNextEdge();
    int getOriginId();
    float getWeight();
    bool isDirected();

    void setNextEdge(Edge *nextEdge);
    void setWeight(float weight);
    void setDirected(bool directed);
};

#endif // EDGE_H_INCLUDED