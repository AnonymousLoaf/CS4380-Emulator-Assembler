INPT_PRMPT     .STR    "Please enter an integer: "
OUTPT_PRMPT    .STR    " cubed is: "

        JMP     MAIN
CUBE    PSHR    FP              ; Save the caller's frame pointer
        MOV     FP, SP          ; Establish this function's FP
                                ; Allocate stack space for local variables (if any) [move SP down]
                                ; Save callee saved registers (if any used) [Push register values]            
        MOV     R0, FP          ; Param1 at FP + 12... (note: this will be true no matter WHERE we are on the stack when called...)
        ADDI    R0, #12         ; ...
        ILDR    R1, R0          ; Load the value from the stack at the address in R0 into R1 (R1 = Param1)
        MOV     R2, R1          ; R2 = Param1
        MUL     R2, R1          ; R2 = Param1 * Param1
        MUL     R2, R1          ; R2 = Param1 * Param1 * Param1
        MOV     R0, FP          ; Return value at FP + 8
        ADDI    R0, #8          ; ...
        ISTR    R2, R0          ; Store return value at FP + 4 on stack...
                                ; Restore callee saved registers (if any used) [Pop register values]
                                ; Deallocate stack space for local variables (if any) [move SP up]
        POPR    FP              ; Restore caller's FP
        RET                     ; Return to caller

MAIN    LDA     R3, INPT_PRMPT
        TRP     #5              ; Prompt for the value
        TRP     #2              ; read the value
                                ; *** Now get ready and call CUBE... ***
        PSHR    R3              ; save caller saved registers (if any used), 
                                ; NOTE: This may seem redundant in a moment, but better safe than sorry... 
        PSHR    R3              ; Push/place parameters onto stack in reverse order... (simply pushing a reg here, in the future you might have to do more work - i.e. saving a struct to the stack etc.)
        ADDI    SP, #-4         ; Allocate space on the stack for the return value...
        CALL    CUBE            ; Call the function...
        
                                ; Now that we've returned we need to retrieve the return value, which should be
                                ; on the top of the stack...
        POPR    R0              ; I'm using a pop here (because it is convenient), there are other ways...
        ADDI    SP, #4          ; Now dealocate space for parameters (note: you may be able to do this with pop instructions as well)
        POPR    R3              ; Restore calle saved registers...
                                ; Now go on with our day...
        TRP     #1              ; "#... 
        LDA     R3, OUTPT_PRMPT 
        TRP     #5              ; "... cubed is:"
        MOV     R3, R0          
        TRP     #1              ; "#..."
        MOVI    R3, #10
        TRP     #3              ; "\n"
        TRP     #0              ; Done.




