#ifndef EMU4380_H
#define EMU4380_H

// Function prototypes
void fetch();
bool decode();
void execute();

// Extern for reg_file and cntrl_regs
extern unsigned int reg_file[22];
extern unsigned int cntrl_regs[3];

// Extern for prog_mem
const int memory_size = 128 * 1024;
extern unsigned char prog_mem[memory_size];

// Enum for registers
enum RegisterNames {
    R0 = 0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15,
    PC = 16, SL, SB, SP, FP, HP
};

// Enum for control register names
enum CntrlRegNames {
    OPERATION = 0, OPERAND_1, OPERAND_2
};

#endif
