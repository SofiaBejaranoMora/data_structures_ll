#include "Edge.h"

Edge::Edge() {
    origin = nullptr;
    destination = nullptr;
    weight = 0.0;
}

Edge::Edge(Stop* origin, Stop* destination, double weight) {
    this->origin = origin;
    this->destination = destination;
    this->weight = weight;
}

Stop* Edge::getOrigin() const {
    return origin;
}

Stop* Edge::getDestination() const {
    return destination;
}

double Edge::getWeight() const {
    return weight;
}

void Edge::setOrigin(Stop* origin) {
    this->origin = origin;
}

void Edge::setDestination(Stop* destination) {
    this->destination = destination;
}

void Edge::setWeight(double weight) {
    this->weight = weight;
}
