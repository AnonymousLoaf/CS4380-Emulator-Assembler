#include <iostream>
#include "emu4380.h"

using namespace std;

const int memory_size = 128 * 1024;
unsigned char prog_mem[memory_size];
unsigned int cntrl_regs[3];
unsigned int reg_file[22];


void fetch() {
    // Read the instruction
    unsigned int instruction = 0;
    for (int i = 0; i < 12; ++i) {
        instruction |= (static_cast<unsigned int>(prog_mem[reg_file[PC] + i]) << ((11 - i) * 8));
    }

    // Update control registers
    cntrl_regs[OPERATION] = (instruction >> 24) & 0xFF;
    cntrl_regs[OPERAND_1] = (instruction >> 16) & 0xFF;
    cntrl_regs[OPERAND_2] = instruction & 0xFFFF;

    cout << cntrl_regs[OPERATION] << endl;
    cout << cntrl_regs[OPERAND_1] << endl; 
    cout << cntrl_regs[OPERAND_2] << endl;

    // Increment the PC
    reg_file[PC] += 12;
}

bool decode() {
    // Extract the operation code
    int operation = cntrl_regs[OPERATION];

    // Validate
    switch (operation) {
        case 1:  // JMP
        case 7:  // MOV
        case 10: // LDR
        case 12: // LDB
        case 13: // ADD
        case 15: // SUB
        case 16: // MUL
        case 17: // DIV
        case 21: // TRP
            return true;
        default:
            return false;
    }
}

void execute() {
}