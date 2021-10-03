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

		for (int i = 0;i < cursor->size;i++) {
				cout << cursor->key[i]<<"\n";
		}

	


		bool contsearch = true; 
		int i = 0;
		while (contsearch) {
				if (cursor->key[i] == x) {
					j = i;
					flag = true;
					contsearch= false;
					for (int i = 0;i < cursor->size;i++) {
						cout << cursor->key[i] << "\n";
					}

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
				

				cout << "j value" << j <<"\n";
				address = cursor->dbptr[j];

				for (int i = 0;i < cursor->size;i++) {
					cout << cursor->key[i] << "\n";
				}

			
				cout << "block's content:";
				for (int i = 0;i < 5;i++) {
					record = (Record*)address;
					cout << "tconst values for block are " << record->tconst;
					if (record->numVotes == x) {
						totalrating += record->averageRating;
						numrating++;
					
					}
					cout << '\n';
					address = (char*)address + 20;
				}
			
				cout << "cursor size is '\n'" << cursor->size;
				if (j == (cursor->size - 1)) {
					cout << "connecting to new node '\n'";
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
				

			/*	if (j < cursor->size - 1 && cursor->key[j] != x) {
					cout << cursor->key[j];
					cout << "debug line 2";
					flag = false;
					//print all the stuff
					break;
				}*/


				
			

		}




		

		//continuesearch(cursor,datablockcount, totalrating,x,iteration,numrating);
	}
}


#include "bpp.h"
#include "mem_pool.h"

using namespace std;

// Function to find any element
// in B+ Tree
void BPTree::findValuerange(int x,int y)
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
		int iteration = 0; //number of times the recursion below is repeated.
		void* address;
		int j = 0;
		bool flag = true;


		for (int i = 0;i <= cursor->size;i++) {
			if ((cursor->key[i] >= x) &&(cursor->key[i]<y)) {// first the first value
				j = i;
				flag = true;
				break;
			}
			else if (i == cursor->size) {
				cout << "value not found";
			}

		}




		while (flag) {


			address = cursor->dbptr[j];
			cout << "block's content:";
			for (int i = 0;i < 5;i++) {
				record = (Record*)address;
				cout << record->tconst << "tconst values are \n";
				if ((record->numVotes >= x) && (record->numVotes <= y)) {
					totalrating += record->averageRating;
					numrating++;
				}
				address = (char*)address + 20;
			}
			j++;

			if (j == cursor->size ) {
				cout << "debug line";
				cursor = cursor->ptr[cursor->size];
				j = 0;

				if (!(cursor->key[j] >=x) && (!(cursor->key[j] <=y))){
					flag = false;
				}
				break;

			}


			else if (j < cursor->size - 1 && !(cursor->key[j] >= x) && (!(cursor->key[j] <= y))) {
				cout << cursor->key[j];
				cout << "debug line 2";
				flag = false;
				//print all the stuff
				break;
			}





		}






		//continuesearch(cursor, datablockcount, totalrating, x, iteration, numrating);
	}
}





/*void BPTree::continuesearch(Node* cursor, int datablockcount, float totalrating, int x, int iteration, int numrating)
 {
	Record* record;
	void* blkaddress;
	iteration++;
	if (iteration == 1) {
		for (int i = 0; i < cursor->size; i++) {
			//cout << cursor->key[i] << "\n";
			if (cursor->key[i] == x) {
				//cout << "The value " << x << " has been found in the leaf node!\n";
				blkaddress = cursor->dbptr[i];
				
				datablockcount++;
				if (datablockcount <= 5) {
					cout << "\n";
					cout << "data blocks access:";
				}
				for (int j = 0; j < 5; j++) {
					record = (Record*)blkaddress;
					cout << record->tconst << ", ";
					if (record->numVotes == x) {
						cout << record->averageRating<< ", ";
						//totalrating = totalrating + (record->averageRating);
						//numrating++;

					}

					blkaddress = (Record*)blkaddress + 20;
				}
				cout << numrating << "num rating\n";
				cout << totalrating << "total rating \n";


			}
		}
		if (cursor->key[cursor->size - 1] == x) {
			cursor = cursor->ptr[cursor->size];
			cout<<"new op" <<cursor->key[0];
			continuesearch(cursor, datablockcount, totalrating, x, iteration, numrating);
		}

		else if (datablockcount==0) {
			cout << "data cannot be found \n";
			return;
		}
		else if(cursor->key[cursor->size - 1] != x){
			//cout << numrating << "num rating\n";
		    //cout << totalrating << "total rating \n";
			cout << '\n';
			cout << "The amount of data blocks accessed is " << datablockcount <<'\n';
			cout<< "The average rating is "<<totalrating/numrating<<'\n';
			return;
			
		}
	}

	

	else if (iteration > 1) {
		//cout << "Entered anoter iteration \n";
		for (int i = 0; i < cursor->size; i++) {
			if (cursor->key[i] == x) {
				void* blkaddress = cursor->dbptr[i];
				record = (Record*)blkaddress;
				datablockcount++;
				if (datablockcount <= 5) {
					cout << "\n";
					cout << "data blocks access:";
				}
				for (int j = 0; j < 5; j++) {
					if (datablockcount <= 5) {
						cout << record->tconst << ", ";
					}

					if (record->numVotes == x) {
						totalrating = totalrating + (record->averageRating);
						numrating++;

					}

					blkaddress = (char*)blkaddress + 20;
				}
				if (cursor->key[i + 1] != x) {
					cout << "The amount of data blocks accessed is " << datablockcount << '\n';
					cout << "The average rating is " << totalrating / numrating << '\n';
					cout << "debug1";
					return;
				}

			}
			if (cursor->key[0]!=x) 
			{
				cout << "The amount of data blocks accessed is " << datablockcount << '\n';
				cout << "The average rating is " << totalrating / numrating << '\n';
				cout << "debug1"<< cursor->key[0];
				return;
			}
		}
		if (cursor->key[cursor->size - 1] == x) {
			cursor = cursor->ptr[cursor->size];
			//cout<<"new op" <<cursor->key[0];
			continuesearch(cursor, datablockcount, totalrating, x, iteration, numrating);
		}
	}


}*/
	
/*

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
}*/


