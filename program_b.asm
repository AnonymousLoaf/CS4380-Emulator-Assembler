MOD_PROMPT1 .STR "Please enter the dividend: "
MOD_PROMPT2 .STR "Please enter the divisor: "
MOD_RESULT  .STR "The modulus is: "
NL          .BYT '\n'
JMP MAIN
MODULO  PSHR FP              ; Save caller's frame pointer
        MOV FP, SP           ; Establish this function's frame pointer
        MOV R0, FP           ; R0 = FP (frame pointer)
        ADDI R0, #12         ; R0 points to the first parameter on the stack
        ILDR R3, R0          ; Load dividend into R3 (Param1)
        ILDR R1, R0
        ADDI R0, #12          ; Move to the second parameter on the stack
        ILDR R2, R0          ; Load divisor into R2 (Param2)
        DIV R3, R2           ; R1 = R1 / R2 (integer division)
        MUL R3, R2           ; R1 = (R1 / R2) * R2
        SUB R1, R3           
        MOV R0, FP           ; R0 = FP (frame pointer)
        ADDI R8, #8
        SUB R0, R8           ; R0 points to the location to store the result
        ISTR R4, R0          ; Store the result on the stack
        POPR FP              ; Restore caller's frame pointer
        RET                  ; Return to caller

MAIN    LDA R3, MOD_PROMPT1  ; Load dividend prompt
        TRP #5               ; Print the prompt
        TRP #2               ; Read the dividend
        PSHR R3
        ADDI  SP, #-4
        LDA R3, MOD_PROMPT2  ; Load divisor prompt
        TRP #5               ; Print the prompt
        TRP #2               ; Read the divisor
        PSHR R3              ; Push parameters onto the stack
        ADDI SP, #-4         ; Allocate space on the stack for the return value
        CALL MODULO
        POPR R0              ; Retrieve the result from the stack
        ADDI SP, #4          ; Deallocate space for parameters
        POPR R3              ; Restore temporary register
        LDA R3, MOD_RESULT   ; Load result prompt
        TRP #5               ; Print the prompt
        MOV R3, R0           ; Move the result to a register
        TRP #1               ; Print the result
        MOVI R3, #10         ; Load newline character
        TRP #3               ; Print a newline
        TRP #0               ; Halt the program