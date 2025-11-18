#pragma once
#include "DoubleNode.h"

template <typename T>
class DoubleList {
private:
    DoubleNode<T>* head;
    DoubleNode<T>* tail;

    void destroyAllNodes();

public:
    DoubleList();
    ~DoubleList();

    void insertAtEnd(T newData);
    bool removeElement(T dataToRemove);
    T* search(T dataToFind);

    T* getHead();
    T* getTail();

    int size();
};

template <typename T>
DoubleList<T>::DoubleList() {
    head = nullptr;
    tail = nullptr;
}

template <typename T>
DoubleList<T>::~DoubleList() {
    destroyAllNodes();
}

template <typename T>
void DoubleList<T>::destroyAllNodes() {
    DoubleNode<T>* currentNode = head;
    while (currentNode != nullptr) {
        DoubleNode<T>* nextNode = currentNode->getNext();
        delete currentNode;
        currentNode = nextNode;
    }
    head = nullptr;
    tail = nullptr;
}

template <typename T>
void DoubleList<T>::insertAtEnd(T newData) {
    DoubleNode<T>* newNode = new DoubleNode<T>(newData);

    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    }
    else {
        newNode->setPrevious(tail);
        tail->setNext(newNode);
        tail = newNode;
    }
}

template <typename T>
bool DoubleList<T>::removeElement(T dataToRemove) {
    DoubleNode<T>* currentNode = head;

    while (currentNode != nullptr) {
        if (currentNode->getData() == dataToRemove) {
            DoubleNode<T>* previousNode = currentNode->getPrevious();
            DoubleNode<T>* nextNode = currentNode->getNext();

            if (previousNode != nullptr) {
                previousNode->setNext(nextNode);
            }
            else {
                head = nextNode;
            }

            if (nextNode != nullptr) {
                nextNode->setPrevious(previousNode);
            }
            else {
                tail = previousNode;
            }

            delete currentNode;
            return true;
        }

        currentNode = currentNode->getNext();
    }

    return false;
}

template <typename T>
T* DoubleList<T>::search(T dataToFind) {
    DoubleNode<T>* currentNode = head;

    while (currentNode != nullptr) {
        if (currentNode->getData() == dataToFind) {
            return &(currentNode->getData());
        }
        currentNode = currentNode->getNext();
    }

    return nullptr;
}

template <typename T>
T* DoubleList<T>::getHead() {
    if (head == nullptr) return nullptr;
    return &(head->getData());
}

template <typename T>
T* DoubleList<T>::getTail() {
    if (tail == nullptr) return nullptr;
    return &(tail->getData());
}

template <typename T>
int DoubleList<T>::size() {
    int count = 0;
    DoubleNode<T>* currentNode = head;
    while (currentNode != nullptr) {
        count++;
        currentNode = currentNode->getNext();
    }
    return count;
}
