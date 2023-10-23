#include <gtest/gtest.h>
#include "emu4380.h"

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

// Define a test case to test the execute() function
TEST_F(Emu4380Test, ExecuteTest) {
    // Set up the control registers and register file
    cntrl_regs[OPERATION] = 13; // ADD
    cntrl_regs[OPERAND_1] = 1;
    cntrl_regs[OPERAND_2] = 2;
    reg_file[1] = 5;
    reg_file[2] = 10;

    // Call the execute() function
    execute();

    // Check that the result is correct
    ASSERT_EQ(reg_file[1], 15);
}

// Add more test cases as needed

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}