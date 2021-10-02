#include "bpp.h"
#include "mem_pool.h"

using namespace std;

// Function to find any element
// in B+ Tree
void BPTree::findValue(int x)
{

	// Return empty tree
	if (rootNode == NULL) {
		cout << "tree is empty at the moment, please insert keys\n";
	}

	else {
		Node* cursor = rootNode;
		int count = 0;
		int blockcount = 0;


		// Traverse until leaf node
		while (cursor->IS_LEAF == false) {

			// Check in the cursor node
			for (int i = 0; i < cursor->size; i++) {

				// If end of node is reached, go to the child
				if (i == cursor->size - 1) {
					cursor = cursor->ptr[i + 1];
					if (cursor->IS_LEAF != true) {
						count++;
					}
					if ((count <= 5) && (cursor->IS_LEAF != true)) {
						cout << "The " << count << "th internal node accessed is ";
						for (int i = 0; i < cursor->size; i++) {
							cout << cursor->key[i] << ",";
						}

					}
					break;
				}

				if (x >= cursor->key[i]) {
					continue;
				}

				// If x is less than the key i, it must be in the node in pointer i
				else if (x < cursor->key[i]) {
					cursor = cursor->ptr[i];
					if (cursor->IS_LEAF != true) {
						count++;
					}
					if ((count <= 5) && (cursor->IS_LEAF != true)) {
						cout << "\n" << "The " << count << "th internal node accessed is ";
						for (int i = 0; i < cursor->size; i++) {
							cout << cursor->key[i] << ",";
						}

					}
					break;
				}

			}
		}
		cout << "\n" << "The total number of index nodes/ internal nodes(exclusive of root and leaf) accessed is " << count << "\n";

		// Search for the value in the leaf
		vector<int> averagerating;
		vector<void*>visited;
		Record* record;
		for (int i = 0; i < cursor->size; i++) {

			// If found then return
			if (cursor->key[i] == x) {
				cout << "the value has been found\n";
				void* blkaddress = cursor->dbptr[i];
				blockcount++;
				//if not visited before, then continue accessing
				visited.push_back(blkaddress);
				record = (Record*)blkaddress;
				cout << record->numVotes;
				cout << "The " << "visited" << " block content 's tconst values are ";
				for (int i = 0; i < 5; i++) {
					cout << record->tconst << ",";

					if (record->numVotes == x) {
						averagerating.push_back(record->averageRating);
					}

					blkaddress = (char*)blkaddress + 20;
				}

				if (cursor->key[i + 1] != x) {
					break;
				}


				if (i == (cursor->size - 1)) {
					if (cursor->key[i] == x) {
						cursor = cursor->ptr[cursor->size - 1];
						//call function recursively
					}
				}


				return;
			}
		}

		// Else element is not present
		cout << "the value cannot be found in the tree\n";
	}
}