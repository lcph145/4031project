#include "bpp.h"
#include "mem_pool.h"

#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

// Constructor of Node
Node::Node()
{
	key = new int[MAX];
	ptr = new Node * [MAX + 1];
	dbptr = new void* [MAX + 1];
}

// Initialise the BPTree Node
BPTree::BPTree()
{
	rootNode = NULL;
}

// Function to find the parent node
Node* BPTree::searchParent(Node* cursor,
	Node* childNode)
{
	Node* parent{};

	// Ignore last level as leaf nodes cannot be parents
	if (cursor->IS_LEAF) {
		return NULL;
	}

	// Traverse the current node with
	// all its child
	for (int i = 0; i < cursor->size + 1; i++) {
		// If child node is found in the cursor's pointers, the parent is found
		if (cursor->ptr[i] == childNode) {
			parent = cursor;
			return parent;
		}

		// If cannot find, do a recursive call
		else {
			parent = searchParent(cursor->ptr[i], childNode);

			// If parent is found, return it
			if (parent != NULL) {
				return parent;
			}
		}
	}

	return parent;
}


int BPTree::countNodes(Node* cursor, int count) {
	if ((cursor->IS_LEAF == true)&&(cursor!=nullptr)) {
		count = count + 1;
		return count;
	}

	else if((cursor->IS_LEAF == false) && (cursor != nullptr)){
		for (int i = 0;i < cursor->size + 1;i++) {
			cursor = cursor->ptr[i];
			count++;
			count = countNodes(cursor, count) + count;
			return count;
		}

	}

}


int BPTree::getheight(Node* cursor) {
	if (cursor->IS_LEAF == false) {
		return getheight(cursor->ptr[0]) + 1;
	}
	else if (cursor->IS_LEAF == true) {
		return 1;
	}
	else {
		return NULL;
	}
}

int BPTree::returnheight() {
	return (getheight(rootNode));
}

int BPTree::returnMax() {
	return MAX; 
}


vector<int> BPTree::returnRootvalue() {
	vector<int> vec{};
	int* curr= rootNode->key;
	int size = rootNode->size;
	for (int i = 0;i < rootNode->size;i++) {
		vec.insert(vec.begin(), rootNode->key[i]);
	}
	return vec;
}

vector<int> BPTree::returnfirstchild() {
	vector<int> vec{};
	Node* cursor = rootNode->ptr[0];
	int size = cursor->size;
	for (int i = 0;i < cursor->size;i++) {
		vec.insert(vec.begin(), cursor->key[i]);
	}
	return vec;
}


// Print the tree
void BPTree::display(Node* cursor) {
	if (cursor != NULL) {
		for (int i = 0; i < cursor->size; i++) {
			cout << cursor->key[i] << " ";
		}
		cout << "\n";
		if (cursor->IS_LEAF != true) {
			for (int i = 0; i < cursor->size + 1; i++) {
				display(cursor->ptr[i]);
			}
		}
	}
}

// Function to get the root Node
Node* BPTree::getRoot()
{
	return rootNode;
}


// Driver Code

