#pragma once
#include "User.h"
#include "NodeTree.h"
#include <list>

using namespace std;

class UserTree {
private:
    NodeTree<User>* root;

    NodeTree<User>* insertRecursive(NodeTree<User>* currentNode, User newUser, bool& wasInserted);
    NodeTree<User>* removeRecursive(NodeTree<User>* currentNode, int userIdentifierToRemove, bool& wasRemoved);
    void destroyRecursive(NodeTree<User>* currentNode);
    void inOrderRecursive(NodeTree<User>* currentNode, list<User>& users);
    void preOrderRecursive(NodeTree<User>* currentNode, list<User>& users);
    void postOrderRecursive(NodeTree<User>* currentNode, list<User>& users);

public:
    UserTree();
    ~UserTree();

    bool insertUser(User newUser);
    bool removeUser(int userIdentifierToRemove);

    list<User> getInOrder();
    list<User> getPreOrder();
    list<User> getPostOrder();

    NodeTree<User>* getRoot();
};
