#include <iostream>
#include "cache.h"

using namespace std;

CacheLine::CacheLine(int block_size) : tag(0), dirty(false), valid(false), data(block_size) {}

DirectMappedCache::DirectMappedCache(int cache_size, int block_size) : cache_size(cache_size), block_size(block_size) {
    num_lines = cache_size / block_size;
    cache_lines.resize(num_lines, CacheLine(block_size));
}

bool DirectMappedCache::isCacheHit(unsigned int address, unsigned int& line_index, unsigned int& offset) {
    line_index = (address / block_size) % num_lines;
    offset = address % block_size;
    return cache_lines[line_index].valid && (cache_lines[line_index].tag == (address / block_size));
}

void DirectMappedCache::handleCacheMiss(unsigned int address) {
    unsigned int line_index = (address / block_size) % num_lines;
    cache_lines[line_index].valid = true;
    cache_lines[line_index].tag = address / block_size;
}

unsigned char DirectMappedCache::readByte(unsigned int address) {
    unsigned int line_index, offset;
    if (isCacheHit(address, line_index, offset)) {
        // Cache hit
        return cache_lines[line_index].data[offset];
    } else {
        // Cache miss
        handleCacheMiss(address);
        return cache_lines[line_index].data[offset];
    }
}

unsigned int DirectMappedCache::readWord(unsigned int address) {
    unsigned int line_index, offset;
    if (isCacheHit(address, line_index, offset)) {
        // Cache hit
        unsigned int word = 0;
        for (int i = 0; i < sizeof(unsigned int); ++i) {
            word |= static_cast<unsigned int>(cache_lines[line_index].data[offset + i]) << (i * 8);
        }
        return word;
    } else {
        // Cache miss
        handleCacheMiss(address);
        return readWord(address);
    }
}

void DirectMappedCache::writeByte(unsigned int address, unsigned char byte) {
    unsigned int line_index, offset;
    if (isCacheHit(address, line_index, offset)) {
        // Cache hit, update data in the cache
        cache_lines[line_index].data[offset] = byte;
        cache_lines[line_index].dirty = true;
    } else {
        // Cache miss, handle it accordingly
        handleCacheMiss(address);
        // Update the data in the cache
        cache_lines[line_index].data[offset] = byte;
        cache_lines[line_index].dirty = true;
    }
}

void DirectMappedCache::writeWord(unsigned int address, unsigned int word) {
    unsigned int line_index, offset;
    if (isCacheHit(address, line_index, offset)) {
        // Cache hit, update data in the cache
        for (int i = 0; i < sizeof(unsigned int); ++i) {
            cache_lines[line_index].data[offset + i] = static_cast<unsigned char>((word >> (i * 8)) & 0xFF);
        }
        cache_lines[line_index].dirty = true;
    } else {
        // Cache miss, handle it accordingly
        handleCacheMiss(address);
        // Update the data in the cache
        writeWord(address, word);
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
