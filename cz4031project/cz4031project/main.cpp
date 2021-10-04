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
	inFile.open("data2.tsv");
	if (!inFile) {
		cerr << "Unable to open file";
		exit(1);   // call system to stop
	}

	mem_pool mem_obj(100000000, 500); //initialise the mem pool with 100MB and 100 bytes

	cout << "current block size is " << mem_obj.getBlkSize()<<"bytes. \n";

	std::string str;

	mem_obj.allocateblock(); 
	BPTree node;
	std::getline(inFile, str); //skip first line
	while (std::getline(inFile, str)) {
		Record record;
		istringstream stream(str);

		stream >> record.tconst >> record.averageRating >> record.numVotes; //reading each entry in the datafile into structures
		cout << "tconst " << record.tconst << " has average rating " << record.averageRating << " with numVotes " << record.numVotes << "\n";
	
		blockaddress = mem_obj.addRecord(&record, sizeof(record));
		node.insertValue(record.numVotes, blockaddress);
		//cout << blockaddress << "\n";
	}

	cout << "\n-------------------- Experiment 1 answers --------------------" << "\n\n";
	cout << "Memory Pool Size in MB is " << mem_obj.getSizeofDB() << "\n";
	cout << "The block size is " << mem_obj.getBlkSize() << " bytes\n";
	cout << "The number of blocks allocated is " << mem_obj.getnumberofBlocks() << "\n";
	cout << "Current memory used = Blocks used multiplied by blocksize = " << (mem_obj.getBlkSize()) * mem_obj.getnumberofBlocks() << " bytes\n" << '\n';


	inFile.close();


	/* Experiment 2 */
	node.display(node.getRoot());
	cout << "\n-------------------- Experiment 2 answers --------------------" << "\n\n";
	cout << "The height of tree is " << node.returnheight() << "\n";
	cout << "The maximum number of keys that can be stored per node is " << node.returnMax() << "\n";
	cout << "The number of nodes used is: " << node.countNodes(node.getRoot(), 1)<< "\n";
	cout << "The content of root node is";
	vec = node.returnRootvalue();
	reverse(vec.begin(), vec.end());
	for (auto i = vec.begin(); i < vec.end(); i++)
	{
		cout << " " << *i;
	}
	vec = node.returnfirstchild();
	reverse(vec.begin(), vec.end());
	cout << "\n";
	cout << "The content of first child node is";
	for (auto i = vec.begin(); i < vec.end(); i++)
	{
		cout << " " << *i;
	}
	cout << " \n ";


	/* Experiment 3 */
	cout << "\n-------------------- Experiment 3 answers --------------------"<<"\n";
	node.findValue(25);

	
	/* Experiment 4 */
	cout << "\n-------------------- Experiment 4 answers --------------------" << "\n";
	node.findValuerange(25, 40);


	/* Experiment 5 */
	cout << "\n-------------------- Experiment 5 answers --------------------\n\n";
	int output = 1;
	int delCount = 0;
	while (output == 1) {
		output = node.remove(25);
		delCount++;
	}
	delCount--;
	cout << "\nThe number of nodes used is: " << node.countNodes(node.getRoot(), 1) << "\n";
	cout << "The height of tree is " << node.returnheight() << "\n";
	cout << "The number of times a node is merged or deleted is " << delCount << "\n";
	vec = node.returnRootvalue();
	reverse(vec.begin(), vec.end());
	cout << "The content of the root node is";
	for (auto i = vec.begin(); i < vec.end(); i++)
	{
		cout << " " << *i;
	}
	vec = node.returnfirstchild();
	reverse(vec.begin(), vec.end());
	cout << "\nThe content of first child node is";
	for (auto i = vec.begin(); i < vec.end(); i++)
	{
		cout << " " << *i;
	}
	cout << " \n ";


	return 0;
}