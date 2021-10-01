#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include "mem_pool.h"
#include "bpp.h"

using namespace std;

struct Record
{
	char tconst[10];
	int numVotes;
	float averageRating;
};



int main() {
	int numVotes = 0;
	float averageRating = 0;
	string tconst = " ";
	void* address;
	vector <int>vec;

	ifstream inFile;
	inFile.open("Project 1 Data - Copy.tsv");
	if (!inFile) {
		cerr << "Unable to open file";
		exit(1);   // call system to stop
	}

	mem_pool mem_obj(100000, 100);

	cout << mem_obj.getBlkSize() << endl;

	std::string str;

	mem_obj.allocateblock();
	BPTree node;
	while (std::getline(inFile, str)) {
		Record record;
		istringstream stream(str);

		stream >> record.tconst >> record.averageRating >> record.numVotes;
		cout << record.tconst << " has " << record.averageRating << " plus " << record.numVotes << "\n";
	
		address = mem_obj.addRecord(&record, sizeof(record));
		node.insertValue(record.numVotes, address);
		cout << address<< "\n";
	}

	cout << " Expt 1 answers " << "\n";
	cout << " Memory Pool Size in MB is " << mem_obj.getSizeofDB() << "\n";
	cout << " The block size is " << mem_obj.getBlkSize() << "\n";
	cout << "The number of blocks allocated is " << mem_obj.getnumberofBlocks() << "\n";
	cout << "Current memory used= Blocks used multiplied by blocksize= " << (mem_obj.getBlkSize()) * mem_obj.getnumberofBlocks() << "\n" << '\n';


	inFile.close();


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
	cout << " The content of first child node is";
	for (auto i = vec.begin(); i < vec.end(); i++)
	{
		cout << " " << *i;
	}
	
		//node.remove(5);





		return 0;
}