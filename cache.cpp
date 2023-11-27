#include <iostream>
#include <vector>
using namespace std;

class CacheLine {
public:
    bool valid;
    bool dirty;
    unsigned int tag;
    vector<unsigned int> data;

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
    // Implement logic to check if there's a cache hit
    // Update line_index and offset accordingly
    // Return true if there is a hit, false otherwise
    }

    void handleCacheMiss(unsigned int address) {
    // Implement logic to handle a cache miss
    // Load data into the cache
    }

    unsigned char readByte(unsigned int address){
        return;
    }
    unsigned int readWord(unsigned int address){
        return;
    }
    void writeByte(unsigned int address, unsigned char byte){
        return;
    }
    void writeWord(unsigned int address, unsigned int word){
        return;
    }

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
