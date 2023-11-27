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
        int num_cache_lines;
        int block_size;

        std::vector<CacheLine> cache_lines;

        DirectMappedCache(int num_cache_lines, int block_size);
        bool isCacheHit(unsigned int address);
        void handleCacheMiss(unsigned int address);
        unsigned char getByte(unsigned int address);
        unsigned int getWord(unsigned int address);
        void writeBack(unsigned int address, unsigned char byte);
        void displayCache();
    };

#endif
