#include <iostream>
#include "cache.h"
#include "emu4380.h"

using namespace std;
extern unsigned char prog_mem[];
extern unsigned int mem_cycle_cntr;

CacheLine::CacheLine(int block_size) : tag(0), dirty(false), valid(false), data(block_size) {}

DirectMappedCache::DirectMappedCache(int cache_size, int block_size) : num_cache_lines(cache_size), block_size(block_size) {
    cache_lines.resize(num_cache_lines, CacheLine(block_size));
}

bool DirectMappedCache::isCacheHit(unsigned int address) {
    unsigned int block_offset = address % block_size;  // Calculate the offset within the block
    unsigned int tag = address / block_size;  // Calculate the tag based on the address and block size
    unsigned int line_index = (address / block_size) % num_cache_lines;  // Calculate the line index

    if (cache_lines[line_index].valid && cache_lines[line_index].tag == tag) {
        if (block_offset < cache_lines[line_index].data.size()) {
            return true;  // Cache hit
        }
    }
    return false;  // Cache miss
}


void DirectMappedCache::handleCacheMiss(unsigned int address) {
    unsigned int line_index = (address / block_size) % num_cache_lines;

    if (cache_lines[line_index].valid && cache_lines[line_index].dirty) {
        // Evict the current line if it's valid and dirty and Write back the data to main memory
        unsigned int evicted_address = cache_lines[line_index].tag * block_size;
        for (int i = 0; i < block_size; ++i) {
            prog_mem[evicted_address + i] = cache_lines[line_index].data[i];
        }
    }

    // Update the cache line with the new block
    cache_lines[line_index].valid = true;
    cache_lines[line_index].tag = address / block_size;

    // Fetch the required block from prog_mem
    unsigned int start_address = address - (address % block_size);
    for (int i = 0; i < block_size; ++i) {
        cache_lines[line_index].data[i] = prog_mem[start_address + i];
    }

    cache_lines[line_index].dirty = false;
}


unsigned char DirectMappedCache::getByte(unsigned int address) {
    unsigned int line_index = (address / block_size) % num_cache_lines;
    unsigned int offset = address % block_size;

    if (cache_lines[line_index].valid && cache_lines[line_index].tag == address / block_size) {
        // Cache hit
        if (offset < cache_lines[line_index].data.size()) {
            return cache_lines[line_index].data[offset];
        }
    }

    // Cache miss or different line, fetch from main memory
    mem_cycle_cntr += 6;
    handleCacheMiss(address);
    if (offset < cache_lines[line_index].data.size()) {
        return cache_lines[line_index].data[offset];
    } else {
        exit(1);
    }
}

unsigned int DirectMappedCache::getWord(unsigned int address) {
    unsigned int line_index = (address / block_size) % num_cache_lines;
    unsigned int offset = address % block_size;

    // Make sure the full value is in the cache line
    if (cache_lines[line_index].valid && cache_lines[line_index].tag == address / block_size) {
        // Cache hit
        if (offset + 3 < cache_lines[line_index].data.size()) {
            unsigned int word = 0;
            for (int i = 0; i < 4; ++i) {
                word += cache_lines[line_index].data[offset + i] << (8 * i);
            }
            return word;
        }
        // If the instruction doesn't fit in the current block, fetch the next block
        mem_cycle_cntr += 12;
        handleCacheMiss(address + 4);
        line_index = (address / block_size) % num_cache_lines; // Recalculate line_index
        offset = address % block_size; // Recalculate offset
    }

    // Cache miss or different line, add to cache
    mem_cycle_cntr += 12;
    handleCacheMiss(address);

    // Now, the full word should be in the cache
    unsigned int word = 0;
    for (int i = 0; i < 4; ++i) {
        word += cache_lines[line_index].data[offset + i] << (8 * i);
    }
    return word;
}



void DirectMappedCache::writeBack(unsigned int address, unsigned char byte) {
    unsigned int line_index = (address / block_size) % num_cache_lines;
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
    for (int i = 0; i < num_cache_lines; ++i) {
        cout << "Line " << i << ": ";
        if (cache_lines[i].valid) {
            cout << "Valid | Tag: " << cache_lines[i].tag << " | Data: ";
            for (int j = 0; j < block_size; ++j) {
                cout << static_cast<int>(cache_lines[i].data[j]) << " ";
            }
        } else {
            cout << "Un-Populated";
        }
        cout << endl;
    }
}
