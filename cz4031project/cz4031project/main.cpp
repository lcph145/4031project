#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "bpp.cpp"
#include "mem_pool.h"

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

	while (std::getline(inFile, str)) {
		Record record;
		istringstream stream(str);

		stream >> record.tconst >> record.averageRating >> record.numVotes;
		cout << record.tconst << " has " << record.averageRating << " plus " << record.numVotes << "\n";

		cout << mem_obj.addRecord(&record, sizeof(record)) << "\n";
	}

	cout << " Memory Pool Size in MB is " << mem_obj.getSizeofDB() << "\n";
	cout << " The block size is " << mem_obj.getBlkSize() << "\n";
	cout << "The number of blocks allocated is " << mem_obj.getnumberofBlocks() << "\n";
	cout << "Current memory used= Blocks used multiplied by blocksize= " << (mem_obj.getBlkSize()) * mem_obj.getnumberofBlocks() << "\n" << '\n';


	inFile.close();

		BPTree node;
		node.insertValue(5);
		node.insertValue(15);
		node.insertValue(25);
		node.insertValue(35);
		node.insertValue(45);
		node.insertValue(55);
		node.insertValue(40);
		node.insertValue(30);
		node.insertValue(20);
		node.display(node.getRoot());

		node.remove(5);
		node.display(node.getRoot());

	


	return 0;
}