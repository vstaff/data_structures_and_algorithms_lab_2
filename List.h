#ifndef LIST_H
#define LIST_H
#include <iostream>
using namespace std;

class DLLNode {
public:
	int data;
	DLLNode* prev;
	DLLNode* next;

	DLLNode(int p_data) {
		data = p_data;
		prev = nullptr;
		next = nullptr;
	}
};

class DLL {
public:
	DLLNode* head;

	DLL() {
		head = nullptr;
	}

	void add(int value) {
		DLLNode* newNode = new DLLNode(value);

		if (head == nullptr) {
			head = newNode;
		}

		else if (head->next == nullptr) {
			if (head->data <= newNode->data) {
				head->next = newNode;
				head->next->prev = head;
			}

			else {
				DLLNode* copy = head;
				head = newNode;
				head->next = copy;
				head->next->prev = head;
			}
		}

		else {
			DLLNode* currentNode = head;

			while (currentNode->next != nullptr && currentNode->data <= newNode->data) {
				currentNode = currentNode->next;
			}

			if (currentNode->prev == nullptr) {
				DLLNode* copy = head;
				head = newNode;
				head->next = copy;
				head->next->prev = head;
			}

			else if (currentNode->next == nullptr && currentNode->data <= newNode->data) {
				currentNode->next = newNode;
				currentNode->next->prev = currentNode;
			}

			else {
				DLLNode* prevNode = currentNode->prev;
				prevNode->next = newNode;
				newNode->prev = prevNode;
				newNode->next = currentNode;
				currentNode->prev = newNode;
			}
		}
	}

	void deleteAfter(int target) {
		if (head == nullptr) {
			return;
		}

		DLLNode* currentNode = head;

		while (currentNode->next != nullptr) {
			if (currentNode->data == target) {
				if (currentNode->next->next == nullptr) {
					delete currentNode->next;
					currentNode->next = nullptr;
					break;
				}
				else {
					DLLNode* copy = currentNode->next;
					currentNode->next->next->prev = currentNode;
					currentNode->next = copy->next;

					delete copy;
				}
			}

			currentNode = currentNode->next;
		}
	}

	void deleteAll(int target) {
		while (head != nullptr && head->data == target) {
			DLLNode* newHead = head->next;
			delete head;
			head = newHead;

			if (head != nullptr) {
				head->prev = nullptr;
			}
		}

		if (head == nullptr) {
			return;
		}

		DLLNode* currentNode = head->next;

		while (currentNode != nullptr) {
			if (currentNode->data == target) {
				DLLNode* newCurrentNode = currentNode->next;

				currentNode->prev->next = currentNode->next;

				if (currentNode->next != nullptr) {
					currentNode->next->prev = currentNode->prev;

				}
				delete currentNode;

				currentNode = newCurrentNode;

			}

			else {
				currentNode = currentNode->next;
			}
		}
	}
	void clear() {
		if (head == nullptr) {
			return;
		}

		DLLNode* nextNode = head;

		while (nextNode != nullptr) {
			DLLNode* temp = nextNode;
			nextNode = nextNode->next;
			delete temp;
		}

		head = nullptr;
	}

	DLL unite(DLL pair) {
		DLL newList;

		DLLNode* pointer1 = head;

		while (pointer1 != nullptr) {
			newList.add(pointer1->data);
			pointer1 = pointer1->next;
		}

		DLLNode* pointer2 = pair.head;

		while (pointer2 != nullptr) {
			newList.add(pointer2->data);
			pointer2 = pointer2->next;
		}

		return newList;
	}


	bool includes(int target) {
		DLLNode* currentNode = head;

		while (currentNode != nullptr) {
			if (currentNode->data == target) {
				return true;
			}

			currentNode = currentNode->next;
		}

		return false;
	}

	void print() {
		DLLNode* currentNode = head;

		while (currentNode != nullptr) {
			cout << currentNode->data << " ";
			currentNode = currentNode->next;
		}
		cout << "\n";
	}
};

DLL unite(DLL list1, DLL list2) {
	DLL newList = DLL();

	DLLNode* pointer1 = list1.head;

	while (pointer1 != nullptr) {
		newList.add(pointer1->data);
		pointer1 = pointer1->next;
	}

	DLLNode* pointer2 = list2.head;

	while (pointer2 != nullptr) {
		newList.add(pointer2->data);
		pointer2 = pointer2->next;
	}

	return newList;
}

#endif 