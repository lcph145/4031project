#include "bpp.h"
#include "mem_pool.h"

using namespace std;
const int MAX = 5;

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

				// Store the keys of the parent's siblings
				leftSibling = i - 1; // leftSibling is the index of left sibling in the parent node
				rightSibling = i + 1; // rightSibling is the index of right sibling in the parent node

				// Check the key
				if (x < cursor->key[i])
				{
					cursor = cursor->ptr[i];
					break;
				}
				// If end of node is reached, go to i+1 pointer's node
				if (i == cursor->size - 1)
				{
					leftSibling = i;
					rightSibling = i + 2;
					cursor = cursor->ptr[i + 1];
					break;
				}
			}
		}
		// Search for key in the current leaf
		bool found = false;
		int pos;
		// Check if key exists
		for (pos = 0; pos < cursor->size; pos++)
		{
			if (cursor->key[pos] == x)
			{
				found = true;
				break;
			}
		}
		if (!found)
		{
			cout << "Not found\n";
			return;
		}

		// Delete the key
		for (int i = pos; i < cursor->size; i++)
		{
			cursor->key[i] = cursor->key[i + 1];
		}
		cursor->size--;
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
		// Shift the pointer of i+1 to i
		cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
		cursor->ptr[cursor->size + 1] = NULL;
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
				cursor->size++;
				cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
				cursor->ptr[cursor->size - 1] = NULL;

				// Transfer
				cursor->key[0] = leftNode->key[leftNode->size - 1];
				leftNode->size--;
				leftNode->ptr[leftNode->size] = cursor;
				leftNode->ptr[leftNode->size + 1] = NULL;
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
				cursor->size++;
				cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
				cursor->ptr[cursor->size - 1] = NULL;

				cursor->key[cursor->size - 1] = rightNode->key[0];
				rightNode->size--;
				rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size + 1];
				rightNode->ptr[rightNode->size + 1] = NULL;

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
				cout << "Changed root node\n";
				return;
			}
			else if (cursor->ptr[0] == child)
			{
				delNode(child);
				rootNode = cursor->ptr[1];
				delNode(cursor);
				cout << "Changed root node\n";
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

void BPTree::delNode(Node* cursor) {
	delete[] cursor->key;
	delete[] cursor->ptr;
	delete cursor;
}