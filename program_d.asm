    ORIG    x3000

    ; Constants
    LOWER_BOUND_PROMPT  .STR  "Welcome to the Prime Number Generator.\nThis program searches for and displays the first 20 prime numbers greater than or equal to a user-provided lower bound.\nPlease enter a lower bound: "
    PRIME_PROMPT        .STR  "The first 20 prime numbers greater than or equal to "
    NEW_LINE            .BYT  '\n'

    ; Data storage
    LOWER_BOUND         .INT  0
    PRIME_COUNT         .CONST 20
    PRIMES              .BLKW PRIME_COUNT
    COUNT               .INT  0

    ; Function prototypes
    JMP     .FILL 1
    BNZ     .FILL 3
    ADD     .FILL 13
    ADDI    .FILL 14
    SUB     .FILL 15
    CMP     .FILL 20
    TRP     .FILL 21
    LDA     .FILL 8
    STR     .FILL 9

    ; Main program
    LDA     R0, LOWER_BOUND_PROMPT
    TRP     #5
    TRP     #2
    LDI     R0, LOWER_BOUND
    TRP     #2
    STR     R0, LOWER_BOUND

    LDI     R0, PRIME_PROMPT
    TRP     #5

    LDI     R1, LOWER_BOUND
    LDI     R2, COUNT
    LDI     R3, PRIMES
    LDI     R4, PRIME_COUNT

CHECK_PRIME
    LDR     R5, R1, #0
    JSR     R6, IS_PRIME
    BRZ     NOT_PRIME

    STR     R5, R3, #0
    ADD     R3, R3, #1

    LDI     R5, COUNT
    ADD     R5, R5, #1
    STR     R5, COUNT

NOT_PRIME
    ADD     R1, R1, #1
    ADD     R2, R2, #1
    ADD     R4, R4, #-1
    BGT     CHECK_PRIME

    ; Print the results
    LDI     R0, NEW_LINE
    TRP     #5

    LDI     R0, LOWER_BOUND
    ADD     R1, R0, #0
    LDI     R0, COUNT
    ADD     R0, R0, #-1
    ADD     R1, R1, R0
    TRP     #1

    LDI     R0, NEW_LINE
    TRP     #5

    LDI     R0, PRIMES
    LDI     R1, PRIME_COUNT

PRINT_PRIMES
    LDR     R2, R0, #0
    TRP     #1
    ADD     R0, R0, #1
    ADD     R1, R1, #-1
    BGT     PRINT_PRIMES

    TRP     #0

; Function to check if a number is prime
IS_PRIME
    ST      R7, SAVE_R7

    LDI     R7, R5
    ADDI    R7, #2
    JSR     R6, DIVIDE
    BNZ     IS_NOT_PRIME

    STR     R7, SAVE_R7
    RET

IS_NOT_PRIME
    AND     R5, R5, #0
    LDI     R7, SAVE_R7
    RET

; Function to divide two numbers
DIVIDE
    ST      R6, SAVE_R6
    ST      R7, SAVE_R7
    ADDI    R6, #-1

DIVIDE_LOOP
    ADDI    R6, #1
    ADDI    R5, #-1
    BRN     DIVIDE_DONE
    BRZ     DIVIDE_LOOP
    BRNZP   DIVIDE_LOOP

DIVIDE_DONE
    AND     R5, R5, #0
    LDI     R6, SAVE_R6
    LDI     R7, SAVE_R7
    RET

; Data storage for saved registers
SAVE_R6 .BLKW 1
SAVE_R7 .BLKW 1
