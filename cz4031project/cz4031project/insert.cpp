#include "bpp.h"
#include "mem_pool.h"

using namespace std;

// Function to implement the Insert
// Operation in B+ Tree
void BPTree::insertValue(int x, void* address)
{
	// If root is null then return
	// Check for null root, and create a new one
	// newly created node
	if (rootNode == NULL) {
		rootNode = new Node;
		rootNode->IS_LEAF = true;
		rootNode->key[0] = x;
		rootNode->dbptr[0] = address;
		rootNode->size = 1;

	}

	// Traverse the B+ Tree
	else {
		Node* cursor = rootNode;
		Node* parent{};

		// Till cursor reaches the
		// leaf node
		while (cursor->IS_LEAF == false) {

			parent = cursor;

			for (int i = 0; i < cursor->size; i++) {

				// If found the position
				// where we have to insert
				// node
				if (x < cursor->key[i]) {
					cursor = cursor->ptr[i];
					break;
				}

				// If reaches the end
				if (i == cursor->size - 1) {
					cursor = cursor->ptr[i + 1];
					break;
				}
			}
		}

		// If there's space on the leaf node, insert node there
		if (cursor->size < MAX) {
			int i = 0;
			while (x > cursor->key[i] && i < cursor->size) {
				i++;
			}

			// For every key above i, shift it forward
			for (int j = cursor->size; j > i; j--) {
				cursor->key[j] = cursor->key[j - 1];
				cursor->dbptr[j] = cursor->dbptr[j - 1];
			}

			// Start inserting the value in position i
			cursor->key[i] = x;
			cursor->dbptr[i] = address;
			cursor->size++;
			cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
			cursor->ptr[cursor->size - 1] = NULL;
		}

		// If the leaf node is full create a new Leaf node
		else {

			Node* newLeaf = new Node;

			// Create virtual node with extra capacity for new node
			int virtualNode[MAX + 1];
			void* virtualptr[MAX + 1];

			// Transfer everything from the current leaf node to virtual node
			for (int i = 0; i < MAX; i++) {
				virtualNode[i] = cursor->key[i];
				virtualptr[i] = cursor->dbptr[i];
			}
			int i = 0, j;

			// Find which position to insert new node
			while (x > virtualNode[i] && i < MAX) {
				i++;
			}

			// For every key above i, shift it forward
			for (int j = MAX + 1; j > i; j--) {
				virtualNode[j] = virtualNode[j - 1];
				virtualptr[j] = virtualptr[j - 1];
			}

			// Initialize the new leaf node and split old node
			virtualNode[i] = x;
			virtualptr[i] = address;
			newLeaf->IS_LEAF = true;

			cursor->size = (MAX + 1) / 2;
			newLeaf->size = MAX + 1 - (MAX + 1) / 2;

			cursor->ptr[cursor->size] = newLeaf;

			newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];

			cursor->ptr[MAX] = NULL;

			// Give the virtual node's keys to cursor node
			for (i = 0; i < cursor->size; i++) {
				cursor->key[i] = virtualNode[i];
				cursor->dbptr[i] = virtualptr[i];
			}

			// Give the virtual node's keys to new leaf node
			for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
				newLeaf->key[i] = virtualNode[j];
				newLeaf->dbptr[i] = virtualptr[j];
			}

			// If cursor is the root node
			if (cursor == rootNode) {

				// Create a new root Node
				Node* newRootNode = new Node;

				// Update rest field of
				// B+ Tree Node
				newRootNode->IS_LEAF = false;
				newRootNode->key[0] = newLeaf->key[0];
				newRootNode->ptr[0] = cursor;
				newRootNode->ptr[1] = newLeaf;
				newRootNode->size = 1;
				rootNode = newRootNode;
			}
			else {

				// Recursive Call for
				// insert in internal
				insertInternal(newLeaf->key[0], parent, newLeaf);
			}
		}
	}
}

// Function to implement the Insert
// Internal Operation in B+ Tree
void BPTree::insertInternal(int x, Node* cursor, Node* childNode)
{

	// Check for overflow
	if (cursor->size < MAX) {
		int i = 0;

		// Look for position to insert the key
		while (x > cursor->key[i] && i < cursor->size) {
			i++;
		}

		// For every key above i, shift it forward
		for (int j = cursor->size; j > i; j--) {
			cursor->key[j] = cursor->key[j - 1];
		}

		// For every pointer above i, shift it forward
		for (int j = cursor->size + 1; j > i + 1; j--) {
			cursor->ptr[j] = cursor->ptr[j - 1];
		}

		// Insert the key at i
		cursor->key[i] = x;
		cursor->size++;
		cursor->ptr[i + 1] = childNode;
	}

	else {
		// For new Internal node in the case of overflow
		Node* newInternal = new Node;

		// Create new virtual node to with space for the key
		int virtualKey[MAX + 1];
		Node* virtualPtr[MAX + 2];

		// Copy all the keys to virtual node
		for (int i = 0; i < MAX; i++) {
			virtualKey[i] = cursor->key[i];
		}

		// Copy all the pointers to virtual node
		for (int i = 0; i < MAX + 1; i++) {
			virtualPtr[i] = cursor->ptr[i];
		}

		int i = 0, j;

		// Look for position to insert the key
		while (x > virtualKey[i] && i < MAX) {
			i++;
		}

		// For every key above i, shift it forward
		for (int j = MAX + 1; j > i; j--) {
			virtualKey[j] = virtualKey[j - 1];
		}

		virtualKey[i] = x;

		// For every pointer above i, shift it forward
		for (int j = MAX + 2; j > i + 1; j--) {
			virtualPtr[j] = virtualPtr[j - 1];
		}

		virtualPtr[i + 1] = childNode;
		newInternal->IS_LEAF = false;

		// Split internal node
		cursor->size = (MAX + 1) / 2;
		newInternal->size = MAX - (MAX + 1) / 2;

		// Copy keys from virtual node to the new internal node
		for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++) {
			newInternal->key[i] = virtualKey[j];
		}

		// Copy pointers from virtual node to the new internal node
		for (i = 0, j = cursor->size + 1; i < newInternal->size + 1; i++, j++) {
			newInternal->ptr[i] = virtualPtr[j];
		}

		// If cursor is the root node
		if (cursor == rootNode) {

			// Create a new root node
			Node* newRootNode = new Node;

			// Update the tree
			newRootNode->IS_LEAF = false;
			newRootNode->key[0] = cursor->key[cursor->size];
			newRootNode->ptr[0] = cursor;
			newRootNode->ptr[1] = newInternal;
			newRootNode->size = 1;
			rootNode = newRootNode;
		}

		else {
			// Recursive Call to do the same for higher internal nodes
			insertInternal(cursor->key[cursor->size], searchParent(rootNode, cursor), newInternal);
		}
	}
}