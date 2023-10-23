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
    
    // Update control registers
    cntrl_regs[OPERATION] = operation;
    cntrl_regs[OPERAND_1] = operand_1;
    cntrl_regs[OPERAND_2] = operand_2;
}


bool decode() {
    int operation = cntrl_regs[OPERATION];
    int operand_1 = cntrl_regs[OPERAND_1];
    int operand_2 = cntrl_regs[OPERAND_2];

    //cout << "Operation: " << operation << endl;
    //cout << "Operand 1: " << operand_1 << endl;
    //cout << "Operand 2: " << operand_2 << endl;

    bool valid = true;

    // Specific error handling
    switch (operation) {
    case 1:  // JMP
        if (operand_1 >= memory_size || operand_1 < 0) {
            valid = false;
        }
        break;
    case 7:  // MOV
        if (operand_1 < 0 || operand_1 > 15 || (operand_2 < 0 || operand_2 > 15)) {
        cerr << "INVALID INSTRUCTION AT: " << reg_file[PC] << endl;
        stop_execution = true;
        valid = false;
    }
        break;
    case 10: // LDR
    case 11: // STB
    case 12: // LDB
        if ((operand_1 < 0 || operand_1 > 15) || (operand_2 < 0 || operand_2 > memory_size)) {
            cerr << "INVALID INSTRUCTION AT: " << reg_file[PC] << endl;
            stop_execution = true;
            valid = false;
        }
        break;
    case 13: // ADD
    case 15: // SUB
    case 16: // MUL
    case 17: // DIV
        if (operand_1 < 0 || operand_1 > 15 || (operand_2 < 0 || operand_2 > 15)) {
        cerr << "INVALID INSTRUCTION AT: " << reg_file[PC] << endl;
        stop_execution = true;
        valid = false;
    }
        break;
    case 21: // TRP
        if (!(operand_1 == 0 || operand_1 == 1 || operand_1 == 2 || operand_1 == 3 || operand_1 == 4 || operand_1 == 98)) {
            valid = false;
        }
        break;
    default:
        cerr << "INVALID INSTRUCTION AT: " << reg_file[PC] << endl;
        valid = false;
        break;
    }

    stop_execution = !valid;
    return valid;
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
        case 2:
            // Read an integer into R3 from stdin
            cin >> reg_file[3];
        case 3:
            // Write char in R3 to console
            cout << static_cast<char>(reg_file[3]);
            break;
        case 4:
            // Read a char into R3 from stdin
            cin >> reg_file[3];
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
