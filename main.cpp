#include <iostream>
#include <fstream>
#include <string>
#include <regex>
using namespace std;

// степень образования
enum Degree {
    B, // бакалавриат
    M, // магистратура 
    S, // специалитет
    N, // не указано
};

//// чтобы понятно отображались ключи 
//std::ostream& operator << (std::ostream& os, const Degree& degree) {
//    switch (degree) {
//    case B:
//        return os << "B";
//    case M:
//        return os << "M";
//    case S:
//        return os << "S";
//    }
//}

// код группы
struct Group {
public:
    Degree degree; // степень образования 
    unsigned int number[4]; // четырехзначный код образовательной программы

    Group() {
        degree = N;
        for (int i = 0; i < 4; ++i) {
            number[i] = 0;
        }
    }

    Group(Degree p_degree, unsigned int p_number[4]) {
        degree = p_degree;

        for (int i = 0; i < 4; ++i) {
            number[i] = p_number[i];
        }
    }
};

// для сравнения ключей
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

// узел дерева
struct Node {
public:
    Group key;
    Node* left;
    Node* right;
    int height;

    Node(Group p_key) {
        key = p_key;
        left = nullptr;
        right = nullptr;
        height = 1;
    }
};

// чтобы понятно отображались ключи 
std::ostream& operator << (std::ostream& os, const Node& node) {
    char degree = 'N';
    
    switch (node.key.degree) {
    case B:
        degree = 'B';
    case M:
        degree = 'M';
    case S:
        degree = 'S';
    }

    string number = "";

    for (int i = 0; i < 4; ++i) {
        number += std::to_string(node.key.number[i]);
    }

    return os << degree + number;
}

// само дерево
class AVLTree {
private:
    Node* root;

    int getHeight(Node* node) {
        return node == nullptr ? 0 : node->height;
    }

    int getBalance(Node* node) {
        return node == nullptr ? 0 : getHeight(node->left) - getHeight(node->right);
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;
        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(getHeight(x->left), getHeight(x->right)) + 1;
        y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

        return y;
    }

    // рекурсивный метод для вставки в дерево
    // в конечном счете возвращает корень дерева с обновленными вершинами
    Node* insert(Node* node, Group key) {
        // если дерево пустое просто добавляем элемент в качестве корня 
        if (node == nullptr) 
            return new Node(key);

        // иначе необходимо найти куда его нужно вставить в дерево
        int comp = compare(key, node->key);
        if (comp < 0)
            // если добавляемый элемент меньше текущего узла - переходим в левое поддерево
            node->left = insert(node->left, key);
        else if (comp > 0)
            // если добавляемый элемент больше текущего узла - переходим в правое поддерево
            node->right = insert(node->right, key);
        else
            return node;

        // после добавление, у вышестоящих вершин высота увеличивается на один
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));

        // необходимо выполнить балансировку дерева 
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

    // для нахождения минимального элемента 
    Node* minValueNode(Node* node) {
        Node* current = node;

        while (current->left != nullptr)
            current = current->left;

        return current;
    }

    Node* deleteNode(Node* root, Group key) {
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

        root->height = 1 + max(getHeight(root->left), getHeight(root->right));

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

    void inOrder(Node* root) {
        if (root != nullptr) {
            inOrder(root->right);
            cout << root->key << endl;
            inOrder(root->left);
        }
    }

    void freeMemory(Node* root) {
        if (root != nullptr) {
            freeMemory(root->left);
            freeMemory(root->right);
            delete root;
        }
    }

public:
    AVLTree() {
        root = nullptr;
    }

    void insert(Group key) {
        root = insert(root, key);
    }

    void deleteKey(Group key) {
        root = deleteNode(root, key);
    }

    void search(Group key) {
        Node* current = root;
        while (current != nullptr) {
            int comp = compare(key, current->key);
            if (comp == 0) {
                cout << "Found: ";
                cout << current;
                return;
            }
            current = (comp < 0) ? current->left : current->right;
        }
        cout << "Not Found" << endl;
    }

    void traverse() {
        inOrder(root);
    }

    ~AVLTree() {
        freeMemory(root);
    }
};

bool isKeyValid(string key) {
    const regex pattern("[BMS]\\d{4}");

    return regex_match(key, pattern);
}

int main() {
    std::ifstream file("./input.txt");
    AVLTree tree;

    if (!file.is_open()) {
        cout << "there is no file with such name\n";
        return 0;
    }

    string line;

    while (std::getline(file, line)) {
        if (!isKeyValid(line)) {
            continue;
        }

        Degree degree = N;

        switch (line.at(0)) {
        case 'B':
            degree = B;
            break;
        case 'M':
            degree = M;
            break;
        case 'S':
            degree = S;
            break;
        }

        //cout << degree;

        int unsigned number[4] = { 0, 0, 0, 0, };

        for (int i = 0; i < 4; ++i) {
            number[i] = line.substr(1).at(i) - '0';
            //cout << number[i];
        }

        //cout << "\n";

        Group key(degree, number);
        tree.insert(key);
    }

    tree.traverse();
    file.close();
}