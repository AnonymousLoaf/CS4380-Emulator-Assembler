welcome_message  .STR    "Welcome to the Prime Number Generator.\n"
instruction_msg  .STR    "This program searches for and displays the first 20 prime numbers greater than or equal to a user-provided lower bound.\n"
lower_bound_msg  .STR    "Please enter a lower bound: "
result_heading   .STR   "The first 20 prime numbers greater than or equal to "
user_input      .INT  0
prime_check     .INT  0
prime_count     .INT  0
         JMP  MAIN
MAIN    LDA R3, welcome_message
    TRP #5
    LDA R3, instruction_msg
    TRP #5
    LDA R3, lower_bound_msg
    TRP #5
    TRP #2
    STR R3, user_input
    STR R3, prime_check
    PSHR R3
    LDA R3, result_heading
    TRP #5
    LDR R3, user_input
    JMP CHECK_PRIMES
CHECK_PRIMES CALL is_prime
    CMPI R3, #1
    BRZ R3, NOT_PRIME
    ADDI R3, #1
    STR R3, prime_count
    CMPI R3, #20
    BRZ R3, PRINT_RESULT
    JMP CHECK_PRIMES
is_prime PSHR FP
    MOV FP, SP
    MOV R0, FP
    ADDI R0, #12
    LDR R3, prime_check
    MOVI R1, #2
    MOVI R2, #0
    DIV R2, R3, R1
    CMPI R2, #0
    BRZ R2, NOT_PRIME
    ADDI R1, #1
    CMP R1, R3
    BGT R1, PRIME
    JMP is_prime
NOT_PRIME MOVI R2, #0
    STR R2, prime_check
    JMP CHECK_PRIMES
PRIME STR R3, prime_check
    RET
PRINT_RESULT LDA R3, result_heading
    TRP #5
    LDR R3, user_input
    TRP #1
    LDA R3, prime_count
    TRP #1
    LDA R3, result_heading
    TRP #5
    LDR R3, user_input
    TRP #1
    MOVI R4, #20
PRINT_LOOP CALL is_prime
    CMPI R3, #1
    BRZ R3, NOT_PRIME_PRINT
    LDR R3, user_input
    TRP #1
    TRP #0
NOT_PRIME_PRINT ADDI R4, #-1
    BRZ R4, END_PRINT
    JMP PRINT_LOOP
END_PRINT TRP #0