#include <iostream>
#include <regex>
#include <fstream>
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
    balance = 0;
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

    // for printing duplicates
    std::string duplicates = "";
    if (node.duplicates.head != nullptr) {
        duplicates += "(";

        DLLNode* currentNode = node.duplicates.head;

        while (currentNode != nullptr) {
            duplicates += std::to_string(currentNode->data);
            duplicates += ", ";
            currentNode = currentNode->next;
        }

        duplicates = duplicates.substr(0, duplicates.size() - 2) + ")";
    }

    return os << degree + number + duplicates;
}

void AVLTree::RNL(Node* root) {
    if (root != nullptr) {
        RNL(root->right);
        std::cout << *root << std::endl;
        RNL(root->left);
    }
}

void AVLTree::freeMemory(Node* root) {
    if (root != nullptr) {
        freeMemory(root->left);
        freeMemory(root->right);
        delete root;
    }
}

//print tree
void AVLTree::print(Node* node, std::string indent, bool isRight) {
    if (node != nullptr) {
        std::cout << indent;
        if (isRight) {
            std::cout << "R----";
            indent += "     ";
        }
        else {
            std::cout << "L----";
            indent += "|    ";
        }
        std::cout << *node << std::endl;
        print(node->left, indent, false);
        print(node->right, indent, true);
    }
}

// insertion by virt 
void AVLTree::searchAndInsert(Group x, Node*& p, bool& h, int rowIndex) {
    Node* p1;
    Node* p2;

    if (p == nullptr) {
        p = new Node(x);
        h = true;
    }

    else if (compare(p->key, x) == 1) {
        searchAndInsert(x, p->left, h, rowIndex);

        if (h) {
            if (p->balance == 1) {
                p->balance = 0;
                h = false;
            }

            else if (p->balance == 0) {
                p->balance = -1;
            }

            else {
                p1 = p->left;

                if (p1->balance == -1) {
                    p->left = p1->right;
                    p1->right = p;
                    p->balance = 0;
                    p = p1;
                }

                else {
                    p2 = p1->right;
                    p1->right = p2->left;
                    p2->left = p1;
                    p->left = p2->right;
                    p2->right = p;

                    if (p2->balance == -1) {
                        p->balance = 1;
                    }
                    else {
                        p->balance = 0;
                    }

                    if (p2->balance == 1) {
                        p1->balance = -1;
                    }
                    else {
                        p1->balance = 0;
                    }
                    p = p2;
                }
                p->balance = 0;
                h = false;
            }
        }
    }

    else if (compare(p->key, x) == -1) {
        searchAndInsert(x, p->right, h, rowIndex);
        if (h) {
            if (p->balance == -1) {
                p->balance = 0;
                h = false;
            }
            else if (p->balance == 0) {
                p->balance = 1;
            }
            else {
                p1 = p->right;

                if (p1->balance == 1) {
                    p->right = p1->left;
                    p1->left = p;
                    p->balance = 0;
                    p = p1;
                }

                else {
                    p2 = p1->left;
                    p1->left = p2->right;
                    p2->right = p1;
                    p->right = p2->left;
                    p2->left = p;

                    if (p2->balance == 1) {
                        p->balance = -1;
                    }
                    else {
                        p->balance = 0;
                    }

                    if (p2->balance == -1) {
                        p1->balance = 1;
                    }
                    else {
                        p1->balance = 0;
                    }
                    p = p2;
                }
                p->balance = 0;
                h = false;
            }
        }
    }
    else {
        p->addDuplicate(rowIndex);
    }
}

// deletion by Virt
void AVLTree::balanceL(Node*& p, bool& h) {
    Node* p1;
    Node* p2;

    // begining of the algorithm
    // h; уменьшилась левая ветвь
    if (p->balance == -1) {
        p->balance = 0;
    }

    else if (p->balance == 0) {
        p->balance = 1;
        h = false;
    }

    else {
        p1 = p->right;
        if (p1->balance >= 0) {
            p->right = p1->left;
            p1->left = p;
            if (p1->balance = 0) {
                p->balance = 1;
                p1->balance = -1;
                h = false;
            }
            else {
                p->balance = 0;
                p1->balance = 0;
            }
            // not sure about where this line must be 
            p = p1;
        }
        else {
            p2 = p1->left;
            p1->left = p2->right;
            p2->right = p1;
            p->right = p2->left;
            p2->left = p;
            if (p2->balance == 1) {
                p->balance = -1;
            } 
            else {
                p->balance = 0;
            }
            if (p2->balance == -1) {
                p1->balance = 1;
            }
            else {
                p1->balance = 0;
            }
            p = p2;
            p2->balance = 0;
        }
    }
}

void AVLTree::balanceR(Node*& p, bool& h) {
    Node* p1;
    Node* p2;
    // уменьшается левая ветвь
    if (p->balance == 1) { 
        p->balance = 0; 
    }
    else if (p->balance == 0) {
        p->balance = -1;
        h = false;
    }
    // bal = -1 остался только такой вариант
    else {
        p1 = p->left;
        if (p1->balance <= 0) {//rr rotation once
            p->left = p1->right;
            p1->right = p;
            if (p1->balance == 0) {
                p->balance = -1;
                p1->balance = 1;
                h = false;
            }
            else {
                p->balance = 0;
                p1->balance = 0;
            }
            p = p1;
        }
        else {
            p2 = p1->right;
            p1->right = p2->left;
            p2->left = p1;
            p->left = p2->right;
            p2->right = p;
            if (p2->balance == -1) { 
                p->balance = 1; 
            }
            else { 
                p->balance = 0; 
            }
            if (p2->balance == 1) { 
                p1->balance = -1; 
            }
            else { 
                p1->balance = 0;
            }
            p = p2;
            p2->balance = 0;
        }
    }
}

// deletion itself
void AVLTree::virtDelete(Group x, Node*& p, bool& h) {
    Node* q;

    if (p == nullptr) {
        return;
    }

    else if (compare(p->key, x) == 1) {
        virtDelete(x, p->left, h);

        if (h) {
            balanceL(p, h);
        }
    }

    else if (compare(p->key, x) == -1) {
        virtDelete(x, p->right, h);

        if (h) {
            balanceR(p, h);
        }
    }

    else {
        q = p;

        if (q->right == nullptr) {
            p = q->left;
            h = true;
        }

        else if (q->left == nullptr) {
            p = q->right;
            h = true;
        }

        else {
            virtDel(q->left, h, q);

            if (h) {
                balanceL(p, h);
            }
        }
    }
}

void AVLTree::virtDel(Node*& r, bool& h, Node*& q) {
    if (r->right != nullptr) {
        virtDel(r->right, h, q);

        if (h) {
            balanceR(r, h);
        }
    }

    else {
        q->key = r->key;
        q->duplicates = r->duplicates;
        q = r;
        r = r->left;
        h = true;
    }
}


// public methods

AVLTree::AVLTree() {
    root = nullptr;
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
    RNL(root);
}

void AVLTree::searchAndInsert(Group key, int rowIndex) {
    bool hasIncreased = false;
    searchAndInsert(key, root, hasIncreased, rowIndex);
}


// output in file
void AVLTree::RNLInFile(std::ofstream& outputFile, Node* root) {
    if (root != nullptr) {
        RNLInFile(outputFile, root->right);
        outputFile << *root << std::endl;
        RNLInFile(outputFile, root->left);
    }
}

void AVLTree::traverseInFile(std::ofstream& outputFile) {
    RNLInFile(outputFile, root);
}


void AVLTree::print() {
    print(root, "", true);
}

// to print in file 
void AVLTree::printInFile(std::ofstream& outputFile) {
    printInFile(outputFile, root, "", true);
}

void AVLTree::printInFile(std::ofstream& outputFile, const Node* node, std::string indent, bool isRight) {
    if (node != nullptr) {
        outputFile << indent;
        if (isRight) {
            outputFile << "R----";
            indent += "     ";
        }
        else {
            outputFile << "L----";
            indent += "|    ";
        }
        outputFile << *node << std::endl;
        printInFile(outputFile, node->left, indent, false);
        printInFile(outputFile, node->right, indent, true);
    }
}


AVLTree::~AVLTree() {
    freeMemory(root);
}

bool isKeyValid(std::string key) {
    const std::regex pattern("[BMS]\\d{4}");

    return regex_match(key, pattern);
}