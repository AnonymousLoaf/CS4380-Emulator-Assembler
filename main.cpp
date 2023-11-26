#include <iostream>
#include <fstream>
#include <string>
#include "emu4380.h"

using namespace std;
extern unsigned char prog_mem[];
extern unsigned int mem_cycle_cntr;

int main(int argc, char* argv[]) {
    ifstream binFile;

    if (argc < 2 || argc > 3) {
        cerr << "Usage: " << argv[0] << " <file_path>" << endl;
        exit(1);
    }

    else if (argc == 2) {
        //cache = no_cache;
    }

    else if (argc == 3) {
        if (string(argv[2]) == "0") {
            //cache = no_cache;
        } else if (string(argv[2]) == "1") {
            //cache = direct_mapped;
        } else {
            cerr << "Invalid cache configuration. Aborting.\n";
            exit(2);
        }
    }

    else {
        cerr << "Unknown Error. Aborting.\n";
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

    exit(0);
}
