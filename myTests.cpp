#include <gtest/gtest.h>
#include "emu4380.h"

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