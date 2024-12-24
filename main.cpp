#include <iostream>
#include <fstream>
//#include <string>
#include <locale.h>
#include <regex>

using namespace std;

// node key field 1 - degree
enum Degree {
	B, 
	M, 
	S,
	N, // not stated 
};

// node key
struct Group {
public:
	// group contains degree and 4-digits number
	Degree degree;
	int unsigned number[4];

	Group() {
		degree = N;
		for (int i = 0; i < 4; ++i) {
			number[i] = 0;
		}
	}

	Group(Degree p_degree, int p_number[4]) {
		degree = p_degree;
		
		for (int i = 0; i < 4; ++i) {
			number[i] = p_number[i];
		}
	}
};

// compares two groups and returns:
// 1 if first group is greater than second one 
// 0 if first group and second one are equal
// -1 if first group is less than second one
int compare(Group g1, Group g2) {
	if (g1.degree == N || g2.degree == N) {
		throw "degree is not stated";
	}

	int g1Degree = g1.degree;
	int g2Degree = g2.degree;

	if (g1Degree != g2Degree) {
		return (g1Degree > g2Degree) ? 1 : -1;
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

// node of the tree
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

std::ostream& operator << (std::ostream& os, const Node& node) {
	char degree;

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

	string number;

	for (int i = 0; i < 4; ++i) {
		number += std::to_string(node.key.number[i]);
	}

	return os << degree << number;
}

class AVLTree {
private:
	Node* root;

	// height of the subtree which root is node 
	int getHeight(Node* node) {
		return node == nullptr ? 0 : node->height;
	}

	int getBalance(Node* node) {
		return node == nullptr ? 0 : getHeight(node->left) - getHeight(node->right);
	}

	void fixHeight(Node* &node) {
		int leftChildHeight = getHeight(node->left);
		int rightChildHeight = getHeight(node->right);
		node->height = max(leftChildHeight, rightChildHeight) + 1;
	}

	// right rotation
	Node* rotateRight(Node* &p) {
		Node* q = p->left;
		p->left = q->right;
		q->right = p;
		fixHeight(p);
		fixHeight(q);
		return q;
	}

	// left rotation 
	Node* rotateLeft(Node* &q) {
		Node* p = q->right;
		q->right = p->left;
		p->left = q;
		fixHeight(q);
		fixHeight(p);
		return p;
	}

	Node* balance(Node* &p)  {
		fixHeight(p);

		if (getBalance(p) == 2) {
			if (getBalance(p->right) < 0) {
				p->right = rotateLeft(p->right);
			}
			return rotateRight(p);
		}

		if (getBalance(p) == -2) {
			if (getBalance(p->left) > 0) {
				p->left = rotateRight(p->left);
			}
			return rotateLeft(p);
		}

		return p;
	}

	Node* insert(Node* &p, const Group k) {
		if (p == nullptr) {
			return new Node(k);
		}

		if (compare(k, p->key) == -1) {
			p->left = insert(p->left, k);
		}

		else {
			p->right = insert(p->right, k);
		}

		return balance(p);
	}

	Node* getMin(Node* &p) {
		return p->left != nullptr ? getMin(p->left) : p;
	}

	Node* removeMin(Node* &p) {
		if (p->left == nullptr) {
			return p->right;
		}

		p->left = removeMin(p->left);
		return balance(p);
	}

	Node* remove(Node* &p, Group k) // удаление ключа k из дерева p
	{
		if (p == nullptr) {
			return nullptr;
		}

		if (compare(k, p->key) == -1) {
			p->left = remove(p->left, k);
		}
		else if (compare(k, p->key) == 1) {
			p->right = remove(p->right, k);
		}


		else {
			Node* q = p->left;
			Node* r = p->right;
			delete p;
			if (!r) return q;
			Node* min = getMin(r);
			min->right = removeMin(r);
			min->left = q;
			return balance(min);
		}

		return balance(p);
	}

	// it may be incorrect 
	//Node* rotateRight(Node* node) {
	//	Node* leftChild = node->left;
	//	Node* leftChildRightGrandchild = leftChild->right;

	//	// Perform rotation
	//	leftChild->right = node;
	//	node->left = leftChildRightGrandchild;

	//	// Update heights
	//	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	//	leftChild->height = max(getHeight(leftChild->left), getHeight(leftChild->right)) + 1;

	//	// Return new root
	//	return leftChild;
	//}

	//Node* rotateLeft(Node* node)
	//{
	//	Node* rightChild = node->right;
	//	Node* rightChildLeftGrandchild = rightChild->left;

	//	rightChild->left = node;
	//	node->right = rightChildLeftGrandchild;

	//	// Update heights
	//	node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
	//	rightChild->height = max(getHeight(rightChild->left), getHeight(rightChild->right)) + 1;

	//	// Return new root
	//	return rightChild;
	//}

	//Node* insert(Node* node, Group key) {
	//	if (node == nullptr) {
	//		return new Node(key);
	//	}

	//	if (compare(key, node->key) == -1) {
	//		node->left = insert(node->left, key);
	//	}
	//	else if (compare(key, node->key) == 1) {
	//		node->right = insert(node->right, key);
	//	}
	//	else {
	//		return node;
	//	}

	//	// most likely i need to cut the code below
	//	// and past it in new method for balancing 
	//	node->height = 1 + max(getHeight(node->left), getHeight(node->right));

	//	// Get the balance factor of this ancestor node
	//	int balance = getBalance(node);

	//	// If this node becomes unbalanced, then there are 4
	//	// cases

	//	// Left Left Case
	//	if (balance > 1 && compare(key, node->left->key) == -1)
	//		return rotateRight(node);

	//	// Right Right Case
	//	if (balance < -1 && compare(key, node->right->key) == 1)
	//		return rotateLeft(node);

	//	// Left Right Case
	//	if (balance > 1 && compare(key, node->left->key) == 1) {
	//		node->left = rotateLeft(node->left);
	//		return rotateRight(node);
	//	}

	//	// Right Left Case
	//	if (balance < -1 && compare(key, node->right->key) == -1) {
	//		node->right = rotateRight(node->right);
	//		return rotateLeft(node);
	//	}

	//	return node;
	//}

	//// balancing tree after node inserting 
	//Node* balance(Node* node) {

	//}

	// центрированный 
	Node* LNR(Node* node) {
		if (node == nullptr) {
			return nullptr;
		}

		LNR(node->left);
		cout << *node << endl;
		LNR(node->right);
	}

	// прямой 
	Node* NLR(Node* node) {
		if (node == nullptr) {
			return nullptr;
		}

		cout << *node;
		NLR(node->left);
		NLR(node->right);
	}

	// обратный 
	Node* LRN(Node* node) {
		if (node == nullptr) {
			return nullptr;
		}

		LRN(node->left);
		LRN(node->right);
		cout << *node;
	}

public:
	AVLTree() {
		root = nullptr;
	}

	void insert(Group key) {
		root = insert(root, key);
	}

	void remove(Group key) {
		remove(root, key);
	}

	void LNR() {
		LNR(root);
	}

	void NLR() {
		NLR(root);
	}

	void LRN() {
		LRN(root);
	}
};

// method for validating input data in text file
bool validateInput(string fileName) {
	std::ifstream file(fileName);
	string line;

	if (file.is_open()) {
		while (std::getline(file, line)) {
			if (line.length() != 5) {
				cout << "каждый ключ должен состоять из 5 символов";
				file.close();
				return false;
			}

			char degree = line.at(0);
			string number = line.substr(1);

			if (degree != 'B' && degree != 'M' && degree != 'S') {
				cout << "первый символ ключа должен быть одной из перечисленных букв: B, M, S\n";
				file.close();
				return false;
				
			}

			for (int i = 0; i < 4; ++i) {
				if (!isdigit(number[i])) {
					cout << "последующие после первого символы ключа должны быть цифрами\n";
					file.close();
					return false;
				}
			}

			cout << degree << ':' << number[0] << number[1] << number[2] << number[3] << endl;
		}
	}
	else {
		cout << "нет файла с именем: " + fileName;
		return false;
	}

	file.close();
	return true;
}

bool validateKey(string key) {
	const regex pattern("[BMS]\\d{4}");
	
	return regex_match(key, pattern);
}

std::vector<int> getDigits(string str_digits) {
	vector<int> digits;

	for (char character : str_digits) {
		digits.push_back(character - '0');
	}

	return digits;
}

int main() {
	setlocale(LC_ALL, "Russian");

	AVLTree tree;

	std::ifstream file("./input.txt");
	string line;

	if (!file.is_open()) {
		cout << "нет файла с таким именем";
		return 0;
	}

	while (std::getline(file, line)) {
		if (!validateKey(line)) {
			cout << "некорректный ключ";
			return 0;
		}

		char degree = line.at(0);
		Degree groupDegree;

		switch (degree) {
		case 'B':
			groupDegree = B;
			break;
		
		case 'M':
			groupDegree = M;
			break; 

		case 'S':
			groupDegree = S;
			break;
		}

		string number = line.substr(1);
		int groupNumber[4];

		for (int i = 0; i < 4; ++i) {
			groupNumber[i] = number[i] - '0';
		}


		Group key = Group(groupDegree, groupNumber);

		tree.insert(key);
	}
}