#include "bpp.h"
#include "mem_pool.h"

using namespace std;

// Function to find any element
// in B+ Tree
void BPTree::findValue(int x)
{
	int n = x - 1;
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
				if (n< cursor->key[i]) {
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
				if (i == cursor->size-1) {
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
		int iteration = 0; //number of times the recursion below is repeated.
		void* address;
		int j = 0;
		bool flag = true;

		/*for (int i = 0;i < cursor->size;i++) {
				cout << cursor->key[i]<<"\n";
		}*/

	


		bool contsearch = true; 
		int i = 0;
		while (contsearch) {
				if (cursor->key[i] == x) {
					j = i;
					flag = true;
					contsearch= false;
					//for (int i = 0;i < cursor->size;i++) {
						//cout << cursor->key[i] << "\n";
					//}

					break;
				}

				else if (cursor->key[i] > x) {
					flag = false;
					contsearch = false;
					cout << "value not found";
					break;



				}
				else if ((i == cursor->size - 1) && (cursor->key[i] < x)) {
					cursor = cursor->ptr[cursor->size];
					i = 0;
					continue;
				}
				else if (cursor->key[i] < x) {
					i++;
					continue;
				}

			
		}


		
		while (flag){
				

				//cout << "j value" << j <<"\n";
				address = cursor->dbptr[j];

				//for (int i = 0;i < cursor->size;i++) {
					//cout << cursor->key[i] << "\n";
				//}

				datablockcount++;
				if (datablockcount <= 5) {
					cout << "block's content: \n";
				}
				for (int i = 0;i < 5;i++) {
					record = (Record*)address;
					if (datablockcount <= 5) {
						cout << "tconst values for block are " << record->tconst;
					}
					if (record->numVotes == x) {
						totalrating += record->averageRating;
						numrating++;
					
					}
					cout << '\n';
					address = (char*)address + 20;
				}
			
				//cout << "cursor size is '\n'" << cursor->size;
				if (j == (cursor->size - 1)) {
					//cout << "connecting to new node '\n'";
					cursor = cursor->ptr[cursor->size];
					j = 0;
					if (cursor->key[j ] != x) {
						flag = false;
						break;
					}
					else {
						continue;
					}
				}
					
					
				else if((j<cursor->size-1)&& cursor->key[j+1] == x){
					j++;
				}

				else if ((j < cursor->size - 1) && cursor->key[j+1] != x) {
					break;
					flag = false;
				}
				

	

				
			

		}cout << "The total datablock count is: " << datablockcount<<"\n";
		 cout << "The average value of average rating is " << totalrating/numrating << "\n";
		




		
	} 
}



// Function to find any element
// in B+ Tree
void BPTree::findValuerange(int x,int y)

{
	int n = x - 1;
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
				if (n < cursor->key[i]) {
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
		int iteration = 0; //number of times the recursion below is repeated.
		void* address;
		int j = 0;
		bool flag = true;

		/*for (int i = 0;i < cursor->size;i++) {
				cout << cursor->key[i]<<"\n";
		}*/




		bool contsearch = true;
		int i = 0;
		while (contsearch) {
			if ((cursor->key[i] >= x) && (cursor->key[i] <= y)) {
				j = i;
				flag = true;
				contsearch = false;
				//for (int i = 0;i < cursor->size;i++) {
					//cout << cursor->key[i] << "\n";
			//	}

				break;
			}


			
			else if ((i == cursor->size - 1) && (cursor->key[i] < x)) {
				cursor = cursor->ptr[cursor->size];
				i = 0;
				continue;
			}
			else if (cursor->key[i] < x) {
				i++;
				continue;
			}


		}



		while (flag) {


			//cout << "j value" << j <<"\n";
			address = cursor->dbptr[j];

		//	for (int i = 0;i < cursor->size;i++) {
				//cout << cursor->key[i] << "\n";
		//	}

			datablockcount++;
			if (datablockcount <= 5) {
				cout << "block's content: \n";
			}
			for (int i = 0;i < 5;i++) {
				record = (Record*)address;
				if (datablockcount <= 5) {
					cout << "\n tconst values for block are " << record->tconst;
				}
				if (record->numVotes==cursor->key[i]) {
					totalrating += record->averageRating;
					numrating++;

				}
				//cout << '\n';
				address = (char*)address + 20;
			}

			//cout << "cursor size is '\n'" << cursor->size;
			if (j == (cursor->size - 1)) {
				//cout << "connecting to new node '\n'";
				cursor = cursor->ptr[cursor->size];
				j = 0;
				if (cursor->key[j] > y || cursor->key[j] < x) {
					flag = false;
					break;
				}
				else {
					continue;
				}
			}


			else if ((j < cursor->size - 1) && cursor->key[j + 1]<=y &&cursor->key[j+1]>=x) {
				j++;
			}

			else if (((j < cursor->size - 1) && cursor->key[j + 1] > y ) || ((j < cursor->size - 1) && ( cursor->key[j] < x))){
				break;
				flag = false;
			}







		}cout << "The total datablock count is: " << datablockcount << "\n";
		cout << "The average value of average rating is " << totalrating / numrating << "\n";






	}
}



