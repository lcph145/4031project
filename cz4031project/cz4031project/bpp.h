#pragma once
#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>

class Node {
	bool IS_LEAF;
	int* key, size;
	Node** ptr;
	friend class BPTree;

public:
	Node();
};

// BP tree
class BPTree {
	Node* rootNode;
	void insertInternal(int, Node*, Node*);
	Node* searchParent(Node*, Node*);
	void removeInternal(int, Node*, Node*);
	void delNode(Node*);
public:
	BPTree();
	int getheight(Node* cursor);
	int returnheight();
	int returnMax();
	int returnnumnodes();
	void findValue(int);
	void insertValue(int);
	void display(Node*);
	void remove(int);
	Node* getRoot();
};