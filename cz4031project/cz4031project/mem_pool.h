#pragma once
#include<iostream>

class mem_pool {
private:
    void* currpointer;
    std::size_t memPoolInitial;
    std::size_t memPoolLeft;
    int blkSize;
    int numBlockInitial;
    int numBlockUsed;
    void* startpointer;
    //  std::size_t memPoolUsed;
    void* endpointer;

public:
    mem_pool(std::size_t memPoolInitial, std::size_t blkSize);
    std::size_t getSizeofDB();
    std::size_t getnumberofBlocks();
    //constructor for memory pool
    bool allocateblock();//allocating blocks, returning a boolean variable
    void* addRecord(void* structaddress, std::size_t recordsize); //adding a record to a block, returns the pointer address
    ~mem_pool(); //destructor for mempool
    // Returns the size of a block in memory pool.
    std::size_t getBlkSize() const
    {
        return blkSize;
    };

};


struct Record
{
    char tconst[10];
    int numVotes;
    float averageRating;
};