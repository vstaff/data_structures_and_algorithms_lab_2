#include <iostream>
#include <regex>
#include "avl_tree.h"

Group::Group() {
    degree = N;

    for (int i = 0; i < 4; ++i) {
        number[i] = 0;
    }
}

Group::Group(Degree p_degree, unsigned int p_number[4]) {
    degree = p_degree;

    for (int i = 0; i < 4; ++i) {
        number[i] = p_number[i];
    }
}

int compare(Group g1, Group g2) {
    if (g1.degree == N || g2.degree == N) {
        throw "degree is not stated";
    }

    if (g1.degree != g2.degree) {
        return (g1.degree > g2.degree) ? 1 : -1;
    }

    for (int i = 0; i < 4; ++i) {
        if (g1.number[i] > g2.number[i]) {
            return 1;
        }
        else if (g1.number[i] < g2.number[i]) {
            return -1;
        }
    }

    return 0;
}

Node::Node(Group p_key) {
    key = p_key;
    left = nullptr;
    right = nullptr;
    height = 1;
}

// add row index where we've found duplicate
void Node::addDuplicate(int rowIndex) {
    duplicates.add(rowIndex);
}

std::ostream& operator << (std::ostream& os, const Node& node) {
    char degree = 'N';

    switch (node.key.degree) {
    case B:
        degree = 'B';
        break;
    case M:
        degree = 'M';
        break;
    case S:
        degree = 'S';
        break;
    }

    std::string number = "";

    for (int i = 0; i < 4; ++i) {
        number += std::to_string(node.key.number[i]);
    }

    return os << degree + number;
}

// private methods

int AVLTree::getHeight(Node* node) {
    return node == nullptr ? 0 : node->height;
}

int AVLTree::getBalance(Node* node) {
    return node == nullptr ? 0 : getHeight(node->left) - getHeight(node->right);
}

Node* AVLTree::rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;
    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;

    return x;
}

Node* AVLTree::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = std::max(getHeight(x->left), getHeight(x->right)) + 1;
    y->height = std::max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

Node* AVLTree::insert(Node* node, Group key) {
    // if tree is empty just set new element as root of the tree
    if (node == nullptr)
        return new Node(key);

    // else we have to find place to insert new element 
    int comp = compare(key, node->key);
    if (comp < 0)
        // if new element is less than current node then recursively insert in the left subtree
        node->left = insert(node->left, key);
    else if (comp > 0)
        // if new element is greater than current node then recursively insert in the right subtree
        node->right = insert(node->right, key);
    else
        // if we key is a complete copy of the current node 

        return node;

    // after inserting all the nodes above the new one increase their height by 1
    node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

    // now we have to rebalance our tree
    int balance = getBalance(node);

    if (balance > 1 && compare(key, node->left->key) < 0)
        return rotateRight(node);

    if (balance < -1 && compare(key, node->right->key) > 0)
        return rotateLeft(node);

    if (balance > 1 && compare(key, node->left->key) > 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    if (balance < -1 && compare(key, node->right->key) < 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

Node* AVLTree::minValueNode(Node* node) {
    Node* current = node;

    while (current->left != nullptr)
        current = current->left;

    return current;
}

Node* AVLTree::deleteNode(Node* root, Group key) {
    if (root == nullptr)
        return root;

    int comp = compare(key, root->key);
    if (comp < 0)
        root->left = deleteNode(root->left, key);
    else if (comp > 0)
        root->right = deleteNode(root->right, key);
    else {
        if ((root->left == nullptr) || (root->right == nullptr)) {
            Node* temp = root->left ? root->left : root->right;

            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            }
            else
                *root = *temp;

            delete temp;
        }
        else {
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == nullptr)
        return root;

    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

void AVLTree::inOrder(Node* root) {
    if (root != nullptr) {
        inOrder(root->right);
        std::cout << *root << std::endl;
        inOrder(root->left);
    }
}

void AVLTree::freeMemory(Node* root) {
    if (root != nullptr) {
        freeMemory(root->left);
        freeMemory(root->right);
        delete root;
    }
}

// public methods

AVLTree::AVLTree() {
    root = nullptr;
}

void AVLTree::insert(Group key) {
    root = insert(root, key);
}

void AVLTree::deleteKey(Group key) {
    root = deleteNode(root, key);
}

void AVLTree::search(Group key) {
    Node* current = root;
    while (current != nullptr) {
        int comp = compare(key, current->key);
        if (comp == 0) {
            std::cout << "Found: ";
            std::cout << current;
            return;
        }
        current = (comp < 0) ? current->left : current->right;
    }
    std::cout << "Not Found" << std::endl;
}

void AVLTree::traverse() {
    inOrder(root);
}

AVLTree::~AVLTree() {
    freeMemory(root);
}

bool isKeyValid(std::string key) {
    const std::regex pattern("[BMS]\\d{4}");

    return regex_match(key, pattern);
}