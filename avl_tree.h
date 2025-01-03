#ifndef AVL_TREE_H
#define AVL_TREE_H

#include <iostream>
#include <string>

#include "dll.h"

enum Degree {
    B, // bakalavriat
    M, // magistratura
    S, // specialitet
    N, // not stated 
};

// structure representing key of avl tree nodes
struct Group {
public:
    Degree degree;
    unsigned int number[4];

    Group();

    Group(Degree p_degree, unsigned int p_number[4]);
};

int compare(Group g1, Group g2);

struct Node {
public:
    Group key;
    Node* left;
    Node* right;
    int height;

    // constantly stored balance (height(rightSubtree) - height(leftSubtree))
    int balance;

    // list of duplicates
    DLL duplicates;

    // when we encounter a duplicate
    void addDuplicate(int rowIndex);

    Node(Group p_key);
};

std::ostream& operator << (std::ostream& os, const Node& node);

class AVLTree {
private:
    Node* root;

    void RNL(Node* root);

    void freeMemory(Node* root);

    // print tree
    void print(Node* node, std::string indent, bool isRight);

    // insertion by Virt
    void searchAndInsert(Group key, Node* &node, bool &subtreeIncreased, int rowIndex);

    // deletion by Virt
    void balanceL(Node*& p, bool& h);

    void balanceR(Node*& p, bool& h);

    // deletion itself
    void virtDelete(Group x, Node*& p, bool& h);

    void virtDel(Node*& r, bool& h, Node*& q);

    void RNLInFile(std::ofstream& outputFile, Node* root);

    void printInFile(std::ofstream& outputFile, const Node* node, std::string indent, bool isRight);

public:
    AVLTree();

    void search(Group key);

    void traverse();

    // to traverse in file
    void traverseInFile(std::ofstream& outputFile);

    void print();

    void printInFile(std::ofstream& outputFile);

    // insertion by virt
    void searchAndInsert(Group key, int rowIndex);

    // deletion by Virt (public)
    void virtDelete(Group x) {
        bool h = false;
        virtDelete(x, root, h);
    }

    ~AVLTree();
};

bool isKeyValid(std::string key);

#endif