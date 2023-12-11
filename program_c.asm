user_input .INT    0
result     .INT    0
prompt     .STR  "Please enter the Fibonacci term you would like computed: "
message_1  .STR    "Term "
message_2  .STR    " of the Fibonacci sequence is "
        JMP    MAIN

FIB     CMPI    R3, #1
        BGT     R3, ELSE
        MOV     R0, R3
        RET

ELSE   ADDI    R10, #1
        SUB     R3, R10
        STR     R3, user_input
        CALL    FIB
        MOV     R4, R0
        ADDI    R9, #2
        SUB     R3, R9
        STR     R3, user_input
        CALL    FIB
        ADD     R0, R4
        RET

MAIN    LDA     R3, prompt
        TRP     #5
        TRP     #2
        STR     R3, user_input
        TRP #98
        CALL    FIB
        STR     R0, result
        LDA     R3, message_1
        TRP     #5
        LDR     R3, user_input
        TRP     #1
        LDA     R3, message_2
        TRP     #5
        LDR     R3, result
        TRP     #1
        TRP     #0
