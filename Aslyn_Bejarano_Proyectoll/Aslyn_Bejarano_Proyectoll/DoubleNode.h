#pragma once
#pragma once

template <typename T>
class DoubleNode {
private:
    T data;
    DoubleNode<T>* previous;
    DoubleNode<T>* next;

public:
    DoubleNode();
    DoubleNode(T newData);

    T getData();
    void setData(T newData);

    DoubleNode<T>* getPrevious();
    DoubleNode<T>* getNext();
    void setPrevious(DoubleNode<T>* previousNode);
    void setNext(DoubleNode<T>* nextNode);
};

template <typename T>
DoubleNode<T>::DoubleNode() {
    previous = nullptr;
    next = nullptr;
}

template <typename T>
DoubleNode<T>::DoubleNode(T newData) {
    data = newData;
    previous = nullptr;
    next = nullptr;
}

template <typename T>
T DoubleNode<T>::getData() {
    return data;
}

template <typename T>
void DoubleNode<T>::setData(T newData) {
    data = newData;
}

template <typename T>
DoubleNode<T>* DoubleNode<T>::getPrevious() {
    return previous;
}

template <typename T>
DoubleNode<T>* DoubleNode<T>::getNext() {
    return next;
}

template <typename T>
void DoubleNode<T>::setPrevious(DoubleNode<T>* previousNode) {
    previous = previousNode;
}

template <typename T>
void DoubleNode<T>::setNext(DoubleNode<T>* nextNode) {
    next = nextNode;
}
