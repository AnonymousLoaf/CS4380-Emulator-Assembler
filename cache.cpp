#include <iostream>
#include <vector>
using namespace std;

class CacheLine {
public:
    bool valid;
    bool dirty;
    unsigned int tag;
    vector<unsigned char> data;

    CacheLine(int block_size) : valid(false), dirty(false), tag(0), data(block_size) {}
};

class Cache {
private:
    int cache_size;
    int block_size;
    int num_lines;
    vector<CacheLine> cache_lines;

public:
    Cache(int cache_size, int block_size) : cache_size(cache_size), block_size(block_size) {
        num_lines = cache_size / block_size;
        cache_lines.resize(num_lines, CacheLine(block_size));
    }

    // Function to check if there's a cache hit
    bool isCacheHit(unsigned int address, unsigned int& line_index, unsigned int& offset) {
        unsigned int block_offset = address % block_size;
        unsigned int block_address = address / block_size;
        line_index = block_address % num_lines;
        offset = block_offset;

        return cache_lines[line_index].valid && cache_lines[line_index].tag == block_address;
    }

    // Function to handle a cache miss
    void handleCacheMiss(unsigned int address) {
        unsigned int line_index, offset;
        if (isCacheHit(address, line_index, offset)) {
            // Cache hit, update cache line
            // ...
        } else {
            // Cache miss, load data into cache
            loadCacheLine(address, line_index);

            // Update cache line
            cache_lines[line_index].valid = true;
            cache_lines[line_index].tag = address / block_size;
        }
    }

    // Function to load data into cache line on cache miss
    void loadCacheLine(unsigned int address, unsigned int line_index) {
        // Load data into cache line from memory or wherever you're simulating
        // For simplicity, let's assume a direct mapping, so we load the entire block.
        // You might want to load data from your memory simulator or another source.

        unsigned int block_address = address / block_size;
        for (int i = 0; i < block_size; ++i) {
            // Load data into the cache line
            cache_lines[line_index].data[i] = /* Load data from memory or another source */;
        }
    }

    // Function to read a byte from cache
    unsigned char readByte(unsigned int address) {
        unsigned int line_index, offset;
        if (isCacheHit(address, line_index, offset)) {
            // Cache hit, update cache hit statistics, and increment mem_cycle_cntr by 1
            // ...
        } else {
            // Cache miss, handle cache miss
            handleCacheMiss(address);

            // Update mem_cycle_cntr based on the specified rules
            // ...
        }

        // Return the byte from the cache
        return cache_lines[line_index].data[offset];
    }

    // Other cache functions...

    // Function to display cache contents for debugging
    void displayCache() {
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
};

// Example usage in your main program
int main() {
    Cache myCache(64, 16);  // Cache size: 64 bytes, Block size: 16 bytes

    // Use myCache in your emulator...
    myCache.displayCache();  // Display cache contents for debugging

    return 0;
}
