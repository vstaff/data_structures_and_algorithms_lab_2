#include <iostream>
#include <fstream>

#include "avl_tree.h"

Group getKey(std::string line) {
    Degree degree = N;

    char lineDegree = line.at(0);

    switch (lineDegree) {
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

    int unsigned number[4] = { 0, 0, 0, 0, };
    const std::string lineNumber = line.substr(1);
    for (int i = 0; i < 4; ++i) {
        const unsigned int currentDigit = lineNumber[i] - '0';
        number[i] = currentDigit;
    }

    Group key(degree, number);
    return key;
}


int main() {
    std::ifstream file("./input1.txt");
    AVLTree tree;

    if (!file.is_open()) {
        std::cout << "there is no file with such name\n";
        return 0;
    }

    std::string line;
    int rowIndex = 0;

    while (std::getline(file, line)) {
        ++rowIndex;

        if (!isKeyValid(line)) {
            continue;
        }

        Group key = getKey(line);
        tree.insert(key, rowIndex);
    }

    tree.traverse();

    tree.print();
    file.close();
}