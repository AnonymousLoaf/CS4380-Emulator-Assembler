#include <iostream>
#include <fstream>
#include <string>
#include "emu4380.h"

using namespace std;
extern unsigned char prog_mem[];

int main() {
    string fileName;
    ifstream binFile;

    cout << "Enter file path: ";
    cin >> fileName;

    binFile.open(fileName, ios::binary);
    if (!binFile.is_open()) {
        cerr << "ERROR! Could not OPEN the given file" << endl;
        return 1;
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
        return 1;
    }

    // Dynamically allocate memory for prog_mem
    // unsigned char* prog_mem = new unsigned char[fileSize];

    // Read the file into prog_mem
    binFile.read(reinterpret_cast<char*>(prog_mem), fileSize);
    if (!binFile.good()) {
        cerr << "ERROR! Could not READ the given file" << endl;
        binFile.close();
        delete[] prog_mem; // Release dynamically allocated memory
        return 1;
    }

    binFile.close();
    reg_file[PC] = prog_mem[0];

    while (true) {
        // Fetch instruction
        fetch();

        // Check if it's valid
        if (!decode()) {
            // Handle invalid instruction
            cerr << "INVALID INSTRUCTION AT: " << reg_file[PC] << endl;
            delete[] prog_mem; // Release dynamically allocated memory
            return 1;
        }

        // Execute instruction
        execute();

        // Check if a TRP 0
        if (cntrl_regs[OPERATION] == 21) {
            break; // Exit the execution loop
        }
    }

    // Release dynamically allocated memory
    delete[] prog_mem;

    return 0;
}
