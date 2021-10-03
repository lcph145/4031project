#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "mem_pool.h"
#include "bpp.h"

using namespace std;


int main() {
	int numVotes = 0;
	float averageRating = 0;
	string tconst = " ";
	void* blockaddress;
	vector <int>vec;

	/* Experiment 1 */
	ifstream inFile;
	inFile.open("Project 1 Data - Copy.tsv");
	if (!inFile) {
		cerr << "Unable to open file";
		exit(1);   // call system to stop
	}

	mem_pool mem_obj(10000000, 100);

	cout << mem_obj.getBlkSize() << endl;

	std::string str;

	mem_obj.allocateblock();
	BPTree node;
	std::getline(inFile, str); //skip first line
	while (std::getline(inFile, str)) {
		Record record;
		istringstream stream(str);

		stream >> record.tconst >> record.averageRating >> record.numVotes;
		cout << "tconst " << record.tconst << " has average rating " << record.averageRating << " with numVotes " << record.numVotes << "\n";
	
		blockaddress = mem_obj.addRecord(&record, sizeof(record));
		node.insertValue(record.numVotes, blockaddress);
		//cout << blockaddress << "\n";
	}

	cout << "Expt 1 answers " << "\n";
	cout << "Memory Pool Size in MB is " << mem_obj.getSizeofDB() << "\n";
	cout << "The block size is " << mem_obj.getBlkSize() << "\n";
	cout << "The number of blocks allocated is " << mem_obj.getnumberofBlocks() << "\n";
	cout << "Current memory used = Blocks used multiplied by blocksize = " << (mem_obj.getBlkSize()) * mem_obj.getnumberofBlocks() << "\n" << '\n';


	inFile.close();


	/* Experiment 2 */
	cout << "\n";
	node.display(node.getRoot());
	cout << " Expt 2 answers " << "\n";
	cout << " The height of tree is " << node.returnheight() << "\n";
	cout << " The maximum number of keys that can be stored per node is " << node.returnMax() << "\n";
	cout << " The number of nodes used is: " << node.countNodes(node.getRoot(), 1)<< "\n";
	cout << " The content of root node is";
	vec = node.returnRootvalue();
	for (auto i = vec.begin(); i < vec.end(); i++)
	{
		cout << " " << *i;
	}
	vec = node.returnfirstchild();
	cout << "\n";
	cout << " The content of first child node is";
	for (auto i = vec.begin(); i < vec.end(); i++)
	{
		cout << " " << *i;
	}
	cout << " \n ";


	/* Experiment 3 */
	cout << "Experiment 3 answers "<<"\n";
	node.findValue(25);
	
	/* Experiment 4 */
	cout << "Experiment 4 answers " << "\n";
	//node.findvaluerange(15,20);


	/* Experiment 5 */
	cout << "Experiment 5 answers " << "\n";
	int output = 1;
	int delCount = 0;
	while (output == 1) {
		output = node.remove(1645);
		delCount++;
	}
	delCount--;
	cout << " The number of nodes used is: " << node.countNodes(node.getRoot(), 1) << "\n";
	cout << " The height of tree is " << node.returnheight() << "\n";
	cout << " The number of times a node is merged or deleted is " << delCount << "\n";

	//node.remove(5);





	return 0;
}