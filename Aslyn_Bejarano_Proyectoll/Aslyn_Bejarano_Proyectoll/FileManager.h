#pragma once
#include <string>
#include <list>
#include "User.h"
#include "UserTree.h"
#include "Graph.h"

using namespace std;

class FileManager {
private:
    string userFilename;
    string graphFilename;

    bool deserializeUser(const string& userText, User& user);
    string serializeUser(const User& user);

    bool deserializeStop(const string& stopText, Stop*& stop);
    string serializeStop(const Stop* stop) const;

    bool deserializeEdge(const string& edgeText, int& originId, int& destinationId, double& weight);
    string serializeEdge(int originId, int destinationId, double weight) const;

public:
    FileManager();
    FileManager(const string& userFilename);
    FileManager(const string& userFilename, const string& graphFilename);

    void setUserFilename(const string& newUserFilename);
    string getUserFilename() const;

    void setGraphFilename(const string& newGraphFilename);
    string getGraphFilename() const;

    bool saveUsers(const list<User>& users);
    list<User> loadUsers();
    UserTree* loadUserTree();

    bool saveGraph(const Graph& graph);
    Graph* loadGraph();
};
