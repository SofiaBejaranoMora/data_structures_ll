#include "Graph.h"
#include <queue>
#include <stack>
#include <limits>
#include <algorithm>
#include <functional>

Graph::Graph() {
}

Graph::~Graph() {
}

int Graph::findStopIndexById(int stopId) const {
    for (int i = 0; i < (int)stops.size(); i++) {
        if (stops[i] != nullptr && stops[i]->getId() == stopId) {
            return i;
        }
    }
    return -1;
}

int Graph::findStopIndexByName(const string& stopName) const {
    for (int i = 0; i < (int)stops.size(); i++) {
        if (stops[i] != nullptr && stops[i]->getName() == stopName) {
            return i;
        }
    }
    return -1;
}

bool Graph::addStop(Stop* newStop) {
    if (newStop == nullptr) {
        return false;
    }

    if (findStopIndexById(newStop->getId()) != -1) {
        return false;
    }

    if (findStopIndexByName(newStop->getName()) != -1) {
        return false;
    }

    stops.push_back(newStop);
    adjacencyList.push_back(vector<pair<int, double>>());
    return true;
}

bool Graph::addConnection(int originStopId, int destinationStopId, double weight) {
    int originIndex = findStopIndexById(originStopId);
    int destinationIndex = findStopIndexById(destinationStopId);

    if (originIndex == -1 || destinationIndex == -1) {
        return false;
    }

    adjacencyList[originIndex].push_back(make_pair(destinationIndex, weight));
    adjacencyList[destinationIndex].push_back(make_pair(originIndex, weight));
    return true;
}

Stop* Graph::findStopById(int stopId) const {
    int index = findStopIndexById(stopId);
    if (index == -1) {
        return nullptr;
    }
    return stops[index];
}

const vector<Stop*>& Graph::getStops() const {
    return stops;
}

const vector<vector<pair<int, double>>>& Graph::getAdjacencyList() const {
    return adjacencyList;
}

vector<Stop*> Graph::getShortestPathDijkstra(
    const string& originStopName,
    const string& destinationStopName,
    double& totalCost
) const {
    vector<Stop*> path;

    int n = (int)stops.size();
    if (n == 0) {
        totalCost = 0.0;
        return path;
    }

    int originIndex = findStopIndexByName(originStopName);
    int destinationIndex = findStopIndexByName(destinationStopName);

    if (originIndex == -1 || destinationIndex == -1) {
        totalCost = 0.0;
        return path;
    }

    double infinityValue = numeric_limits<double>::infinity();

    vector<double> distances(n, infinityValue);
    vector<int> previous(n, -1);
    vector<bool> visited(n, false);

    distances[originIndex] = 0.0;

    typedef pair<double, int> NodeState;
    priority_queue<NodeState, vector<NodeState>, greater<NodeState>> pendingNodes;

    pendingNodes.push(make_pair(0.0, originIndex));

    while (!pendingNodes.empty()) {
        NodeState currentState = pendingNodes.top();
        pendingNodes.pop();

        double currentDistance = currentState.first;
        int currentIndex = currentState.second;

        if (visited[currentIndex]) {
            continue;
        }

        visited[currentIndex] = true;

        if (currentIndex == destinationIndex) {
            break;
        }

        const vector<pair<int, double>>& neighbors = adjacencyList[currentIndex];

        for (const pair<int, double>& neighbor : neighbors) {
            int neighborIndex = neighbor.first;
            double edgeWeight = neighbor.second;

            double newDistance = currentDistance + edgeWeight;

            if (newDistance < distances[neighborIndex]) {
                distances[neighborIndex] = newDistance;
                previous[neighborIndex] = currentIndex;
                pendingNodes.push(make_pair(newDistance, neighborIndex));
            }
        }
    }

    if (distances[destinationIndex] == infinityValue) {
        totalCost = 0.0;
        return path;
    }

    totalCost = distances[destinationIndex];

    vector<int> indexPath;
    int current = destinationIndex;
    while (current != -1) {
        indexPath.push_back(current);
        current = previous[current];
    }

    reverse(indexPath.begin(), indexPath.end());

    for (int index : indexPath) {
        path.push_back(stops[index]);
    }

    return path;
}

vector<Stop*> Graph::breadthFirstTraversal() const {
    vector<Stop*> result;

    int n = (int)stops.size();
    if (n == 0) {
        return result;
    }

    int originIndex = -1;
    int smallestId = numeric_limits<int>::max();

    for (int i = 0; i < n; i++) {
        if (stops[i] != nullptr && stops[i]->getId() < smallestId) {
            smallestId = stops[i]->getId();
            originIndex = i;
        }
    }

    if (originIndex == -1) {
        return result;
    }

    vector<bool> visited(n, false);
    queue<int> pendingNodes;

    visited[originIndex] = true;
    pendingNodes.push(originIndex);

    while (!pendingNodes.empty()) {
        int currentIndex = pendingNodes.front();
        pendingNodes.pop();

        result.push_back(stops[currentIndex]);

        const vector<pair<int, double>>& neighbors = adjacencyList[currentIndex];
        for (const pair<int, double>& neighbor : neighbors) {
            int neighborIndex = neighbor.first;
            if (!visited[neighborIndex]) {
                visited[neighborIndex] = true;
                pendingNodes.push(neighborIndex);
            }
        }
    }

    return result;
}

vector<Stop*> Graph::depthFirstTraversal() const {
    vector<Stop*> result;

    int n = (int)stops.size();
    if (n == 0) {
        return result;
    }

    int originIndex = -1;
    int smallestId = numeric_limits<int>::max();

    for (int i = 0; i < n; i++) {
        if (stops[i] != nullptr && stops[i]->getId() < smallestId) {
            smallestId = stops[i]->getId();
            originIndex = i;
        }
    }

    if (originIndex == -1) {
        return result;
    }

    vector<bool> visited(n, false);
    stack<int> pendingNodes;

    pendingNodes.push(originIndex);

    while (!pendingNodes.empty()) {
        int currentIndex = pendingNodes.top();
        pendingNodes.pop();

        if (visited[currentIndex]) {
            continue;
        }

        visited[currentIndex] = true;
        result.push_back(stops[currentIndex]);

        const vector<pair<int, double>>& neighbors = adjacencyList[currentIndex];
        for (int i = (int)neighbors.size() - 1; i >= 0; i--) {
            int neighborIndex = neighbors[i].first;
            if (!visited[neighborIndex]) {
                pendingNodes.push(neighborIndex);
            }
        }
    }

    return result;
}

Graph* Graph::getTreePrim() const {
    Graph* mstGraph = new Graph();

    int n = (int)stops.size();
    if (n == 0) {
        return mstGraph;
    }

    for (Stop* stop : stops) {
        mstGraph->addStop(stop);
    }

    double infinityValue = numeric_limits<double>::infinity();

    vector<double> key(n, infinityValue);
    vector<int> parent(n, -1);
    vector<bool> inMst(n, false);

    key[0] = 0.0;

    typedef pair<double, int> NodeState;
    priority_queue<NodeState, vector<NodeState>, greater<NodeState>> pendingNodes;

    pendingNodes.push(make_pair(0.0, 0));

    while (!pendingNodes.empty()) {
        NodeState currentState = pendingNodes.top();
        pendingNodes.pop();

        int currentIndex = currentState.second;

        if (inMst[currentIndex]) {
            continue;
        }

        inMst[currentIndex] = true;

        const vector<pair<int, double>>& neighbors = adjacencyList[currentIndex];

        for (const pair<int, double>& neighbor : neighbors) {
            int neighborIndex = neighbor.first;
            double edgeWeight = neighbor.second;

            if (!inMst[neighborIndex] && edgeWeight < key[neighborIndex]) {
                key[neighborIndex] = edgeWeight;
                parent[neighborIndex] = currentIndex;
                pendingNodes.push(make_pair(key[neighborIndex], neighborIndex));
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (parent[i] != -1) {
            int originId = stops[i]->getId();
            int destinationId = stops[parent[i]]->getId();
            double weight = key[i];
            mstGraph->addConnection(originId, destinationId, weight);
        }
    }

    return mstGraph;
}

double Graph::getTreePrimTotalCost() const {
    int n = (int)stops.size();
    if (n == 0) {
        return 0.0;
    }

    double infinityValue = numeric_limits<double>::infinity();

    vector<double> key(n, infinityValue);
    vector<bool> inMst(n, false);

    key[0] = 0.0;

    typedef pair<double, int> NodeState;
    priority_queue<NodeState, vector<NodeState>, greater<NodeState>> pendingNodes;

    pendingNodes.push(make_pair(0.0, 0));

    double totalCost = 0.0;

    while (!pendingNodes.empty()) {
        NodeState currentState = pendingNodes.top();
        pendingNodes.pop();

        double currentKey = currentState.first;
        int currentIndex = currentState.second;

        if (inMst[currentIndex]) {
            continue;
        }

        inMst[currentIndex] = true;
        totalCost += currentKey;

        const vector<pair<int, double>>& neighbors = adjacencyList[currentIndex];

        for (const pair<int, double>& neighbor : neighbors) {
            int neighborIndex = neighbor.first;
            double edgeWeight = neighbor.second;

            if (!inMst[neighborIndex] && edgeWeight < key[neighborIndex]) {
                key[neighborIndex] = edgeWeight;
                pendingNodes.push(make_pair(key[neighborIndex], neighborIndex));
            }
        }
    }

    return totalCost;
}
