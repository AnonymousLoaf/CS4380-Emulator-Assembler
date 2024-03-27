#include <iostream>
#include <fstream>
#include <string>
#include "emu4380.h"
#include "cache.h"

using namespace std;
extern unsigned char prog_mem[];
extern const int memory_size;

int main(int argc, char* argv[]) {
    ifstream binFile;

    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <file_path>" << endl;
        exit(1);
    }

    
    string fileName = argv[1];

    binFile.open(fileName, ios::binary);
    if (!binFile.is_open()) {
        cerr << "ERROR! Could not OPEN the given file" << endl;
        exit(1);
    }

    // Get the file size
    binFile.seekg(0, ios::end);
    int fileSize = binFile.tellg();
    binFile.seekg(0, ios::beg);

    // Check if the file size exceeds the limit
    const int maxFileSize = 131072;
    if (fileSize > maxFileSize) {
        cerr << "ERROR! File size exceeds the maximum limit of " << maxFileSize << " bytes." << endl;
        binFile.close();
        exit(1);
    }

    // Read the file into prog_mem
    binFile.read(reinterpret_cast<char*>(prog_mem), fileSize);
    if (!binFile.good()) {
        cerr << "ERROR! Could not READ the given file" << endl;
        binFile.close();
        exit(1);
    }

    binFile.close();

    // Set PC to the first address indicated by the program memory
    reg_file[PC] = prog_mem[0];

    // Set SB to the highest available memory address + 1
    reg_file[SB] = memory_size;

    // Set SP to the highest available memory address + 1
    reg_file[SP] = memory_size;

    // Set SL to the address of the first byte past the last instruction read into prog_mem
    reg_file[SL] = fileSize + 1;

    while (!stop_execution) {
        // Fetch instruction
        fetch();

        // Decode instruction - check validity
        if (!decode()) {
            cerr << "INVALID INSTRUCTION AT: " << reg_file[PC] << endl;
            exit(1);
        }

        // Execute instruction
        execute();
    }
    /*cout << "PC: " << reg_file[PC] << endl;
    cout << "SB: " << reg_file[SB] << endl;
    cout << "SP: " << reg_file[SP] << endl;
    cout << "SL: " << reg_file[SL] << endl;
    cout << "Program Memory: " << endl;
    for (int i = 0; i < memory_size; ++i) {
            cout << static_cast<int>(prog_mem[i]) << " ";
        }
        cout << endl;*/

    exit(0);
}