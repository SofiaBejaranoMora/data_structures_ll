#pragma once
#include <string>
#include <list>
#include "User.h"
#include "UserTree.h"

using namespace std;

class FileManager {
private:
    string userFilename;

    bool deserializeUser(const string& userText, User& user);
    string serializeUser(const User& user);

public:
    FileManager();
    FileManager(const string& userFilename);

    void setUserFilename(const string& newUserFilename);
    string getUserFilename() const;

    bool saveUsers(const list<User>& users);
    list<User> loadUsers();

    UserTree* loadUserTree();
};
