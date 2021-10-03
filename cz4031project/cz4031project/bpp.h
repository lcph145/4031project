#pragma once
#include <vector>
#include <iostream>

const int MAX = 5;

class Node {
	bool IS_LEAF;
	int* key, size;
	Node** ptr;
	friend class BPTree;
	void** dbptr;

public:
	Node();
};

// BP tree
class BPTree {
	Node* rootNode;
	void insertInternal(int, Node*, Node*);
	Node* searchParent(Node*, Node*);
	void removeNonLeaf(int, Node*, Node*);
	void delNode(Node*);
	void adjustNode(Node*, std::string);
public:
	BPTree();
	int getheight(Node* cursor);
	int returnheight();
	int returnMax();
	std::vector<int> returnRootvalue();
	std::vector<int> returnfirstchild();
	void findValue(int);
	void findValuerange(int x, int y);
	void insertValue(int, void*);
	void display(Node*);
	int remove(int);
	Node* getRoot();
	int countNodes(Node* cursor, int count);

};