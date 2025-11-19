#pragma once
#include <vector>
#include <utility>
#include <string>
#include "Stop.h"

using namespace std;

class Graph {
private:
    vector<Stop*> stops;
    vector<vector<pair<int, double>>> adjacencyList;

    int findStopIndexById(int stopId) const;
    int findStopIndexByName(const string& stopName) const;

public:
    Graph();
    ~Graph();

    bool addStop(Stop* newStop);
    bool addConnection(int originStopId, int destinationStopId, double weight);

    Stop* findStopById(int stopId) const;

    const vector<Stop*>& getStops() const;
    const vector<vector<pair<int, double>>>& getAdjacencyList() const;

    vector<Stop*> getShortestPathDijkstra(
        const string& originStopName,
        const string& destinationStopName,
        double& totalCost
    ) const;

    vector<Stop*> breadthFirstTraversal() const;
    vector<Stop*> depthFirstTraversal() const;

    Graph* getTreePrim() const;
    double getTreePrimTotalCost() const;
};
