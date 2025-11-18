#include "UserTree.h"

UserTree::UserTree() {
    root = nullptr;
}

UserTree::~UserTree() {
    destroyRecursive(root);
    root = nullptr;
}

void UserTree::destroyRecursive(NodeTree<User>* currentNode) {
    if (currentNode == nullptr) return;
    destroyRecursive(currentNode->getLeft());
    destroyRecursive(currentNode->getRight());
    delete currentNode;
}

NodeTree<User>* UserTree::insertRecursive(NodeTree<User>* currentNode, User newUser, bool& wasInserted) {
    if (currentNode == nullptr) {
        wasInserted = true;
        NodeTree<User>* newNode = new NodeTree<User>(newUser);
        return newNode;
    }

    int newUserIdentifier = newUser.getId();
    int currentUserIdentifier = currentNode->getData().getId();

    if (newUserIdentifier < currentUserIdentifier) {
        currentNode->setLeft(insertRecursive(currentNode->getLeft(), newUser, wasInserted));
    }
    else if (newUserIdentifier > currentUserIdentifier) {
        currentNode->setRight(insertRecursive(currentNode->getRight(), newUser, wasInserted));
    }
    else {
        wasInserted = false;
    }

    return currentNode;
}

bool UserTree::insertUser(User newUser) {
    bool wasInserted = false;
    root = insertRecursive(root, newUser, wasInserted);
    return wasInserted;
}

NodeTree<User>* UserTree::removeRecursive(NodeTree<User>* currentNode, int userIdentifierToRemove, bool& wasRemoved) {
    if (currentNode == nullptr) return nullptr;

    int currentUserIdentifier = currentNode->getData().getId();

    if (userIdentifierToRemove < currentUserIdentifier) {
        currentNode->setLeft(removeRecursive(currentNode->getLeft(), userIdentifierToRemove, wasRemoved));
    }
    else if (userIdentifierToRemove > currentUserIdentifier) {
        currentNode->setRight(removeRecursive(currentNode->getRight(), userIdentifierToRemove, wasRemoved));
    }
    else {
        wasRemoved = true;

        if (currentNode->getLeft() == nullptr && currentNode->getRight() == nullptr) {
            delete currentNode;
            return nullptr;
        }
        else if (currentNode->getLeft() == nullptr) {
            NodeTree<User>* replacementNode = currentNode->getRight();
            delete currentNode;
            return replacementNode;
        }
        else if (currentNode->getRight() == nullptr) {
            NodeTree<User>* replacementNode = currentNode->getLeft();
            delete currentNode;
            return replacementNode;
        }
        else {
            NodeTree<User>* successorNode = currentNode->getRight();
            while (successorNode->getLeft() != nullptr) {
                successorNode = successorNode->getLeft();
            }

            User successorUser = successorNode->getData();
            currentNode->setData(successorUser);

            bool wasSuccessorRemoved = false;
            currentNode->setRight(removeRecursive(currentNode->getRight(), successorUser.getId(), wasSuccessorRemoved));
        }
    }

    return currentNode;
}

bool UserTree::removeUser(int userIdentifierToRemove) {
    bool wasRemoved = false;
    root = removeRecursive(root, userIdentifierToRemove, wasRemoved);
    return wasRemoved;
}

void UserTree::inOrderRecursive(NodeTree<User>* currentNode, list<User>& users) {
    if (currentNode == nullptr) return;
    inOrderRecursive(currentNode->getLeft(), users);
    users.push_back(currentNode->getData());
    inOrderRecursive(currentNode->getRight(), users);
}

void UserTree::preOrderRecursive(NodeTree<User>* currentNode, list<User>& users) {
    if (currentNode == nullptr) return;
    users.push_back(currentNode->getData());
    preOrderRecursive(currentNode->getLeft(), users);
    preOrderRecursive(currentNode->getRight(), users);
}

void UserTree::postOrderRecursive(NodeTree<User>* currentNode, list<User>& users) {
    if (currentNode == nullptr) return;
    postOrderRecursive(currentNode->getLeft(), users);
    postOrderRecursive(currentNode->getRight(), users);
    users.push_back(currentNode->getData());
}

list<User> UserTree::getInOrder() {
    list<User> users;
    inOrderRecursive(root, users);
    return users;
}

list<User> UserTree::getPreOrder() {
    list<User> users;
    preOrderRecursive(root, users);
    return users;
}

list<User> UserTree::getPostOrder() {
    list<User> users;
    postOrderRecursive(root, users);
    return users;
}

NodeTree<User>* UserTree::getRoot() {
    return root;
}
