#include "bpp.h"
#include "mem_pool.h"

using namespace std;

// Function to find any element
// in B+ Tree
void BPTree::findValue(int x)
{
	// Return empty tree
	if (rootNode == NULL) {
		cout << "Tree is empty at the moment, please insert keys\n";
		return;
	}

	else {
		Node* cursor = rootNode;
		int nodecount = 0;
		int blockcount = 0;

		// Traverse until leaf node
		while (cursor->IS_LEAF == false) {

			// Check in the cursor node
			for (int i = 0; i < cursor->size; i++) {

				// If end of node is reached, go to the child
				if (i == cursor->size - 1) {
					cursor = cursor->ptr[i + 1];
					if (cursor->IS_LEAF != true) {
						nodecount++;
					}
					if ((nodecount <= 5) && (cursor->IS_LEAF != true)) {
						cout << "The " << nodecount << "th internal node accessed is ";
						for (int i = 0; i < cursor->size; i++) {
							cout << cursor->key[i] << ",";
						}
					}
					break;
				}

				// If x is less than the key i, it must be in the node in pointer i
				if (x < cursor->key[i]) {
					cursor = cursor->ptr[i];
					if (cursor->IS_LEAF != true) {
						nodecount++;
					}
					
					// It is sufficient to report for the first 5 index nodes or data blocks only if there are more than 5
					// I'll re-implement this part later after discussing with Luke
					if ((nodecount <= 5) && (cursor->IS_LEAF != true)) {
						cout << "\n" << "The " << nodecount << "th internal node accessed is ";

						// Print all values in the current node
						for (int i = 0; i < cursor->size; i++) {
							cout << cursor->key[i] << ",";
						}
					}
					break;
				}

			}
		}
		cout << "\n" << "The total number of index nodes/internal nodes(exclusive of root and leaf) accessed is " << nodecount << "\n";

		// Search for the value in the leaf node reached
		vector<int> averagerating;
		vector<void*>visited;
		Record* record;

		for (int i = 0; i < cursor->size; i++) {

			// If found then return
			if (cursor->key[i] == x) {
				cout << "The value " << x << " has been found!\n";
				void* blkaddress = cursor->dbptr[i];
				cout << "The cursor key is " << cursor->key[i] << " and the dbptr is " << cursor->dbptr[i] << "\n";
				cout << "The block address is: " << blkaddress << "\n";
				blockcount++;
				//if not visited before, then continue accessing
				visited.push_back(blkaddress);
				record = (Record*)blkaddress;
				//Record *temp_loc = record;
				//memcpy(temp_loc, blkaddress, 20);
				//uintptr_t p = cursor->dbptr[i];
				//int value = *reinterpret_cast<int*>(p);
			//	cout << temp_loc;
				cout << "The average rating for record with tconst " << record->tconst << " is " << record->averageRating;
				cout << "\nThe " << "visited" << " block content's tconst values are ";
				//display(cursor);
				for (int i = 0; i < 5; i++) {
//					cout << record->tconst << ", ";

//					if (record->numVotes == x) {
	//					cout << " with numvotes " << record->numVotes << " and with rating " << record->averageRating << "\n";
						//averagerating.push_back(record->averageRating);
	//				}

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

		//for (int i = 0; i < visited.size(); i++)
		//	cout << "visited vector " << i << ": " << visited.at(i);

		// Else element is not present
		cout << "The value cannot be found in the tree\n";
		return;
	}
}