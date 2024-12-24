#ifndef DLL_H
#define DLL_H

class DLLNode {
public:
	int data;
	DLLNode* prev;
	DLLNode* next;

	DLLNode(int p_data);
};

class DLL {
public:
	DLLNode* head;

	DLL();

	void add(int value);

	void deleteAfter(int target);

	void deleteAll(int target);

	void clear();

	//DLL unite(DLL pair);

	bool includes(int target);

	void print();
};

DLL unite(DLL list1, DLL list2);

#endif