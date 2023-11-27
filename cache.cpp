#include <iostream>
#include "cache.h"
#include "emu4380.h"

using namespace std;
extern unsigned char prog_mem[];

CacheLine::CacheLine(int block_size) : tag(0), dirty(false), valid(false), data(block_size) {}

DirectMappedCache::DirectMappedCache(int cache_size, int block_size) : cache_size(cache_size), block_size(block_size) {
    num_lines = cache_size / block_size;
    cache_lines.resize(num_lines, CacheLine(block_size));
}

bool DirectMappedCache::isCacheHit(unsigned int address) {
    unsigned int tag = address / block_size;
    unsigned int line_index = (address / block_size) % num_lines;

    if (cache_lines[line_index].valid && cache_lines[line_index].tag == tag) {
        return true; // Cache hit
    } else {
        return false; // Cache miss
    }
}

void DirectMappedCache::handleCacheMiss(unsigned int address) {
    unsigned int line_index = (address / block_size) % num_lines;
    cache_lines[line_index].valid = true;
    cache_lines[line_index].tag = address / block_size;

    // Fetch the required block from prog_mem
    unsigned int start_address = address - (address % block_size);
    for (int i = 0; i < block_size; ++i) {
        cache_lines[line_index].data[i] = prog_mem[start_address + i];
    }

    cache_lines[line_index].dirty = false;
}

unsigned int DirectMappedCache::getBlock(unsigned int address) {
    unsigned int line_index = (address / block_size) % num_lines;
    return cache_lines[line_index].tag;
}


void DirectMappedCache::writeBack(unsigned int address, unsigned char byte) {
    unsigned int line_index = (address / block_size) % num_lines;
    unsigned int offset = address % block_size;

    if (cache_lines[line_index].valid && cache_lines[line_index].tag == address / block_size) {
        // Cache hit
        cache_lines[line_index].data[offset] = byte;
        cache_lines[line_index].dirty = true;
    } else {
        // Cache miss or different line, write directly to main memory
        prog_mem[address] = byte;
    }
}


void DirectMappedCache::displayCache() {
    cout << "Cache Contents:" << endl;
    for (int i = 0; i < num_lines; ++i) {
        cout << "Line " << i << ": ";
        if (cache_lines[i].valid) {
            cout << "Valid | Tag: " << cache_lines[i].tag << " | Data: ";
            for (int j = 0; j < block_size; ++j) {
                cout << static_cast<int>(cache_lines[i].data[j]) << " ";
            }
        } else {
            cout << "Invalid";
        }
        cout << endl;
    }
}
