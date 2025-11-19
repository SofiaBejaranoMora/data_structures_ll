#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

FileManager::FileManager() {
    userFilename = "";
    graphFilename = "";
}

FileManager::FileManager(const string& userFilename) {
    this->userFilename = userFilename;
    graphFilename = "";
}

FileManager::FileManager(const string& userFilename, const string& graphFilename) {
    this->userFilename = userFilename;
    this->graphFilename = graphFilename;
}

void FileManager::setUserFilename(const string& newUserFilename) {
    userFilename = newUserFilename;
}

string FileManager::getUserFilename() const {
    return userFilename;
}

void FileManager::setGraphFilename(const string& newGraphFilename) {
    graphFilename = newGraphFilename;
}

string FileManager::getGraphFilename() const {
    return graphFilename;
}

string FileManager::serializeUser(const User& user) {
    string result = "{\n";
    result += user.toString();
    result += "\n}\n";
    return result;
}

bool FileManager::deserializeUser(const string& userText, User& user) {
    int id = 0;
    string name = "";
    int age = 0;
    double balance = 0.0;

    istringstream stream(userText);
    string line;

    while (getline(stream, line)) {
        if (line.rfind("Cedula:", 0) == 0) {
            string value = line.substr(7);
            while (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }
            id = stoi(value);
        }
        else if (line.rfind("Nombre:", 0) == 0) {
            string value = line.substr(7);
            while (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }
            name = value;
        }
        else if (line.rfind("Edad:", 0) == 0) {
            string value = line.substr(5);
            while (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }
            age = stoi(value);
        }
        else if (line.rfind("Balance:", 0) == 0) {
            string value = line.substr(8);
            while (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }
            balance = stod(value);
        }
    }

    if (id == 0 && name == "" && age == 0 && balance == 0.0) {
        return false;
    }

    user = User(id, name, age, balance);
    return true;
}

bool FileManager::saveUsers(const list<User>& users) {
    if (userFilename.empty()) {
        return false;
    }

    ofstream outputFile(userFilename);
    if (!outputFile.is_open()) {
        return false;
    }

    std::cout << "Archivo de usuarios guardado en: " << userFilename << std::endl;

    for (const User& user : users) {
        string serialized = serializeUser(user);
        outputFile << serialized;
    }

    outputFile.close();
    return true;
}

list<User> FileManager::loadUsers() {
    list<User> users;

    if (userFilename.empty()) {
        return users;
    }

    ifstream inputFile(userFilename);
    if (!inputFile.is_open()) {
        return users;
    }

    string line;
    bool insideUser = false;
    string currentUserText;

    while (getline(inputFile, line)) {
        if (!insideUser) {
            if (line.find('{') != string::npos) {
                insideUser = true;
                currentUserText.clear();
            }
        }
        else {
            if (line.find('}') != string::npos) {
                User user;
                if (deserializeUser(currentUserText, user)) {
                    users.push_back(user);
                }
                insideUser = false;
                currentUserText.clear();
            }
            else {
                if (!currentUserText.empty()) {
                    currentUserText += "\n";
                }
                currentUserText += line;
            }
        }
    }

    inputFile.close();
    return users;
}

UserTree* FileManager::loadUserTree() {
    UserTree* userTree = new UserTree();

    list<User> users = loadUsers();
    for (User user : users) {
        userTree->insertUser(user);
    }

    return userTree;
}

string FileManager::serializeStop(const Stop* stop) const {
    string result = "{\n";
    result += "Id: " + to_string(stop->getId()) + "\n";
    result += "Nombre: " + stop->getName() + "\n";
    result += "Descripcion: " + stop->getDescription() + "\n";
    result += "}\n";
    return result;
}

bool FileManager::deserializeStop(const string& stopText, Stop*& stop) {
    int id = 0;
    string name = "";
    string description = "";

    istringstream stream(stopText);
    string line;

    while (getline(stream, line)) {
        if (line.rfind("Id:", 0) == 0) {
            string value = line.substr(3);
            while (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }
            id = stoi(value);
        }
        else if (line.rfind("Nombre:", 0) == 0) {
            string value = line.substr(7);
            while (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }
            name = value;
        }
        else if (line.rfind("Descripcion:", 0) == 0) {
            string value = line.substr(12);
            while (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }
            description = value;
        }
    }

    if (id == 0 && name == "" && description == "") {
        return false;
    }

    stop = new Stop(id, name, description);
    return true;
}

string FileManager::serializeEdge(int originId, int destinationId, double weight) const {
    string result = "{\n";
    result += "Origen: " + to_string(originId) + "\n";
    result += "Destino: " + to_string(destinationId) + "\n";
    result += "Peso: " + to_string(weight) + "\n";
    result += "}\n";
    return result;
}

bool FileManager::deserializeEdge(const string& edgeText, int& originId, int& destinationId, double& weight) {
    originId = 0;
    destinationId = 0;
    weight = 0.0;

    istringstream stream(edgeText);
    string line;

    while (getline(stream, line)) {
        if (line.rfind("Origen:", 0) == 0) {
            string value = line.substr(7);
            while (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }
            originId = stoi(value);
        }
        else if (line.rfind("Destino:", 0) == 0) {
            string value = line.substr(8);
            while (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }
            destinationId = stoi(value);
        }
        else if (line.rfind("Peso:", 0) == 0) {
            string value = line.substr(5);
            while (!value.empty() && value[0] == ' ') {
                value.erase(0, 1);
            }
            weight = stod(value);
        }
    }

    if (originId == 0 && destinationId == 0 && weight == 0.0) {
        return false;
    }

    return true;
}

bool FileManager::saveGraph(const Graph& graph) {
    if (graphFilename.empty()) {
        return false;
    }

    ofstream outputFile(graphFilename);
    if (!outputFile.is_open()) {
        return false;
    }

    const vector<Stop*>& stops = graph.getStops();
    const vector<vector<pair<int, double>>>& adjacencyList = graph.getAdjacencyList();

    outputFile << "STOPS\n";
    for (const Stop* stop : stops) {
        outputFile << serializeStop(stop);
    }

    outputFile << "\nEDGES\n";

    int stopCount = (int)stops.size();
    for (int i = 0; i < stopCount; i++) {
        for (const auto& neighbor : adjacencyList[i]) {
            int j = neighbor.first;
            double w = neighbor.second;
            if (j > i) {
                int originId = stops[i]->getId();
                int destinationId = stops[j]->getId();
                outputFile << serializeEdge(originId, destinationId, w);
            }
        }
    }

    outputFile.close();
    std::cout << "Archivo de grafo guardado en: " << graphFilename << std::endl;
    return true;
}

Graph* FileManager::loadGraph() {
    if (graphFilename.empty()) {
        return nullptr;
    }

    ifstream inputFile(graphFilename);
    if (!inputFile.is_open()) {
        return nullptr;
    }

    Graph* graph = new Graph();

    string line;
    bool readingStops = false;
    bool readingEdges = false;
    bool insideBlock = false;
    string currentBlock;

    while (getline(inputFile, line)) {
        if (line == "STOPS") {
            readingStops = true;
            readingEdges = false;
            insideBlock = false;
            currentBlock.clear();
            continue;
        }
        if (line == "EDGES") {
            readingStops = false;
            readingEdges = true;
            insideBlock = false;
            currentBlock.clear();
            continue;
        }

        if (!insideBlock) {
            if (line.find('{') != string::npos) {
                insideBlock = true;
                currentBlock.clear();
            }
        }
        else {
            if (line.find('}') != string::npos) {
                if (readingStops) {
                    Stop* stop = nullptr;
                    if (deserializeStop(currentBlock, stop) && stop != nullptr) {
                        graph->addStop(stop);
                    }
                }
                else if (readingEdges) {
                    int originId;
                    int destinationId;
                    double weight;
                    if (deserializeEdge(currentBlock, originId, destinationId, weight)) {
                        graph->addConnection(originId, destinationId, weight);
                    }
                }
                insideBlock = false;
                currentBlock.clear();
            }
            else {
                if (!currentBlock.empty()) {
                    currentBlock += "\n";
                }
                currentBlock += line;
            }
        }
    }

    inputFile.close();
    return graph;
}
