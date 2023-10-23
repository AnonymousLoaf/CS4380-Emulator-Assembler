#include <iostream>
#include "emu4380.h"

using namespace std;

const int memory_size = 131072;
unsigned char prog_mem[memory_size];
unsigned int cntrl_regs[3];
unsigned int reg_file[22];
bool stop_execution = false;


void fetch() {
    // Read the instruction
    uint32_t* instruction_ptr = reinterpret_cast<uint32_t*>(&prog_mem[reg_file[PC]]);
    unsigned int operation = *instruction_ptr;
    reg_file[PC] += INSTRUCTION_SIZE;

    uint32_t* operand_1_ptr = reinterpret_cast<uint32_t*>(&prog_mem[reg_file[PC]]);
    unsigned int operand_1 = *operand_1_ptr;
    reg_file[PC] += INSTRUCTION_SIZE;

    uint32_t* operand_2_ptr = reinterpret_cast<uint32_t*>(&prog_mem[reg_file[PC]]);
    unsigned int operand_2 = *operand_2_ptr;
    reg_file[PC] += INSTRUCTION_SIZE;

    if (operation == 1 && (operand_1 < 0 || operand_1 >= memory_size)) {
            cerr << "Invalid JMP instruction (Out of range): " << operand_1 << endl;
            stop_execution = true;
            return;
        }
    if (operation == 1 && operand_1 == reg_file[PC]) {
            cerr << "Invalid JMP instruction (Infinite loop): " << operand_1 << endl;
            stop_execution = true;
            return;
    }
    // Other edge cases for JMP
    
    // Update control registers
    cntrl_regs[OPERATION] = operation;
    cntrl_regs[OPERAND_1] = operand_1;
    cntrl_regs[OPERAND_2] = operand_2;

    
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
        cerr << "INVALID INSTRUCTION AT: " << reg_file[PC] << endl;
        stop_execution = true;
        return false;
    }
}

void execute() {
    // Extract the operation and operands
    unsigned int operation = cntrl_regs[OPERATION];
    unsigned int operand_1 = cntrl_regs[OPERAND_1];
    unsigned int operand_2 = cntrl_regs[OPERAND_2];

    // Switch statement to handle all Instructions
    switch (operation) {
    case 1: // JMP
        // Jump to address - move the PC to operand_1
        reg_file[PC] = operand_1;
        break;
    case 7: // MOV
        // Move contents of operand_2 to operand_1
        reg_file[operand_1] = reg_file[operand_2];
        break;
    case 9: // STR
        // Store integer in RS at Address
        prog_mem[operand_2] = static_cast<unsigned int>(reg_file[operand_1]);
        break;
    case 10: // LDR
        // Load integer at address operand_2 to operand_1
        reg_file[operand_1] = *reinterpret_cast<unsigned int*>(&prog_mem[operand_2]);
        break;
    case 11: //STB
        // Store least significant byte in RS at Address
        prog_mem[operand_2] = static_cast<unsigned char>(reg_file[operand_1]);
    case 12: // LDB
        // Load byte at address operand_2 to operand_1
        reg_file[operand_1] = prog_mem[operand_2];
        break;
    case 13: // ADD
        // Add operand_2 to operand_1 and store in operand_1
        reg_file[operand_1] += reg_file[operand_2];
        break;
    case 15: // SUB
        // Subtract operand_2 from operand_1 and store in operand_1
        reg_file[operand_1] -= reg_file[operand_2];
        break;
    case 16: // MUL
        // Multiply opeand_2 by operand_1 and store in operand_1
        reg_file[operand_1] *= reg_file[operand_2];
        break;
    case 17: // DIV
        // Divide operand_1 by operand_2 and store in operand_1
        if (reg_file[operand_2] != 0) {
            reg_file[operand_1] /= reg_file[operand_2];
        }
        else {
            cerr << "Cannot divide by 0!" << endl;
        }
        break;
    case 21:
        switch (operand_1) {
        case 0:
            // Executes the STOP/Exit routine
            stop_execution = true;
            break;
        case 1:
            // Write int in R3 to console
            cout << static_cast<int>(reg_file[3]);
            break;
        case 3:
            // Write char in R3 to console
            cout << static_cast<char>(reg_file[3]);
            break;
        case 98:
            // Print all registers to console
            for (int i = 0; i < 16; ++i) {
                cout << "R" << i << "\t" << reg_file[i] << endl;
            }
            cout << "PC\t" << reg_file[PC] << endl;
            cout << "SL\t" << reg_file[SL] << endl;
            cout << "SB\t" << reg_file[SB] << endl;
            cout << "SP\t" << reg_file[SP] << endl;
            cout << "FP\t" << reg_file[FP] << endl;
            cout << "HP\t" << reg_file[HP] << endl;
            break;
        default:
            cerr << "INVALID INSTRUCTION AT: " << reg_file[PC] << endl;
            break;
        }
        break;
    default:
        cerr << "INVALID INSTRUCTION AT: " << reg_file[PC] << endl;
        stop_execution = true;
        break;
    }
}
