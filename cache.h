#ifndef CACHE_H
#define CACHE_H

#include <vector>

using namespace std;

class CacheLine {
public:
    bool valid;
    bool dirty;
    unsigned int tag;
    vector<unsigned int> data;

    CacheLine(int block_size);
};

class DirectMappedCache {
private:
    int cache_size;
    int block_size;
    int num_lines;
    vector<CacheLine> cache_lines;

public:
    DirectMappedCache(int cache_size, int block_size);

    bool isCacheHit(unsigned int address, unsigned int& line_index, unsigned int& offset);
    void handleCacheMiss(unsigned int address);
    unsigned char readByte(unsigned int address);
    unsigned int readWord(unsigned int address);
    void writeByte(unsigned int address, unsigned char byte);
    void writeWord(unsigned int address, unsigned int word);
};

#endif
