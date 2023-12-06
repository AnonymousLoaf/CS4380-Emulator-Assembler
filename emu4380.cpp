#include <iostream>
#include "emu4380.h"
#include "cache.h"

using namespace std;

const int memory_size = 131072;
unsigned char prog_mem[memory_size];
unsigned int cntrl_regs[3];
unsigned int reg_file[22];
bool stop_execution = false;
unsigned int mem_cycle_cntr = 0;
extern unsigned int cache;
extern DirectMappedCache myCache;

unsigned char readByte(unsigned int address) {
    if (cache == 0) {
        mem_cycle_cntr += 8;
        return *reinterpret_cast<unsigned char*>(&prog_mem[address]);
    } else {
        int hit = myCache.isCacheHit(address);
        mem_cycle_cntr += 1;
        if (hit) {
            return myCache.getByte(address);
        } else {
            mem_cycle_cntr += 8;  // Cache miss penalty
            myCache.handleCacheMiss(address);
            return readByte(address);
        }
    }
}

unsigned int readWord(unsigned int address) {
    if (cache == 0) {
        mem_cycle_cntr += 8;
        return *reinterpret_cast<unsigned int*>(&prog_mem[address]);
    } else {
        int hit = myCache.isCacheHit(address);
        if (hit) {
            //cout << "Cache hit" << endl;
            mem_cycle_cntr += 1;  // Cache hit
            return myCache.getWord(address);
        } else {
            //cout << "Cache miss" << endl;
            mem_cycle_cntr += 8;  // Cache miss
            myCache.handleCacheMiss(address);
            //myCache.displayCache();
            return readWord(address);
        }
    }
}

void writeByte(unsigned int address, unsigned char byte) {
    if (cache == 0) {
        mem_cycle_cntr += 8;
        *reinterpret_cast<unsigned char*>(&prog_mem[address]) = byte;
    } else {
        mem_cycle_cntr += 1;  // Cache hit
        myCache.writeBack(address, byte);
    }
}

void writeWord(unsigned int address, unsigned int word) {
    if (cache == 0) {
        mem_cycle_cntr += 8;
        *reinterpret_cast<unsigned int*>(&prog_mem[address]) = word;
    } else {
        mem_cycle_cntr += 1;  // Cache hit
        myCache.writeBack(address, word);
    }
}

void fetch() {
    // Read the instruction
    unsigned int operation = readWord(reg_file[PC]);
    //cout << "Operation: " << operation << endl;
    reg_file[PC] += INSTRUCTION_SIZE;

    unsigned int operand_1 = readWord(reg_file[PC]);
    //cout << "Operand 1: " << operand_1 << endl;
    reg_file[PC] += INSTRUCTION_SIZE;

    unsigned int operand_2 = readWord(reg_file[PC]);
    //cout << "Operand 2: " << operand_2 << endl;
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

    //cout << "Decode Operation: " << operation << endl;
    //cout << "Decode Operand 1: " << operand_1 << endl;
    //cout << "Decode Operand 2: " << operand_2 << endl;

    bool valid = true;

    // Specific error handling
    switch (operation) {
    case 1:  // JMP
        if (operand_1 >= memory_size || operand_1 < 0) {
            valid = false;
        }
        break;
    case 2: // JMR
        if (operand_1 < 0 || operand_1 > 15) {
            valid = false;
        }
        break;
    case 3: // BNZ
    case 4: // BGT
    case 5: // BLT
    case 6: // BRZ
        if ((operand_1 < 0 || operand_1 > 15) || (operand_2 >= memory_size || operand_2 < 0)) {
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
    case 8: // LDA
        if (operand_1 < 0 || operand_1 > 15 || (operand_2 >= memory_size || operand_2 < 0)) {
            valid = false;
        }
        break;
    case 9: // STR
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
    case 14: // ADDI
    case 15: // SUB
    case 16: // MUL
    case 17: // DIV
        if (operand_1 < 0 || operand_1 > 15 || (operand_2 < 0 || operand_2 > 15)) {
        cerr << "INVALID INSTRUCTION AT: " << reg_file[PC] << endl;
        stop_execution = true;
        valid = false;
    }
        break;
    case 18: // AND
    case 19: // OR
        if (operand_1 < 0 || operand_1 > 15 || (operand_2 < 0 || operand_2 > 15)) {
            cerr << "INVALID INSTRUCTION AT: " << reg_file[PC] << endl;
            stop_execution = true;
            valid = false;
        }
        break;
    case 20: //CMP
        if (operand_1 < 0 || operand_1 > 15 || (operand_2 < 0 || operand_2 > 15)) {
            valid = false;
        }
        break;
    case 21: // TRP
        if (!(operand_1 == 0 || operand_1 == 1 || operand_1 == 2 || operand_1 == 3 || operand_1 == 4 || operand_1 == 5 || operand_1 == 6 || operand_1 == 98)) {
            valid = false;
        }
        break;
    case 22: // ISTR
    case 23: // ILDR
    case 24: // ISTB
    case 25: // ILDB
        if (operand_1 < 0 || operand_1 > 15 || (operand_2 >= memory_size || operand_2 < 0)) {
            valid = false;
        }
        break;
    case 31: // MOVI
        if (operand_1 < 0 || operand_1 > 15) {
            valid = false;
        }
        break;
    case 32: // CMPI
        if (operand_1 < 0 || operand_1 > 15) {
            valid = false;
        }
        break;
    case 33: // MULI
    case 34: // DIVI
        if (operand_1 < 0 || operand_1 > 15) {
            valid = false;
        }
        break;
    case 35: // ALCI
        if (operand_1 < 0 || operand_1 > 15) {
            valid = false;
        }
        break;
    case 36: //ALLC
        if (operand_1 < 0 || operand_1 > 15) {
            valid = false;
        }
        break;
    case 37: // IALLC
        if (operand_1 < 0 || operand_1 > 15 || (operand_2 < 0 || operand_2 > 15)) {
            valid = false;
        }
        break;
    case 38: // SDIV
        if (operand_1 < 0 || operand_1 > 15 || (operand_2 < 0 || operand_2 > 15)) {
            valid = false;
        }
        break;
    case 40: // PSHR
    case 41: // PSHB
    case 42: // POPR
    case 43: // POPB
        if (operand_1 < 0 || operand_1 > 15) {
            valid = false;
        }
        break;
    case 44: // CALL
        if (operand_1 < 0 || operand_1 > 15 || (operand_2 >= memory_size || operand_2 < 0)) {
            valid = false;
        }
        break;
    case 45: // RET
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
    //cout << "Operation: " << operation << endl;
    //cout << "Operand 1: " << operand_1 << endl;
    //cout << "Operand 2: " << operand_2 << endl;

    // Switch statement to handle all Instructions
    switch (operation) {
    case 1: // JMP
        // Jump to address - move the PC to operand_1
        reg_file[PC] = operand_1;
        break;
    case 2: // JMR
        // Update PC to value in operand_1
        reg_file[PC] = reg_file[operand_1];
        break;
    case 3: // BNZ
        // Update PC to Address if RS != 0
        if (reg_file[operand_1] != 0) {
            reg_file[PC] = operand_2;
        }
        break;
    case 4: // BGT
        // Update PC to Address if RS > 0
        if (reg_file[operand_1] > 0) {
            reg_file[PC] = operand_2;
        }
        else {
            reg_file[PC] += INSTRUCTION_SIZE;
        }
        break;
    case 5: // BLT
        // Update PC to Address if RS < 0
        if (reg_file[operand_1] < 0) {
            reg_file[PC] = operand_2;
        }
        else {
            reg_file[PC] += INSTRUCTION_SIZE;
        }
        break;
    case 6: // BRZ
        // Update PC to Address if RS == 0
        if (reg_file[operand_1] == 0) {
            reg_file[PC] = operand_2;
        }
        else {
            reg_file[PC] += INSTRUCTION_SIZE;
        }
        break;
    case 7: // MOV
        // Move contents of operand_2 to operand_1
        reg_file[operand_1] = reg_file[operand_2];
        break;
    case 8: // LDA
        // Load address operand_2 to operand_1 using readWord
        reg_file[operand_1] = operand_2;
        break;
    case 9: // STR
        // Store integer in RS at Address using writeWord
        writeWord(operand_2, reg_file[operand_1]);
        break;
    case 10: // LDR
        // Load integer at address operand_2 to operand_1 using readWord
        reg_file[operand_1] = readWord(operand_2);
        break;
    case 12: // LDB
        // Load byte at address operand_2 to operand_1 using readByte
        reg_file[operand_1] = readByte(operand_2);
        break;
    case 13: // ADD
        // Add operand_2 to operand_1 and store in operand_1
        reg_file[operand_1] += reg_file[operand_2];
        break;
    case 14: // ADDI
        // Add immediate value in operand_2 to operand_1 and store in operand_1
        reg_file[operand_1] += operand_2;   
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
    case 18: // AND
        // NOT bitwise AND operand_1 and operand_2 and store in operand_1
        reg_file[operand_1] = ~(reg_file[operand_1] & reg_file[operand_2]);
        break;
    case 19: // OR
        // NOT bitwise OR operand_1 and operand_2 and store in operand_1
        reg_file[operand_1] = ~(reg_file[operand_1] | reg_file[operand_2]);
        break;
    case 20: // CMP
        // Compare operand_1 and operand_2 and set flags
        if (reg_file[operand_1] == reg_file[operand_2]) {
            reg_file[operand_1] = 0;
        }
        else if (reg_file[operand_1] > reg_file[operand_2]) {
            reg_file[operand_1] = 1;
        }
        else {
            reg_file[operand_1] = -1;
        }
        break;
    case 21:
        switch (operand_1) {
        case 0:
            // Executes the STOP/Exit routine
            cout << endl << "Execution completed. Total memory cycles: " << mem_cycle_cntr << "\n" << endl;
            stop_execution = true;
            break;
        case 1:
            // Write int in R3 to console
            cout << static_cast<int>(reg_file[3]);
            break;
        case 2:
            // Read an integer into R3 from stdin
            cin >> reg_file[3];
            break;
        case 3:
            // Write char in R3 to console
            cout << static_cast<char>(reg_file[3]);
            break;
        case 4:
            // Read a charater into R3 from stdin
            char input;
            cin.ignore();
            cin.get(input);
            reg_file[3] = static_cast<unsigned int>(input);
            break;
        case 5:
            // Write the pascal style string whose starting address is in R3 to stdout
            for (int i = reg_file[3]; readByte(i) != 0; ++i) {
                cout << readByte(i);
            }
            break;
        case 6:
            // Read a string from stdin and store it in memory as a Pascal-style string whose starting address is in R3.
            char input2;
            cin.ignore();
            cin.get(input2);
            for (int i = reg_file[3]; input2 != '\n'; ++i) {
                writeByte(i, static_cast<unsigned char>(input2));
                cin.get(input2);
            }
            writeByte(reg_file[3] + 1, 0);
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
    case 22: // ISTR
        // Store integer in RS at Address using writeWord
        writeWord(operand_2, reg_file[operand_1]);
        break;
    case 23: // ILDR
        // Load integer at address operand_2 to operand_1 using readWord
        reg_file[operand_1] = readWord(operand_2);
        break;
    case 24: // ISTB
        // Store byte in operand_1 and address operand_2 using writeByte
        writeByte(operand_2, reg_file[operand_1]);
        break;
    case 25: // ILDB
        // Load byte at address operand_2 to operand_1 using readByte
        reg_file[operand_1] = readByte(operand_2);
        break;
    case 31: //MOVI
        // Move operand_2 to operand_1
        reg_file[operand_1] = operand_2;
        break;
    case 32: // CMPI
        // Compare operand_1 and operand_2 and set flags
        if (reg_file[operand_1] == operand_2) {
            reg_file[operand_1] = 0;
        }
        else if (reg_file[operand_1] > operand_2) {
            reg_file[operand_1] = 1;
        }
        else {
            reg_file[operand_1] = -1;
        }
        break;
    case 33: // MULI
        // Multiply operand_1 by operand_2 and store in operand_1
        reg_file[operand_1] *= operand_2;
        break;
    case 34: // DIVI
        // Divide operand_1 by operand_2 and store in operand_1
        if (operand_2 != 0) {
            reg_file[operand_1] /= operand_2;
        }
        else {
            cerr << "Cannot divide by 0!" << endl;
        }
        break;
    case 35: // ALCI
        // Allocate operand_2 bytes of memory and store address in operand_1
        reg_file[operand_1] = reg_file[HP];
        reg_file[HP] += operand_2;
        break;
    case 36: // ALLC
        // Allocate operand_2 bytes of memory and store address in operand_1
        reg_file[operand_1] = reg_file[HP];
        reg_file[HP] += reg_file[operand_2];
        break;
    case 37: // IALLC
        // Allocate operand_2 bytes of memory and store address in operand_1
        reg_file[operand_1] = reg_file[HP];
        reg_file[HP] += operand_2;
        break;
    case 38: // SDIV
        // Store result of SIGNED division of operand_1 by operand_2 in operand_1
        if (reg_file[operand_2] != 0) {
            reg_file[operand_1] = static_cast<int>(reg_file[operand_1]) / static_cast<int>(reg_file[operand_2]);
        }
        else {
            cerr << "Cannot divide by 0!" << endl;
        }
        break;
    case 40: // PSHR
        // Push contents of operand_1 onto stack and set SP to SP - 4
        reg_file[SP] -= 4;
        writeWord(reg_file[SP], reg_file[operand_1]);
        break;
    case 41: // PSHB
        // Push contents of operand_1 onto stack and set SP to SP - 1
        reg_file[SP] -= 1;
        writeByte(reg_file[SP], reg_file[operand_1]);
        break;
    case 42: // POPR
        // Pop contents of stack into operand_1 and set SP to SP + 4
        reg_file[operand_1] = readWord(reg_file[SP]);
        reg_file[SP] += 4;
        break;
    case 43: // POPB
        // Pop contents of stack into operand_1 and set SP to SP + 1
        reg_file[operand_1] = readByte(reg_file[SP]);
        reg_file[SP] += 1;
        break;
    case 44: // CALL
        // Push PC onto stack and set PC
        writeWord(reg_file[SP], reg_file[PC]);
        // Set PC to operand_2
        reg_file[PC] = operand_1;
        break;
    case 45: // RET
        // Pop PC from stack
        reg_file[PC] = readWord(reg_file[SP]);
        break;
    default:
        cerr << "INVALID INSTRUCTION AT: " << reg_file[PC] << endl;
        stop_execution = true;
        break;
    }
}
