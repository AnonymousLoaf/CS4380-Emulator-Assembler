FIB_PRMPT   .STR    "Enter a position in the Fibonacci sequence: "
OUT_PRMPT   .STR    "The Fibonacci number at position "
            .BTS    #32
            .STR    " is: "
            .BTS    #10  ; newline character
ONE         .INT    #1
            .BYT    #0     ; Initialize a counter variable to 0
POS_VAR     .INT    #0     ; Variable to store user input (position in Fibonacci sequence)
            JMP     MAIN   ; Jump to the main program
FIB         PSHR    FP     ; Save the caller's frame pointer
            MOV     FP, SP ; Establish this function's frame pointer
            MOV     R0, FP ; Get the position argument from the stack
            ADDI    R0, #12 ; Offset to the position variable
            ILDR    R0, R0 ; Load the position into R0
            CMPI     R0, #0  ; Check if position is 0 or 1
            BRZ     R0, BASE_CASE ; If true, return 0 or 1
            CMPI     R0, #1
            BRZ     R0, BASE_CASE ; If true, return 0 or 1
            LDR     R8, ONE
            SUB    R0, R8  ; Calculate fib(n-1)
            PSHR    R0     ; Push argument for the recursive call
            ADDI    SP, #-4 ; Allocate space on the stack
            CALL    FIB     ; Recursive call
            POPR    R0     ; Retrieve the result
            SUB    R1, R8  ; Calculate fib(n-2)
            PSHR    R1     ; Push argument for the recursive call
            ADDI    SP, #-4 ; Allocate space on the stack
            CALL    FIB     ; Recursive call
            POPR    R1     ; Retrieve the result
            ADD     R0, R1 ; Calculate fib(n-1) + fib(n-2)
            JMP      END_FIB

BASE_CASE   CMPI     R0, #0  ; Base case: fib(0) = 0
            BRZ     R0, BASE_RET
            MOVI    R0, #1  ; Base case: fib(1) = 1
            JMP      END_FIB

BASE_RET    MOVI    R0, #0  ; Return 0 for invalid positions
END_FIB     POPR    FP     ; Restore the caller's frame pointer
            RET              ; Return to the caller
            
MAIN        LDA     R3, FIB_PRMPT ; Load the address of the prompt string
            TRP     #5            ; Print the prompt
            TRP     #2            ; Read an integer from the user
            STR     R0, POS_VAR   ; Store the entered position in POS_VAR
            LDA     R3, OUT_PRMPT ; Load the address of the output string
            TRP     #5            ; Print the output prefix
            LDR     R0, POS_VAR   ; Get the position from POS_VAR
            PSHR    R0            ; Push the argument for the Fibonacci function
            ADDI    SP, #-4       ; Allocate space on the stack
            CALL    FIB           ; Call the Fibonacci function
            POPR    R0            ; Retrieve the result
            TRP     #1            ; Print the result
            TRP     #0            ; Exit
