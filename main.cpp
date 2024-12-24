#include <iostream>
#include <fstream>

#include "avl_tree.h"


int main() {
    std::ifstream file("./input1.txt");
    AVLTree tree;

    if (!file.is_open()) {
        std::cout << "there is no file with such name\n";
        return 0;
    }

    std::string line;

    while (std::getline(file, line)) {
        if (!isKeyValid(line)) {
            continue;
        }

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
        std::cout << "has created " << degree  << number[0] << number[1] << number[2] << number[3] << std::endl;
        tree.insert(key);
    }

    tree.traverse();
    file.close();
}