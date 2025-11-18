#pragma once

template <typename T>
class NodeTree {
private:
    T data;
    NodeTree<T>* left;
    NodeTree<T>* right;

public:
    NodeTree();
    NodeTree(T value);

    T getData();
    void setData(T value);

    NodeTree<T>* getLeft();
    NodeTree<T>* getRight();
    void setLeft(NodeTree<T>* node);
    void setRight(NodeTree<T>* node);
};

template <typename T>
NodeTree<T>::NodeTree() {
    left = nullptr;
    right = nullptr;
}

template <typename T>
NodeTree<T>::NodeTree(T value) {
    data = value;
    left = nullptr;
    right = nullptr;
}

template <typename T>
T NodeTree<T>::getData() {
    return data;
}

template <typename T>
void NodeTree<T>::setData(T value) {
    data = value;
}

template <typename T>
NodeTree<T>* NodeTree<T>::getLeft() {
    return left;
}

template <typename T>
NodeTree<T>* NodeTree<T>::getRight() {
    return right;
}

template <typename T>
void NodeTree<T>::setLeft(NodeTree<T>* node) {
    left = node;
}

template <typename T>
void NodeTree<T>::setRight(NodeTree<T>* node) {
    right = node;
}
