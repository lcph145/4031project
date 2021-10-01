#include <climits>
#include <fstream>
#include <iostream>
#include <sstream>
using namespace std;
const int MAX = 3;

struct key {
	int value;

};


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
	void adjustNode(Node*, string);
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
// Constructor of Node
Node::Node()
{
	key = new int[MAX];
	ptr = new Node * [MAX + 1];
}

// Initialise the BPTree Node
BPTree::BPTree()
{
	rootNode = NULL;
}

void BPTree::delNode(Node* cursor) {
	delete[] cursor->key;
	delete[] cursor->ptr;
	delete cursor;
}

void BPTree::adjustNode(Node* cursor, string op) {
	if (op == "+") {
		cursor->size++;
		cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
		cursor->ptr[cursor->size - 1] = NULL;
	}
	else if (op == "-") {
		cursor->size--;
		cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
		cursor->ptr[cursor->size + 1] = NULL;
	}
}


void BPTree::findValue(int x)
{
	if (rootNode != NULL) {
		Node* cursor = rootNode;

		// Traverse until leaf node
		while (cursor->IS_LEAF == false) {

			// Check in the cursor node
			for (int i = 0; i < cursor->size; i++) {

				// If end of node is reached, go to the child
				if (i == cursor->size - 1) {
					cursor = cursor->ptr[i + 1];
					break;
				}

				if (x >= cursor->key[i]) {
					continue;
				}

				// If x is less than the key i, it must be in the node in pointer i
				else if (x < cursor->key[i]) {
					cursor = cursor->ptr[i];
					break;
				}

			}
		}

		// Search for the value in the leaf
		for (int i = 0; i < cursor->size; i++) {

			// If found then return
			if (cursor->key[i] == x) {
				cout << "Value found\n";
				return;
			}
		}

		// Else element is not present
		cout << "Unable to find value in the tree\n";
	}
}

// Function to implement the Insert
// Operation in B+ Tree
void BPTree::insertValue(int x)
{
	if (rootNode == NULL) {
		rootNode = new Node;
		rootNode->IS_LEAF = true;
		rootNode->key[0] = x;
		rootNode->size = 1;
		return;
	}

	// Traverse the B+ Tree
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
		}

		// Start inserting the value in position i
		cursor->key[i] = x;
		adjustNode(cursor, "+");
	}

	// If the leaf node is full create a new Leaf node
	else {

		Node* newLeaf = new Node;

		// Create virtual node with extra capacity for new node
		int virtualNode[MAX + 1];

		// Transfer everything from the current leaf node to virtual node
		for (int i = 0; i < MAX; i++) {
			virtualNode[i] = cursor->key[i];
		}
		int i = 0, j;

		// Find which position to insert new node
		while (x > virtualNode[i] && i < MAX) {
			i++;
		}

		// For every key above i, shift it forward
		for (int j = MAX + 1; j > i; j--) {
			virtualNode[j] = virtualNode[j - 1];
		}

		// Initialize the new leaf node and split old node
		virtualNode[i] = x;
		newLeaf->IS_LEAF = true;

		cursor->size = (MAX + 1) / 2;
		newLeaf->size = MAX + 1 - (MAX + 1) / 2;

		cursor->ptr[cursor->size] = newLeaf;

		newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX];

		cursor->ptr[MAX] = NULL;

		// Give the virtual node's keys to cursor node
		for (i = 0; i < cursor->size; i++) {
			cursor->key[i] = virtualNode[i];
		}

		// Give the virtual node's keys to new leaf node
		for (i = 0, j = cursor->size; i < newLeaf->size; i++, j++) {
			newLeaf->key[i] = virtualNode[j];
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

// Function to find the parent node
Node* BPTree::searchParent(Node* cursor, Node* childNode)
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

// Function to get the root Node
Node* BPTree::getRoot()
{
	return rootNode;
}

void BPTree::remove(int x)
{
	if (rootNode == NULL)
	{
		cout << "Tree empty\n";
	}
	else
	{
		Node* cursor = rootNode;
		Node* parent{};

		int leftSibling, rightSibling;
		// in the following while loop, cursor will will travel to the leaf node possibly consisting the key
		// Travel to leaf node while looking for key
		while (cursor->IS_LEAF == false)
		{
			for (int i = 0; i < cursor->size; i++)
			{
				parent = cursor;

				// Check the key
				if (x < cursor->key[i])
				{
					// Store siblings for borrowing later
					leftSibling = i - 1;
					rightSibling = i + 1;
					cursor = cursor->ptr[i];
					break;
				}
				// If end of node is reached, go to i+1 pointer's node
				else if (i == cursor->size - 1)
				{
					// Store siblings for borrowing later
					leftSibling = i;
					rightSibling = i + 2;
					cursor = cursor->ptr[i + 1];
					break;
				}
			}
		}

		// Check if key exists
		for (int pos = 0; pos < cursor->size; pos++)
		{
			if (cursor->key[pos] == x)
			{
				// Delete the key
				for (int i = pos; i < cursor->size; i++)
				{
					cursor->key[i] = cursor->key[i + 1];
				}
				cursor->size--;
				cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
				cursor->ptr[cursor->size + 1] = NULL;
				// Pointers not needed if node is root and leaf
				if (cursor == rootNode)
				{
					cout << "Deleted " << x << " from leaf node successfully\n";
					for (int i = 0; i <= MAX; i++)
					{
						cursor->ptr[i] = NULL;
					}
					if (cursor->size == 0)
					{
						cout << "F\n";
						delNode(cursor);
						rootNode = NULL;
					}
					return;
				}
				
				cout << "Deleted " << x << " from leaf node successfully\n";
				// Check if key minimum is met (underflow)
				if (cursor->size >= (MAX + 1) / 2)
				{
					return;
				}
				cout << "Underflow in leaf node!\n";
				// Try to borrow from siblings
				if (leftSibling >= 0)
				{
					Node* leftNode = parent->ptr[leftSibling];
					// Check if sibling will have enough after transfer
					if (leftNode->size >= (MAX + 1) / 2 + 1)
					{
						// Shift following keys forward to make space
						for (int i = cursor->size; i > 0; i--)
						{
							cursor->key[i] = cursor->key[i - 1];
						}
						adjustNode(cursor, "+");

						// Transfer
						cursor->key[0] = leftNode->key[leftNode->size - 1];
						adjustNode(leftNode, "-");
						// Change parent to reflect first value of node
						parent->key[leftSibling] = cursor->key[0];
						cout << "Transferred " << cursor->key[0] << " from left sibling of leaf node\n";
						return;
					}
				}
				if (rightSibling <= parent->size)
				{
					Node* rightNode = parent->ptr[rightSibling];
					if (rightNode->size >= (MAX + 1) / 2 + 1)
					{
						adjustNode(cursor, "+");

						cursor->key[cursor->size - 1] = rightNode->key[0];
						adjustNode(rightNode, "-");

						// Fix right sibling after transferring
						for (int i = 0; i < rightNode->size; i++)
						{
							rightNode->key[i] = rightNode->key[i + 1];
						}

						parent->key[rightSibling - 1] = rightNode->key[0];
						cout << "Transferred " << cursor->key[cursor->size - 1] << " from right sibling of leaf node\n";
						return;
					}
				}
				// Unable to borrow from siblings (case 3), merge nodes
				if (leftSibling >= 0)
				{
					Node* leftNode = parent->ptr[leftSibling];
					// Transfer all keys to left node since values are ascending order
					for (int i = leftNode->size, j = 0; j < cursor->size; i++, j++)
					{
						leftNode->key[i] = cursor->key[j];
					}

					leftNode->size += cursor->size;
					leftNode->ptr[leftNode->size] = cursor->ptr[cursor->size];
					cout << "Merging two leaf nodes\n";
					removeInternal(parent->key[leftSibling], parent, cursor);
					delNode(cursor);
				}
				else if (rightSibling <= parent->size)
				{
					Node* rightNode = parent->ptr[rightSibling];
					// Transfer all keys to current node since values are ascending order
					for (int i = cursor->size, j = 0; j < rightNode->size; i++, j++)
					{
						cursor->key[i] = rightNode->key[j];
					}

					cursor->size += rightNode->size;
					cursor->ptr[cursor->size] = rightNode->ptr[rightNode->size];
					cout << "Merging two leaf nodes\n";
					removeInternal(parent->key[rightSibling - 1], parent, rightNode);
					delNode(rightNode);
				}
			}
		}


		
	}
}
void BPTree::removeInternal(int x, Node* cursor, Node* child)
{
	// If root only has 1 key left, no more use for it
	if (cursor == rootNode)
	{
		if (cursor->size == 1) {
			if (cursor->ptr[1] == child)
			{
				delNode(child);
				rootNode = cursor->ptr[0];
				delNode(cursor);
				return;
			}
			else if (cursor->ptr[0] == child)
			{
				delNode(child);
				rootNode = cursor->ptr[1];
				delNode(cursor);
				return;
			}
		}
	}
	int pos;
	for (pos = 0; pos < cursor->size; pos++)
	{
		if (cursor->key[pos] == x)
		{
			break;
		}
	}
	for (int i = pos; i < cursor->size; i++)
	{
		cursor->key[i] = cursor->key[i + 1];
	}
	//now deleting the pointer child
	for (pos = 0; pos < cursor->size + 1; pos++)
	{
		if (cursor->ptr[pos] == child)
		{
			break;
		}
	}
	for (int i = pos; i < cursor->size + 1; i++)
	{
		cursor->ptr[i] = cursor->ptr[i + 1];
	}
	cursor->size--;
	if (cursor->size >= (MAX + 1) / 2 - 1)//no underflow
	{
		cout << "Deleted " << x << " from internal node successfully\n";
		return;
	}
	cout << "Underflow in internal node!\n";
	//underflow, try to transfer first
	if (cursor == rootNode)return;
	Node* parent = searchParent(rootNode, cursor);
	int leftSibling, rightSibling;
	//finding left n right sibling of cursor
	for (pos = 0; pos < parent->size + 1; pos++)
	{
		if (parent->ptr[pos] == cursor)
		{
			leftSibling = pos - 1;
			rightSibling = pos + 1;
			break;
		}
	}
	//try to transfer
	if (leftSibling >= 0)//if left sibling exists
	{
		Node* leftNode = parent->ptr[leftSibling];
		//check if it is possible to transfer
		if (leftNode->size >= (MAX + 1) / 2)
		{
			//make space for transfer of key
			for (int i = cursor->size; i > 0; i--)
			{
				cursor->key[i] = cursor->key[i - 1];
			}
			//transfer key from left sibling through parent
			cursor->key[0] = parent->key[leftSibling];
			parent->key[leftSibling] = leftNode->key[leftNode->size - 1];
			//transfer last pointer from leftnode to cursor
			//make space for transfer of ptr
			for (int i = cursor->size + 1; i > 0; i--)
			{
				cursor->ptr[i] = cursor->ptr[i - 1];
			}
			//transfer ptr
			cursor->ptr[0] = leftNode->ptr[leftNode->size];
			cursor->size++;
			leftNode->size--;
			cout << "Transferred " << cursor->key[0] << " from left sibling of internal node\n";
			return;
		}
	}
	if (rightSibling <= parent->size)//check if right sibling exist
	{
		Node* rightNode = parent->ptr[rightSibling];
		//check if it is possible to transfer
		if (rightNode->size >= (MAX + 1) / 2)
		{
			//transfer key from right sibling through parent
			cursor->key[cursor->size] = parent->key[pos];
			parent->key[pos] = rightNode->key[0];
			for (int i = 0; i < rightNode->size - 1; i++)
			{
				rightNode->key[i] = rightNode->key[i + 1];
			}
			//transfer first pointer from rightnode to cursor
			//transfer ptr
			cursor->ptr[cursor->size + 1] = rightNode->ptr[0];
			for (int i = 0; i < rightNode->size; ++i)
			{
				rightNode->ptr[i] = rightNode->ptr[i + 1];
			}
			cursor->size++;
			rightNode->size--;
			cout << "Transferred " << cursor->key[0] << " from right sibling of internal node\n";
			return;
		}
	}
	//transfer wasnt posssible hence do merging
	if (leftSibling >= 0)
	{
		//leftnode + parent key + cursor
		Node* leftNode = parent->ptr[leftSibling];
		leftNode->key[leftNode->size] = parent->key[leftSibling];
		for (int i = leftNode->size + 1, j = 0; j < cursor->size; j++)
		{
			leftNode->key[i] = cursor->key[j];
		}
		for (int i = leftNode->size + 1, j = 0; j < cursor->size + 1; j++)
		{
			leftNode->ptr[i] = cursor->ptr[j];
			cursor->ptr[j] = NULL;
		}
		leftNode->size += cursor->size + 1;
		cursor->size = 0;
		//delete cursor
		removeInternal(parent->key[leftSibling], parent, cursor);
		cout << "Merged with left sibling\n";

	}
	else if (rightSibling <= parent->size)
	{
		//cursor + parent key + rightnode
		Node* rightNode = parent->ptr[rightSibling];
		cursor->key[cursor->size] = parent->key[rightSibling - 1];
		for (int i = cursor->size + 1, j = 0; j < rightNode->size; j++)
		{
			cursor->key[i] = rightNode->key[j];
		}
		for (int i = cursor->size + 1, j = 0; j < rightNode->size + 1; j++)
		{
			cursor->ptr[i] = rightNode->ptr[j];
			rightNode->ptr[j] = NULL;
		}
		cursor->size += rightNode->size + 1;
		rightNode->size = 0;
		//delete cursor
		removeInternal(parent->key[rightSibling - 1], parent, rightNode);
		cout << "Merged with right sibling\n";
	}
}




int main() {
	BPTree node;
	node.insertValue(5);
	node.insertValue(15);
	node.insertValue(25);
	node.insertValue(35);
	node.insertValue(45);
	node.insertValue(55);
	node.insertValue(40);
	node.insertValue(30);
	node.insertValue(20);
	node.display(node.getRoot());

	node.remove(15);
	node.display(node.getRoot());
}