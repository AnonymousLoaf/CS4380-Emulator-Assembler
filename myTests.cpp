#include <gtest/gtest.h>
#include "emu4380.h"

TEST(EmulatorTest, ReadByteTest) {
    // Initialize memory
    prog_mem[100] = 0xAB;

    // Read a byte from memory
    unsigned char byte = readByte(100);

    // Verify the read byte
    ASSERT_EQ(byte, 0xAB);
}

TEST(EmulatorTest, ReadWordTest) {
    // Initialize memory
    prog_mem[100] = 0xAB;
    prog_mem[101] = 0xCD;

    // Read a word from memory
    unsigned int word = readWord(100);

    // Verify the read word
    ASSERT_EQ(word, 0xCDAB);
}

TEST(EmulatorTest, WriteByteTest) {
    // Write a byte to memory
    writeByte(100, 0xAB);

    // Verify the written byte
    ASSERT_EQ(prog_mem[100], 0xAB);
}

TEST(EmulatorTest, WriteWordTest) {
    // Write a word to memory
    writeWord(100, 0xCDAB);

    // Verify the written word
    ASSERT_EQ(prog_mem[100], 0xAB);
    ASSERT_EQ(prog_mem[101], 0xCD);
}

TEST(EmulatorTest, FetchTest) {
    // Initialize memory
    prog_mem[100] = 0x01;
    prog_mem[101] = 0x02;
    prog_mem[102] = 0x03;
    prog_mem[103] = 0x04;

    // Set PC to the starting address
    reg_file[PC] = 100;

    // Fetch the instruction
    fetch();

    // Verify the fetched instruction
    ASSERT_EQ(cntrl_regs[OPERATION], 0x01);
    ASSERT_EQ(cntrl_regs[OPERAND_1], 0x02);
    ASSERT_EQ(cntrl_regs[OPERAND_2], 0x03);
}

TEST(EmulatorTest, JMPInstruction) {
    // JMP 100
    prog_mem[0] = 1;
    prog_mem[4] = 100;
    prog_mem[8] = 0;

    // Simulate main
    fetch();
    bool valid = decode();
    execute();

    // Is PC set to the correct address?
    ASSERT_EQ(reg_file[PC], 100);

    //Test INVALID INSTRUCTION
    // JMP 131072
    prog_mem[12] = 1;
    prog_mem[16] = -10;
    prog_mem[20] = 0;

    // Simulate main
    fetch();
    valid = decode();
    execute();

    // Is the instruction invalid?
    ASSERT_EQ(valid, false);
}

TEST(EmulatorTest, MOVInstruction) {
    // R0 = 42
    reg_file[0] = 42; // Set a value in R0
    // MOV R0, R1
    prog_mem[0] = 7;
    prog_mem[4] = 1;
    prog_mem[8] = 0;

    // Simulate main
    fetch();
    bool valid = decode();
    execute();

    // Does R1 contain the value from R0?
    ASSERT_EQ(reg_file[1], 42);

    //Test INVALID INSTRUCTION
    // MOV R0, R16
    prog_mem[12] = 7;
    prog_mem[16] = 26;
    prog_mem[20] = 0;

    // Simulate main
    fetch();
    valid = decode();
    execute();

    // Is the instruction invalid?
    ASSERT_EQ(valid, false);
}

TEST(EmulatorTest, STRInstruction) {
    // R0 = 42
    reg_file[0] = 42;
    // STR R0, 100
    prog_mem[0] = 9;
    prog_mem[4] = 0;
    prog_mem[8] = 100;

    // Simulate main
    fetch();
    bool valid = decode();
    execute();

    // Does the address contain the value from R0?
    ASSERT_EQ(prog_mem[100], 42);
}

TEST(EmulatorTest, LDRInstruction) {
    // R0 = 42
    reg_file[0] = 42;
    // STR R0, 100
    prog_mem[0] = 9;
    prog_mem[4] = 0;
    prog_mem[8] = 100;
    // LDR R1, 100
    prog_mem[12] = 10;
    prog_mem[16] = 1;
    prog_mem[20] = 100;

    // Simulate main
    fetch();
    bool valid = decode();
    execute();
    fetch();
    valid = decode();
    execute();

    // Does R1 contain the value from the address?
    ASSERT_EQ(reg_file[1], 42);
}

TEST(EmulatorTest, STBInstruction) {
    // R0 = 42
    reg_file[0] = 42;
    // STB R0, 100
    prog_mem[0] = 11;
    prog_mem[4] = 0;
    prog_mem[8] = 100;

    // Simulate main
    fetch();
    bool valid = decode();
    execute();

    // Does the address contain the least significant byte?
    ASSERT_EQ(prog_mem[100], 42);
}

TEST(EmulatorTest, LDBInstruction) {
    // R0 = 42
    reg_file[0] = 42;
    // STB R0, 100
    prog_mem[0] = 11;
    prog_mem[4] = 0;
    prog_mem[8] = 100;
    // LDB R1, 100
    prog_mem[12] = 12;
    prog_mem[16] = 1;
    prog_mem[20] = 100;

    // Simulate main
    fetch();
    bool valid = decode();
    execute();
    fetch();
    valid = decode();
    execute();

    // Does R1 contain the least significant byte?
    ASSERT_EQ(reg_file[1], 42);
}

TEST(EmulatorTest, ADDInstruction) {
    // R0 = 42
    reg_file[0] = 42;
    // R1 = 10
    reg_file[1] = 10;
    // ADD R0, R1, R2
    prog_mem[0] = 13;
    prog_mem[4] = 0;
    prog_mem[8] = 1;

    // Simulate main
    fetch();
    bool valid = decode();
    execute();

    // Does R0 contain the sum of R0 and R1?
    ASSERT_EQ(reg_file[0], 52);
}

TEST(EmulatorTest, SUBInstruction) {
    // R0 = 42
    reg_file[0] = 42;
    // R1 = 10
    reg_file[1] = 10;
    // SUB R0, R1, R2
    prog_mem[0] = 15;
    prog_mem[4] = 0;
    prog_mem[8] = 1;

    // Simulate main
    fetch();
    bool valid = decode();
    execute();

    // Does R0 contain the difference of R0 and R1?
    ASSERT_EQ(reg_file[0], 32);
}

TEST(EmulatorTest, MULInstruction) {
    // R0 = 42
    reg_file[0] = 42;
    // R1 = 10
    reg_file[1] = 10;
    // MUL R0, R1, R2
    prog_mem[0] = 16;
    prog_mem[4] = 0;
    prog_mem[8] = 1;

    // Simulate main
    fetch();
    bool valid = decode();
    execute();

    // Does R0 contain the product of R0 and R1?
    ASSERT_EQ(reg_file[0], 420);
}

TEST(EmulatorTest, DIVInstruction) {
    // R0 = 42
    reg_file[0] = 42;
    // R1 = 10
    reg_file[1] = 10;
    // DIV R0, R1, R2
    prog_mem[0] = 17;
    prog_mem[4] = 0;
    prog_mem[8] = 1;

    // Simulate main
    fetch();
    bool valid = decode();
    execute();

    // Does R0 contain the quotient of R0 and R1?
    ASSERT_EQ(reg_file[0], 4);
}

TEST(EmulatorTest, TRPInstruction) {
    reg_file[0] = 0;
    reg_file[1] = 1;
    reg_file[2] = 2;
    reg_file[3] = 3;
    reg_file[4] = 4;
    reg_file[5] = 5;
    reg_file[6] = 6;
    reg_file[7] = 7;

    prog_mem[0] = 21;
    prog_mem[4] = 1;
    prog_mem[8] = 0;
}