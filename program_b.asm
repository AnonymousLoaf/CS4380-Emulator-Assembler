; Program B: Modulus Function

; Constants
INPUT_PROMPT        .STR    "Enter the dividend: "
DIVISOR_PROMPT      .STR    "Enter the divisor: "
RESULT_PROMPT       .STR    "The remainder is: "
NEWLINE             .BYT    '\n'

; Data
DIVIDEND            .INT    #0
DIVISOR             .INT    #0

; Function: mod
; Parameters: R0 (dividend), R1 (divisor)
; Returns: R0 (remainder)
mod:
    CMP     R1, #0          ; Check if divisor is zero
    BEQ     INVALID_DIVISOR ; If zero, return an error value (-1)

    MOV     R2, R0          ; Copy the dividend to R2
    DIV     R2, R1          ; Divide the dividend by the divisor

    MUL     R2, R1          ; Multiply the quotient by the divisor
    SUB     R0, R0, R2      ; Subtract the result from the original dividend to get the remainder

    RET

INVALID_DIVISOR:
    MOVI    R0, #-1         ; Set R0 to -1 to indicate an error
    RET

; Main Program
MAIN:
    LDA     R3, INPUT_PROMPT
    TRP     #5              ; Print the input prompt for the dividend
    TRP     #2              ; Read the dividend from the user
    MOV     DIVIDEND, R0    ; Store the dividend

    LDA     R3, DIVISOR_PROMPT
    TRP     #5              ; Print the input prompt for the divisor
    TRP     #2              ; Read the divisor from the user
    MOV     DIVISOR, R0     ; Store the divisor

    ; Call the mod function
    PSHR    DIVISOR         ; Push the divisor onto the stack
    PSHR    DIVIDEND        ; Push the dividend onto the stack
    CALL    mod             ; Call the mod function
    POPR    R0              ; Retrieve the remainder

    LDA     R3, RESULT_PROMPT
    TRP     #5              ; Print the result prompt
    TRP     #1              ; Print the remainder
    TRP     #5              ; Print newline

    TRP     #0              ; Exit
