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
	


void BPTree::findValuerange(int x, int y)//for expt 4, havent edited much
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

				// If x is less than the key i, it must be in the node in pointer i
				if (x < cursor->key[i]) {
					cursor = cursor->ptr[i];
					if (cursor->IS_LEAF != true) {
						nodecount++;
					}



					if ((nodecount <= 5) && (cursor->IS_LEAF != true)) {
						cout << "\n" << "The " << nodecount << "th internal node accessed is ";

						// Print all values in the current node
						for (int i = 0; i < cursor->size; i++) {
							cout << cursor->key[i] << ",";
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
						cout << "The " << nodecount << "th internal node accessed is :";
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
		float totalrating = 0;
		int numrating = 0;
		Record* record;
		int datablockcount = 0;
		int iteration = 1;

		continuesearchrange(cursor, datablockcount, totalrating, x, iteration, numrating,y);

	}

}


void BPTree::continuesearchrange(Node* cursor, int datablockcount, float totalrating, int x, int iteration, int numrating,int y)
{
	Record* record;
	//for (int i = 0; i < cursor->size; i++) {
		//cout << cursor->key[i] << "\n";
	//}

	for (int i = 0; i < cursor->size; i++) {

		// If found then return
		//cout << cursor->size;
		//cout << "\n";
		//cout << cursor->key[i];
		//cout << "\n";
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

			if (i == (cursor->size - 1)) {
				cursor = cursor->ptr[cursor->size - 1];
				iteration++;
				continuesearchrange(cursor, datablockcount, totalrating, x, iteration, numrating,y);



			}
			cout << "\n";
			if (cursor->key[i + 1] != x) {
				cout << "datablocks accessed is " << datablockcount << "\n";
				cout << "The average rating is:" << totalrating / numrating << "\n";
				return;
			}
		}


		else if ((cursor->key[i] != x) && (i == cursor->size - 1) && (iteration == 1) && (datablockcount == 0)) {

			// Else element is not present
			cout << "The value cannot be found in the tree\n";
			cout << "The average rating is:" << totalrating / numrating << "\n";
			return;
		}

		else if ((iteration > 1) && (cursor->key[i] != x) && (i == 0)) {
			cout << "datablocks accessed is" << datablockcount;
			cout << "The average rating is:" << totalrating / numrating << "\n";
			return;
		}

		else if ((iteration > 1) && (cursor->key[i] != x) && (cursor->key[i - 1] != x)) {
			cout << "datablocks accessed is" << datablockcount;
			cout << "The average rating is:" << totalrating / numrating << "\n";
			return;
		}
	}
}//for expt 5, havent edited much.




// Function to find any element
// in B+ Tree
void BPTree::rangedSearch(int x, int y)
{
	// Return empty tree
	if (rootNode == NULL) {
		cout << "Tree is empty at the moment, please insert keys\n";
		return;
	}

	else {
		Node* cursor = rootNode;
		int nodecount = 0; //counter for nodes
		int blockcount = 0; //counter for blocks accessed
		bool complete = false; //bool for if ranged query is complete

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

		// x has been found - continue search until y is found
		int i = 0;
		while (!complete) {
			// Found a key within range, now we need to iterate through the entire range until the upperBoundKey.
			if (cursor->key[i] > y)
			{
				complete = true;
				break;
			}
			if ((cursor->key[i] >= x) && (cursor->key[i] <= y))
			{
				// for displaying to output file
				cout << "Index node accessed. Content is -----";
				display(cursor);

				// Add new line for each leaf node's linked list printout.
				cout << "\ntconst for average rating: " << cursor->key[i] << " > ";
			}
			i++;
		}
		cout << "\n" << "The total number of index nodes/internal nodes(exclusive of root and leaf) accessed is " << nodecount << "\n";

		// Search for the value in the leaf node reached
		float totalrating = 0;//sum total rating to find avg later on
		int numrating = 0; // the number to be divided by avg= totalrating/ numrating
		Record* record; // instantiate this so i can read each record
		int datablockcount = 0; //current data block count
		int iteration = 1; //number of times the recursion below is repeated.

		continueRanged(cursor, datablockcount, totalrating, x, iteration, numrating, y);
	}
}


void BPTree::continueRanged(Node* cursor, int datablockcount, float totalrating, int x, int iteration, int numrating, int y)
{
	{
		Record* record;
		//for (int i = 0; i < cursor->size; i++) {
			//cout << cursor->key[i] << "\n";
		//}

		for (int i = 0; i < cursor->size - 1; i++) { //cant figure why size-1 works only
			if ((cursor->key[i] >= x) && (cursor->key[i] <= y)) {
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

						if ((record->numVotes >= x) && (record->numVotes <= y)) {
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
				continueRanged(cursor, datablockcount, totalrating, x, iteration, numrating, y);


			}
			cout << "\n";
			if (cursor->key[i + 1] > y) {
				cout << "Datablocks accessed is " << datablockcount << "\n";
				cout << "The average rating is:" << totalrating / numrating << "\n";
				return;
			}


			else if ((cursor->key[i] > y) && (i == cursor->size - 1) && (iteration == 1) && (datablockcount == 0)) {

				// Else element is not present
				cout << "The value cannot be found in the tree\n";
				cout << "The average rating is:" << totalrating / numrating << "\n";
				return;
			}

			else if ((iteration > 1) && (cursor->key[i] > y) && (i == 0)) {
				cout << "datablocks accessed is" << datablockcount;
				cout << "The average rating is:" << totalrating / numrating << "\n";
				return;
			}

			else if ((iteration > 1) && (cursor->key[i] > y)) {
				cout << "datablocks accessed is" << datablockcount;
				cout << "The average rating is:" << totalrating / numrating << "\n";
				return;
			}
		}
	}
}
