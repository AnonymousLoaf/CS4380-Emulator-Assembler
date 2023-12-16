prompt     .STR  "Please enter the Fibonacci term you would like computed: "
fib_3      .INT  2
fib_prev   .INT  0
fib_curr   .INT  1
count      .INT  0
user_input .INT  0
result     .INT  0
message_1  .STR  "Term "
message_2  .STR  " of the Fibonacci sequence is "
NL         .BYT  '\n'
           JMP  MAIN

MAIN    LDR R4, fib_prev
        LDR R5, fib_curr
        LDR R6, fib_3
        LDR R8, user_input ;This is the user input
        LDR R9, result ;This is the result
        LDR R10, count ;This is the count
        LDA R3, prompt
        TRP #5
        TRP #2
        STR R3, user_input
        SUB R3, R5
        ADDI R10, #1
        BRZ R3, PRINT
        LDR R3, user_input
        ADDI R9, #1
        SUB R3, R6
        ADDI R10, #1
        MOV R3, R10
        BRZ R3, PRINT
        ADD R9, R9
        BNZ R3, LOOP


LOOP    LDR R7, user_input
        MOV R8, R5
        ADD R5, R4
        MOV R4, R8
        MOV R9, R5
        ADDI R10, #1
        SUB R7, R10
        BGT R7, LOOP
        BRZ R7, PRINT

PRINT   MOV R3, R9
        LDA R3, message_1
        TRP #5
        LDR R3, user_input
        TRP #1
        LDA R3, message_2
        TRP #5
        MOV R3, R9
        TRP #1
        TRP #0