#ifndef EMU4380_H
#define EMU4380_H
#define INSTRUCTION_SIZE 4

// Function prototypes
void fetch();
bool decode();
void execute();

// Externs
extern unsigned int reg_file[];
extern unsigned char prog_mem[];
extern unsigned int cntrl_regs[];
extern bool stop;

// Enums
enum RegNames {
    R0 = 0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15,
    PC = 16, SL, SB, SP, FP, HP
};

enum CntrlRegNames {
    OPERATION = 0, OPERAND_1, OPERAND_2
};

#endif
