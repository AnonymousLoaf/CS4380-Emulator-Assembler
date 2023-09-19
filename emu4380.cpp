#include <iostream>
#include "emu4380.h"

using namespace std;

const int memory_size = 128 * 1024;
unsigned char prog_mem[memory_size];
unsigned int cntrl_regs[3];
unsigned int reg_file[22];


void fetch() {
    // Read the instruction
    unsigned int operation = static_cast<unsigned int>(prog_mem[reg_file[PC]]);
    reg_file[PC] += INSTRUCTION_SIZE;
    unsigned int operand_1 = static_cast<unsigned int>(prog_mem[reg_file[PC]]);
    reg_file[PC] += INSTRUCTION_SIZE;
    unsigned int operand_2 = static_cast<unsigned int>(prog_mem[reg_file[PC]]);
    reg_file[PC] += INSTRUCTION_SIZE;

    // Update control registers
    cntrl_regs[OPERATION] = operation;
    cntrl_regs[OPERAND_1] = operand_1;
    cntrl_regs[OPERAND_2] = operand_2;

    // Debug: Print the control registers
    cout << "OPERATION: " << cntrl_regs[OPERATION] << endl;
    cout << "OPERAND_1: " << cntrl_regs[OPERAND_1] << endl;
    cout << "OPERAND_2: " << cntrl_regs[OPERAND_2] << endl;
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