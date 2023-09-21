#include <gtest/gtest.h>
#include "emu4380.h"

// Include the header file for the code you want to test (main.cpp and emu4380.cpp)

// Define a test fixture
class Emu4380Test : public ::testing::Test {
protected:
    // Add setup code if necessary
    virtual void SetUp() {
        // Initialize any necessary variables or state before each test
    }

    // Add cleanup code if necessary
    virtual void TearDown() {
        // Clean up any resources after each test
    }
};

// Define a test case to read the binary file and execute instructions
TEST_F(Emu4380Test, RunBinaryFileTest) {
    // Load the binary file into prog_mem
    const char* binaryFile = "test_program.bin";
    std::ifstream binFile(binaryFile, std::ios::binary);

    if (!binFile.is_open()) {
        FAIL() << "Failed to open binary file: " << binaryFile;
    }

    binFile.read(reinterpret_cast<char*>(prog_mem), memory_size);

    if (!binFile.good()) {
        binFile.close();
        FAIL() << "Failed to read binary file: " << binaryFile;
    }

    binFile.close();

    // Set PC to the first address indicated by the program memory
    reg_file[PC] = prog_mem[0];

    while (!stop_execution) {
        // Fetch instruction
        fetch();

        // Decode instruction - check validity
        if (!decode()) {
            FAIL() << "INVALID INSTRUCTION AT: " << reg_file[PC];
        }

        // Execute instruction
        execute();
    }

    // Add assertions to check the final state of the registers, etc.
    // Example:
    // ASSERT_EQ(reg_file[R0], expected_R0_value);
    // ASSERT_EQ(reg_file[R1], expected_R1_value);
    // ...
}

// Add more test cases as needed

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
