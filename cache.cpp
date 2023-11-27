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
    unsigned int tag = address / cache_size;
    unsigned int line_index = (address / block_size) % num_lines;
    unsigned int offset = address % block_size;

    if (cache_lines[line_index].valid && cache_lines[line_index].tag == tag) {
        return true; // Cache hit
    } else {
        return false; // Cache miss
    }
}

void DirectMappedCache::handleCacheMiss(unsigned int address) {
    unsigned int line_index = (address / block_size) % num_lines;
    cache_lines[line_index].valid = true;
    cache_lines[line_index].tag = address / cache_size;

    // Fetch the required block from prog_mem
    unsigned int start_address = address - (address % block_size);
    for (int i = 0; i < block_size; ++i) {
        cache_lines[line_index].data[i] = prog_mem[start_address + i];
    }

    cache_lines[line_index].dirty = false;
}

unsigned char DirectMappedCache::readByte(unsigned int address) {
    unsigned int line_index = (address / block_size) % num_lines;
    unsigned int offset = address % block_size;
    return cache_lines[line_index].data[offset];
}

unsigned int DirectMappedCache::readWord(unsigned int address) {
    unsigned int line_index = (address / block_size) % num_lines;
    unsigned int offset = address % block_size;
    unsigned int word = 0;
    for (int i = 0; i < 4; ++i) {
        word |= cache_lines[line_index].data[offset + i] << (8 * i);
    }
    return word;
}

void DirectMappedCache::writeByte(unsigned int address, unsigned char byte) {
    if (isCacheHit(address)) {
        unsigned int line_index = (address / block_size) % num_lines;
        unsigned int offset = address % block_size;
        cache_lines[line_index].data[offset] = byte;
        cache_lines[line_index].dirty = true;
    } else {
        prog_mem[address] = byte;
    }
}

void DirectMappedCache::writeWord(unsigned int address, unsigned int word) {
    if (isCacheHit(address)) {
        unsigned int line_index = (address / block_size) % num_lines;
        unsigned int offset = address % block_size;
        for (int i = 0; i < 4; ++i) {
            cache_lines[line_index].data[offset + i] = (word >> (8 * i)) & 0xFF;
        }
        cache_lines[line_index].dirty = true;
    } else {
        for (int i = 0; i < 4; ++i) {
            prog_mem[address + i] = (word >> (8 * i)) & 0xFF;
        }
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
