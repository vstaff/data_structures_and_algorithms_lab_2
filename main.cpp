#include <iostream>
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

	Group(Degree p_degree, int unsigned p_number[4]) {
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

	Node* rotateRight(Node* node) {
		Node* leftChild = node->left;
		Node* leftChildRightGrandchild = leftChild->right;

		// Perform rotation
		leftChild->right = node;
		node->left = leftChildRightGrandchild;

		// Update heights
		node->height = max(getHeight(node->left), getHeight(node->right)) + 1;
		leftChild->height = max(getHeight(leftChild->left), getHeight(leftChild->right)) + 1;

		// Return new root
		return leftChild;
	}

	Node* leftRotate(Node* node)
	{
		Node* rightChild = node->right;
		Node* rightChildLeftGrandchild = rightChild->left;

		rightChild->left = node;
		node->right = rightChildLeftGrandchild;

		// Update heights
		node->height
			= max(getHeight(node->left), getHeight(node->right)) + 1;
		rightChild->height
			= max(getHeight(rightChild->left), getHeight(rightChild->right)) + 1;

		// Return new root
		return rightChild;
	}

};