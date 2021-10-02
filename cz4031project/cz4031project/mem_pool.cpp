#include "mem_pool.h"

#include <iostream>
#include <string>

using namespace std;

mem_pool::mem_pool(std::size_t memPoolInitial, std::size_t blkSize)
{
	this->memPoolInitial = memPoolInitial;
	this->memPoolLeft = memPoolInitial;
	this->blkSize = blkSize;
	this->numBlockInitial = memPoolInitial / blkSize;
	this->numBlockUsed = 0;
	this->startpointer = operator new(memPoolInitial);
	this->currpointer = nullptr;
	this->endpointer = nullptr;

}

std::size_t mem_pool::getSizeofDB()
{
	return memPoolInitial / 1000000;
}

std::size_t mem_pool::getnumberofBlocks()
{
	return numBlockUsed;
}
bool mem_pool::allocateblock() {
	if (memPoolLeft >= blkSize) {
		currpointer = (char*)startpointer + numBlockUsed * blkSize;
		endpointer = (char*)startpointer + (numBlockUsed + 1) * blkSize;
		memPoolLeft = memPoolLeft - blkSize;
		cout << "memPoolLeft: " << memPoolLeft << "\n";
		numBlockUsed++;
		cout << "Current pointer: " << currpointer << "\n";
		return true;
	}
	else {
		cout << "mem is currently full. cannot allocate memory";
		return false;
	}

}

/*bool MemPool::deallocateblock() {


}*/

void* mem_pool::addRecord(void* structaddress, std::size_t recordsize) {
	if (numBlockUsed < numBlockInitial && recordsize + (char*)currpointer <= endpointer) {
		memcpy(currpointer, structaddress, recordsize);
		currpointer = (char*)currpointer + recordsize;
		//return block address
		cout << "Endpointer is " << endpointer << "\nAnd currptr is " << currpointer << "\n";
		return ((char*)endpointer-100);
	}
	else if (recordsize + (char*)currpointer > endpointer && numBlockInitial - numBlockUsed > 0) {
		if (allocateblock()) {
			return (addRecord(structaddress, recordsize));
		};
	}
	else {
		// To handle
		cout << "Insufficient data allocated";
		return ((char*)endpointer - 100);
	}
}

mem_pool::~mem_pool()
{
}

/*bool Mempool::delRecord(Record record) {


}*/
