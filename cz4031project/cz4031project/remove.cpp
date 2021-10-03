#include "bpp.h"
#include "mem_pool.h"

using namespace std;

int BPTree::remove(int x)
{
	if (rootNode == NULL)
	{
		cout << "Empty tree\n";
		return 0;
	}
	else
	{
		Node* cursor = rootNode;
		Node* parent{};
		int leftNeighbour, rightNeighbour;
		// in the following while loop, cursor will will travel to the leaf node possibly consisting the key
		// Travel to leaf node while looking for key
		while (cursor->IS_LEAF == false)
		{
			for (int i = 0; i < cursor->size; i++)
			{
				parent = cursor;

				// Store the keys of the parent's neighbours
				leftNeighbour = i - 1; // leftNeighbour is the index of left Neighbour in the parent node
				rightNeighbour = i + 1; // rightNeighbour is the index of right Neighbour in the parent node

				// Check the key
				if (x < cursor->key[i])
				{
					cursor = cursor->ptr[i];
					break;
				}
				// If end of node is reached, go to i+1 pointer's node
				if (i == cursor->size - 1)
				{
					leftNeighbour = i;
					rightNeighbour = i + 2;
					cursor = cursor->ptr[i + 1];
					break;
				}
			}
		}
		// Search for key in the current leaf
		bool found = false;
		int position;
		// Check if key exists
		for (position = 0; position < cursor->size; position++)
		{
			if (cursor->key[position] == x)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			return 0;
		}

		// Delete the key
		for (int i = position; i < cursor->size; i++)
		{
			cursor->key[i] = cursor->key[i + 1];
		}
		cursor->size--;
		// Pointers not needed if node is root and leaf
		if (cursor == rootNode)
		{
			cout << "Deleted " << x << "\n";
			for (int i = 0; i <= MAX; i++)
			{
				cursor->ptr[i] = NULL;
			}
			if (cursor->size == 0)
			{
				cout << "Nothing left in the tree\n";
				delNode(cursor);
				rootNode = NULL;
			}
			return 1;
		}
		// Shift the pointer of i+1 to i
		cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
		cursor->ptr[cursor->size + 1] = NULL;
		cout << "Deleted " << x << "\n";
		// Check if key minimum is met (underflow)
		if (cursor->size >= (MAX + 1) / 2)
		{
			return 1;
		}
		// Try to borrow from Neighbour
		if (leftNeighbour >= 0)
		{
			Node* leftNode = parent->ptr[leftNeighbour];
			// Check if neighbour will have enough after transfer
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
				parent->key[leftNeighbour] = cursor->key[0];
				return 1;
			}
		}
		if (rightNeighbour <= parent->size)
		{
			Node* rightNode = parent->ptr[rightNeighbour];
			if (rightNode->size >= (MAX + 1) / 2 + 1)
			{
				adjustNode(cursor, "+");

				cursor->key[cursor->size - 1] = rightNode->key[0];
				adjustNode(rightNode, "-");

				// Fix right neighbour after transferring
				for (int i = 0; i < rightNode->size; i++)
				{
					rightNode->key[i] = rightNode->key[i + 1];
				}

				parent->key[rightNeighbour - 1] = rightNode->key[0];
				return 1;
			}
		}
		// Unable to borrow from neighbour (case 3), merge nodes
		if (leftNeighbour >= 0)
		{
			Node* leftNode = parent->ptr[leftNeighbour];
			// Transfer all keys to left node since values are ascending order
			for (int i = leftNode->size, j = 0; j < cursor->size; i++, j++)
			{
				leftNode->key[i] = cursor->key[j];
			}

			leftNode->size += cursor->size;
			leftNode->ptr[leftNode->size] = cursor->ptr[cursor->size];
			removeNonLeaf(parent->key[leftNeighbour], parent, cursor);
			delNode(cursor);
			return 1;
		}
		else if (rightNeighbour <= parent->size)
		{
			Node* rightNode = parent->ptr[rightNeighbour];
			// Transfer all keys to current node since values are ascending order
			for (int i = cursor->size, j = 0; j < rightNode->size; i++, j++)
			{
				cursor->key[i] = rightNode->key[j];
			}

			cursor->size += rightNode->size;
			cursor->ptr[cursor->size] = rightNode->ptr[rightNode->size];
			// Update parents of the merge
			removeNonLeaf(parent->key[rightNeighbour - 1], parent, rightNode);
			delNode(rightNode);
			return 1;
		}
	}
}

void BPTree::removeNonLeaf(int x, Node* cursor, Node* child)
{
	// If root only has 1 key left, pass on root status
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
	// Searching for the child
	int position;
	for (position = 0; position < cursor->size; position++)
	{
		if (cursor->key[position] == x)
		{
			break;
		}
	}
	for (int i = position; i < cursor->size; i++)
	{
		cursor->key[i] = cursor->key[i + 1];
	}
	// Deleting
	for (position = 0; position < cursor->size + 1; position++)
	{
		if (cursor->ptr[position] == child)
		{
			break;
		}
	}
	for (int i = position; i < cursor->size + 1; i++)
	{
		cursor->ptr[i] = cursor->ptr[i + 1];
	}
	cursor->size--;

	// Check for underflow condition
	if (cursor->size >= (MAX + 1) / 2 - 1)
	{
		return;
	}
	//Borrow from neighbours
	if (cursor == rootNode)return;
	Node* parent = searchParent(rootNode, cursor);
	int leftNeighbour, rightNeighbour;

	for (position = 0; position < parent->size + 1; position++)
	{
		if (parent->ptr[position] == cursor)
		{
			leftNeighbour = position - 1;
			rightNeighbour = position + 1;
			break;
		}
	}

	if (leftNeighbour >= 0)
	{
		Node* leftNode = parent->ptr[leftNeighbour];

		if (leftNode->size >= (MAX + 1) / 2)
		{
			// Make space 
			for (int i = cursor->size; i > 0; i--)
			{
				cursor->key[i] = cursor->key[i - 1];
			}
			// Transfer contents of key and pointer
			cursor->key[0] = parent->key[leftNeighbour];
			parent->key[leftNeighbour] = leftNode->key[leftNode->size - 1];
			for (int i = cursor->size + 1; i > 0; i--)
			{
				cursor->ptr[i] = cursor->ptr[i - 1];
			}
			cursor->ptr[0] = leftNode->ptr[leftNode->size];
			cursor->size++;
			leftNode->size--;
			return;
		}
	}
	if (rightNeighbour <= parent->size)
	{
		Node* rightNode = parent->ptr[rightNeighbour];
		if (rightNode->size >= (MAX + 1) / 2)
		{
			// Transfer contents of key and pointer
			cursor->key[cursor->size] = parent->key[position];
			parent->key[position] = rightNode->key[0];
			for (int i = 0; i < rightNode->size - 1; i++)
			{
				rightNode->key[i] = rightNode->key[i + 1];
			}
			cursor->ptr[cursor->size + 1] = rightNode->ptr[0];
			for (int i = 0; i < rightNode->size; ++i)
			{
				rightNode->ptr[i] = rightNode->ptr[i + 1];
			}
			cursor->size++;
			rightNode->size--;
			return;
		}
	}
	// Merging
	if (leftNeighbour >= 0)
	{
		// Transfering to left neighbour from cursor
		Node* leftNode = parent->ptr[leftNeighbour];
		leftNode->key[leftNode->size] = parent->key[leftNeighbour];
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
		// Update parents of the merge
		removeNonLeaf(parent->key[leftNeighbour], parent, cursor);
	}
	else if (rightNeighbour <= parent->size)
	{
		// Transfering to cursor from right neighbour
		Node* rightNode = parent->ptr[rightNeighbour];
		cursor->key[cursor->size] = parent->key[rightNeighbour - 1];
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
		// Update parents of the merge
		removeNonLeaf(parent->key[rightNeighbour - 1], parent, rightNode);
	}
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