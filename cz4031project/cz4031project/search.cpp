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
		int nodecount = 0; //counter for nodes
		int blockcount = 0;// counter for blocks accessedd

		// Traverse until leaf node
		while (cursor->IS_LEAF == false) {

			// Check in the cursor node
			for (int i = 0; i < cursor->size; i++) {


				// If x is less than the key i, it must be in the node in pointer i
				if (x < cursor->key[i]) {
					cursor = cursor->ptr[i];
					if (cursor->IS_LEAF != true) {
						nodecount++;
					}



					if ((nodecount <= 5) && (cursor->IS_LEAF != true)) {
						cout << "\nInternal node #" << nodecount << " accessed is " << cursor->key[0];

						// Print all values in the current node
						for (int i = 1; i < cursor->size; i++) {
							cout << ", " << cursor->key[i];
						}
					}
					break;
				}


				// If end of node is reached, go to the child
				if (i == cursor->size - 1) {
					cursor = cursor->ptr[i + 1];

					if (cursor->IS_LEAF != true) {
						nodecount++;
					}
					if ((nodecount <= 5) && (cursor->IS_LEAF != true)) {
						cout << "\nInternal node #" << nodecount << " accessed is " << cursor->key[0];
						for (int i = 1; i < cursor->size; i++) {
							cout << ", " << cursor->key[i];
						}
					}
					break;
				}
			}
		}
		cout << "\n" << "The total number of index nodes/internal nodes(exclusive of root and leaf) accessed is " << nodecount << "\n";

		// Search for the value in the leaf node reached
		float totalrating = 0;//sum total rating to find avg later on
		int numrating = 0; // the number to be divided by avg= totalrating/ numrating
		Record* record; // instantiate this so i can read each record
		int datablockcount = 0; //current data block count
		int iteration = 1; //number of times the recursion below is repeated.

		continuesearch(cursor,datablockcount, totalrating,x,iteration,numrating);
	}
}


void BPTree::continuesearch(Node* cursor,int datablockcount, float totalrating,int x,int iteration,int numrating)
{
	Record* record;
	//for (int i = 0; i < cursor->size; i++) {
		//cout << cursor->key[i] << "\n";
	//}

	for (int i = 0; i < cursor->size-1; i++) { //cant figure why size-1 works only
		if (cursor->key[i] == x) {
			cout << "The value " << x << " has been found!\n";
			void* blkaddress = cursor->dbptr[i];


			datablockcount++;
			if (datablockcount <= 5) {
				cout << "The block contains records with tconst of : ";
				for (int j = 0; j < 5; j++) {
					record = (Record*)blkaddress;
					cout << record->tconst << ", ";
					//cout << record->numVotes << ", ";
					//cout << record->averageRating << ", ";

					if (record->numVotes == x) {
						totalrating = totalrating + (record->averageRating);
						numrating++;

					}

					blkaddress = (char*)blkaddress + 20;

					//cout << blkaddress;
				}
			}
			}


			if (i == (cursor->size - 1)) {
					cursor = cursor->ptr[cursor->size - 1];
					iteration++;
					continuesearch(cursor, datablockcount, totalrating, x, iteration,numrating);
						

			}
			cout << "\n";
			if (cursor->key[i + 1] != x) {
				cout << "datablocks accessed is " << datablockcount<<"\n";
				cout <<"The average rating is:" << totalrating / numrating << "\n";
				return;
			}
			


		else if ((cursor->key[i] != x) && (i == cursor->size - 1) && (iteration == 1) && (datablockcount == 0)) {
				
			// Else element is not present
			cout << "The value cannot be found in the tree\n";
			cout << "The average rating is:" << totalrating / numrating << "\n";
			return;
		}

		else if ((iteration > 1)&&(cursor->key[i]!=x)&&(i==0)) {
			cout << "datablocks accessed is" << datablockcount;
			cout << "The average rating is:" << totalrating / numrating << "\n";
			return;
		}

		else if ((iteration > 1) && (cursor->key[i] != x) && (cursor->key[i-1] != x)) {
			cout << "datablocks accessed is" << datablockcount;
			cout << "The average rating is:" << totalrating / numrating << "\n";
			return;
		}

	}
}
	

// Function to find any element
// in B+ Tree
void BPTree::findRange(int x, int y)
{
	// Return empty tree
	if (rootNode == NULL) {
		cout << "Tree is empty at the moment, please insert keys\n";
		return;
	}

	else {
		Node* cursor = rootNode;
		Node* parent = rootNode;
		int nodecount = 0; //counter for nodes
		int blockcount = 0;// counter for blocks accessedd
		
		// Traverse until leaf node
		while (cursor->IS_LEAF == false) {

			// Check in the cursor node
			for (int i = 0; i < cursor->size; i++) {


				// If x is less than the key i, it must be in the node in pointer i
				if (x < cursor->key[i]) {
					Node* parent = cursor->ptr[cursor->size - 1];
					cursor = cursor->ptr[i];
					if (cursor->IS_LEAF != true) {
						nodecount++;
					}



					if ((nodecount <= 5) && (cursor->IS_LEAF != true)) {
						cout << "\nInternal node #" << nodecount << " accessed is " << cursor->key[0];

						// Print all values in the current node
						for (int i = 1; i < cursor->size; i++) {
							cout << ", " << cursor->key[i];
						}
					}
					break;
				}


				// If end of node is reached, go to the child
				if (i == cursor->size - 1) {
					cursor = cursor->ptr[i + 1];

					if (cursor->IS_LEAF != true) {
						nodecount++;
					}
					if ((nodecount <= 5) && (cursor->IS_LEAF != true)) {
						cout << "\nInternal node #" << nodecount << " accessed is " << cursor->key[0];
						for (int i = 1; i < cursor->size; i++) {
							cout << ", " << cursor->key[i];
						}
					}
					break;
				}
			}
		}
		cout << "\n" << "The total number of index nodes/internal nodes(exclusive of root and leaf) accessed is " << nodecount << "\n";

		// Search for the value in the leaf node reached
		float totalrating = 0;//sum total rating to find avg later on
		int numrating = 0; // the number to be divided by avg= totalrating/ numrating
		Record* record; // instantiate this so i can read each record
		int datablockcount = 0; //current data block count
		int iteration = 1; //number of times the recursion below is repeated.
		bool complete = false;
		

		while (!complete) {
			complete = continuerange(cursor, datablockcount, totalrating, y, iteration, numrating, y);
		}		
	}
}


bool BPTree::continuerange(Node* cursor, int datablockcount, float totalrating, int x, int iteration, int numrating, int y)
{
	Record* record;
	//for (int i = 0; i < cursor->size; i++) {
		//cout << cursor->key[i] << "\n";
	//}
	Node* parent = cursor->ptr[cursor->size - 1];

	for (int i = 0; i < cursor->size - 1; i++) { //cant figure why size-1 works only
		if (cursor->key[i] <= y) {
			cout << "The value " << cursor->key[i] << " has been found!\n";
			void* blkaddress = cursor->dbptr[i];

			datablockcount++;
			if (datablockcount <= 5) {
				cout << "The block contains records with tconst of : ";
				for (int j = 0; j < 5; j++) {
					record = (Record*)blkaddress;
					cout << record->tconst << ", ";
					//cout << record->numVotes << ", ";
					//cout << record->averageRating << ", ";

					if (record->numVotes == x) {
						totalrating = totalrating + (record->averageRating);
						numrating++;

					}

					blkaddress = (char*)blkaddress + 20;

					//cout << blkaddress;
				}
			}
		}


		if (i >= (cursor->size - 1)) {
			cursor = cursor->ptr[cursor->size];
			iteration++;
			continuerange(cursor, datablockcount, totalrating, x, iteration, numrating, y);
		}

		cout << "\n";
		if (cursor->key[i + 1] > y) {
			cout << "datablocks accessed is " << datablockcount << "\n";
			cout << "The average rating is:" << totalrating / numrating << "\n";
			return true;
		}

		else if ((cursor->key[i] > y) && (i == cursor->size - 1) && (iteration == 1) && (datablockcount == 0)) {

			// Else element is not present
			cout << "The value cannot be found in the tree\n";
			cout << "The average rating is:" << totalrating / numrating << "\n";
			return true;
		}

		else if ((iteration > 1) && (cursor->key[i] > y) && (i == 0)) {
			cout << "datablocks accessed is" << datablockcount;
			cout << "The average rating is:" << totalrating / numrating << "\n";
			return true;
		}

		else if ((iteration > 1) && (cursor->key[i] > y) && (cursor->key[i - 1] > y)) {
			cout << "datablocks accessed is" << datablockcount;
			cout << "The average rating is:" << totalrating / numrating << "\n";
			return true;
		}

	}
}


