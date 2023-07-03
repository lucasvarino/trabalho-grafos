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
    bool marked;

public:
    Edge(int targetId, int originId, float weight);
    ~Edge();

    int getTargetId();
    Edge *getNextEdge();
    int getOriginId();
    float getWeight();
    bool isDirected();
    bool isMarked() { return marked; };

    void setNextEdge(Edge *nextEdge);
    void setWeight(float weight);
    void setDirected(bool directed);
    void setMarked(bool marked) { this->marked = marked; };
};

#endif // EDGE_H_INCLUDED