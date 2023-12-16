#ifndef EMU4380_H
#define EMU4380_H
#define INSTRUCTION_SIZE 4

// Function prototypes
void fetch();
bool decode();
void execute();

// Externs
extern unsigned int reg_file[];
extern unsigned int cntrl_regs[];
extern unsigned int mem_cycle_cntr;
extern unsigned char prog_mem[];
<<<<<<< HEAD
=======
extern const int memory_size;
>>>>>>> project-4
extern unsigned int cache;

extern bool stop_execution;

// Enums
enum RegNames {
    R0 = 0, R1, R2, R3, R4, R5, R6, R7, R8, R9, R10, R11, R12, R13, R14, R15,
    PC = 16, SL, SB, SP, FP, HP
};

enum CntrlRegNames {
    OPERATION = 0, OPERAND_1, OPERAND_2
};

unsigned char readByte(unsigned int address);
unsigned int readWord(unsigned int address);
void writeByte(unsigned int address, unsigned char byte);
void writeWord(unsigned int address, unsigned int word);

#endif
