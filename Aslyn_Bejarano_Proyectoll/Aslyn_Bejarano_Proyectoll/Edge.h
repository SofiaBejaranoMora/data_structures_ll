#pragma once
#include "Stop.h"

class Edge {
private:
    Stop* origin;
    Stop* destination;
    double weight;

public:
    Edge();
    Edge(Stop* origin, Stop* destination, double weight);

    Stop* getOrigin() const;
    Stop* getDestination() const;
    double getWeight() const;

    void setOrigin(Stop* origin);
    void setDestination(Stop* destination);
    void setWeight(double weight);
};
