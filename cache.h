#ifndef CACHE_H
#define CACHE_H

#include <vector>

class CacheLine {
public:
    unsigned int tag;
    bool dirty;
    bool valid;
    std::vector<unsigned char> data;

    CacheLine(int block_size);
};

class DirectMappedCache {
public:
    int cache_size;
    int block_size;
    int num_lines;
    std::vector<CacheLine> cache_lines;

    DirectMappedCache(int cache_size, int block_size);
    bool isCacheHit(unsigned int address, unsigned int& line_index, unsigned int& offset);
    void handleCacheMiss(unsigned int address);
    unsigned char readByte(unsigned int address);
    unsigned int readWord(unsigned int address);
    void writeByte(unsigned int address, unsigned char byte);
    void writeWord(unsigned int address, unsigned int word);
    void displayCache();
};

#endif
