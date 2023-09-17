#include <iostream>
#include <fstream>
#include <string>
#include "emu4380.h"

using namespace std;

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
        
	binFile.read(reinterpret_cast<char*>(prog_mem), memory_size);
	if (!binFile.good()) {
		cerr <<"ERROR! Could not READ the given file" << endl;
		return 1;
	}
	
	binFile.close();
	
	while (true) {
        // Fetch instruction
        fetch();

        // Check it it's valid
        if (!decode()) {
            // Handle invalid instruction
            cerr << "INVALID INSTRUCTION AT: " << PC << endl;
            return 1;
        }

        // Execute instruction
        execute();

        // Check if a TRP 0
        if (cntrl_regs[OPERATION] == 21 && cntrl_regs[OPERAND_1] == 0 && cntrl_regs[OPERAND_2] == 0) {
            break; // Exit the execution loop
        }
    }

	return 0;
}
