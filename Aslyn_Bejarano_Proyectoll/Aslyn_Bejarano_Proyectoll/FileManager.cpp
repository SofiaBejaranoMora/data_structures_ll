#include "FileManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

FileManager::FileManager() {
    userFilename = "";
}

FileManager::FileManager(const string& userFilename) {
    this->userFilename = userFilename;
}

void FileManager::setUserFilename(const string& newUserFilename) {
    userFilename = newUserFilename;
}

string FileManager::getUserFilename() const {
    return userFilename;
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

    fs::path fullPath = fs::absolute(userFilename);
    cout << "Archivo de usuarios guardado en: " << fullPath.string() << endl;

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
