; Input prompt strings
prompt      .STR  "Enter dividend: "
prompt2     .STR  "Enter divisor: "
result_str  .STR  "Remainder: "
NL          .BYT  '\n'
dividend    .INT  0
divisor     .INT  0
remainder   .INT  0
message_1   .STR  "Error: Division by zero!\n"
            JMP   MAIN

MOD         CMPI   R1, #0           ; Check if divisor is zero
            LDR   R3, divisor       ; Load divisor
            BRZ   R3, DivideByZero  ; Branch to error handling if divisor is zero
            MOVI  R0, #0            ; Initialize remainder to zero
            MOVI  R2, #0            ; Initialize quotient to zero
MODLoop     SUB   R0, R3            ; Subtract divisor from remainder
            ADDI  R2, #1            ; Increment quotient
            CMP   R0, R3            ; Check if remainder is less than divisor
            BGT   R0, MODLoop       ; Continue loop until remainder is less than divisor
            ADD   R0, R3            ; Adjust remainder to get the actual modulus
            RET

DivideByZero STR  R3, message_1     ; Store error string in R3
             TRP   #5               ; Print error string
             TRP   #0               ; Halt the program

MAIN        LDA   R3, prompt        ; Load prompt string
            TRP   #5                ; Print prompt
            TRP   #2                ; Read integer from user into R3
            STR   R3, dividend      ; Store user input in dividend
            LDA   R3, prompt2       ; Load prompt2 string
            TRP   #5                ; Print prompt2
            TRP   #2                ; Read integer from user into R3
            STR   R3, divisor       ; Store user input in divisor

            CALL  MOD               ; Call the mod function to calculate remainder

            ; Display the remainder
            LDA   R3, result_str   ; Load result string
            TRP   #5               ; Print result string
            LDR   R3, dividend     ; Load dividend
            TRP   #1               ; Print dividend as integer
            LDA   R3, result_str   ; Load result string
            TRP   #5               ; Print result string
            LDR   R3, R0           ; Load remainder
            TRP   #1               ; Print remainder as integer
            TRP   #0               ; Halt the program
