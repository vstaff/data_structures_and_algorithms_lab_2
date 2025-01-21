#include <iostream>
#include <fstream>
#include "avl_tree.h"
#include "dll.h"

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
    std::ifstream inputFile("./input1.txt");
    AVLTree tree;

    if (!inputFile.is_open()) {
        std::cout << "there is no file with such name\n";
        return 0;
    }
    
    std::string line;
    int rowIndex = 0;

    std::cout << "inserting keys in the tree\n";

    while (std::getline(inputFile, line)) {
        ++rowIndex;

        if (!isKeyValid(line)) {
            std::cout << "incorrect key on line: " << rowIndex << std::endl << std::endl;
            continue;
        }

        Group key = getKey(line);
        Node node(key, rowIndex);
        
        std::cout << "inserting: " << node << std::endl;
        tree.searchAndInsert(key, rowIndex);
        tree.print();
        std::cout << std::endl;
    }
    inputFile.close();

    std::cout << "right to left traverse of the tree: " << std::endl;
    tree.traverse();

    //// here is demonstration of deleting node by its key
    Degree targetDegree = S;
    unsigned int targetNumber[4] = { 3, 8, 2, 0, };
    Group target(targetDegree, targetNumber);
    Node targetNode(target, 1);

    std::cout << "now let's delete node: " << targetNode << std::endl;

    tree.virtDelete(target, 1);
    tree.virtDelete(target, 9);
    tree.virtDelete(target, 100);
    tree.virtDelete(target, 13);

    std::cout << "from right to left traverse of the tree: " << std::endl;
    tree.traverse();

    std::cout << "that's what tree looks like: " << std::endl;
    tree.print();

    std::cout << "print tree in the output file\n";
    std::ofstream outputFile;
    outputFile.open("./output.txt");

    if (!outputFile.is_open()) {
        std::cout << "there is no file with such name\n";
        return 0;
    }

    tree.traverseInFile(outputFile);
    tree.printInFile(outputFile);

    //// sandbox 
    //AVLTree tree2;
    //Degree degree = M;
    //unsigned int number[5] = {4, 5, 1, 2};
    //Group key(degree, number);

    //tree2.virtDelete(key);

    //std::cout << "here is tree2: " << std::endl;
    //tree2.traverse();
    //tree2.print();

    //std::cout << "result of the search: " << std::endl;
    //tree.search(key);
}