; Prime Number Generator Program

; Constants
LOWER_BOUND_PROMPT  .STR    "Welcome to the Prime Number Generator.\nThis program searches for and displays the first 20 prime numbers greater than or equal to a user-provided lower bound.\nPlease enter a lower bound: "
RESULT_PROMPT       .STR    "The first 20 prime numbers greater than or equal to "
NEWLINE             .BYT    '\n'

; Data
LOWER_BOUND         .INT    #0
PRIME_COUNT         .INT    #0
PRIME_ARRAY         .INT    20  ; Array to store the first 20 prime numbers

; Function: is_prime
; Parameters: R0 (number to be evaluated for primality)
; Returns: R0 (1 if prime, 0 if not prime)
is_prime:
    CMP     R0, #1          ; Check if the number is less than 2
    BLT     NOT_PRIME

    MOVI    R1, #2          ; Set divisor to 2
CHECK_DIVISOR:
    CMP     R1, R0          ; Compare divisor to the number
    BGE     IS_PRIME        ; If divisor >= number, the number is prime

    MOV     R2, R0          ; Copy the number to R2
    DIV     R2, R1          ; Divide the number by the divisor
    CMP     R2, #0          ; Check if the remainder is 0
    BEQ     NOT_PRIME       ; If remainder is 0, the number is not prime

    ADDI    R1, #1          ; Increment the divisor
    BR      CHECK_DIVISOR   ; Check the next divisor

IS_PRIME:
    MOVI    R0, #1          ; Set R0 to 1 (prime)
    RET

NOT_PRIME:
    MOVI    R0, #0          ; Set R0 to 0 (not prime)
    RET

; Function: generate_primes
; Parameters: R0 (lower bound)
; Returns: None
generate_primes:
    MOV     LOWER_BOUND, R0 ; Store the lower bound

    MOVI    R1, #0          ; Initialize prime count
    MOVI    R2, #2          ; Start checking from 2

CHECK_PRIMES:
    CALL    is_prime        ; Check if R2 is prime
    CMP     R0, #1          ; Check the result
    BEQ     ADD_TO_LIST     ; If prime, add to the list

    ADDI    R2, #1          ; Increment to the next number
    BR      CHECK_PRIMES    ; Check the next number

ADD_TO_LIST:
    ISTR    R2, PRIME_ARRAY, R1  ; Add the prime number to the list
    ADDI    R1, #1          ; Increment the prime count
    CMP     R1, #20         ; Check if we have found 20 primes
    BGE     PRINT_RESULT    ; If yes, print the result

    ADDI    R2, #1          ; Increment to the next number
    BR      CHECK_PRIMES    ; Check the next number

PRINT_RESULT:
    LDA     R3, RESULT_PROMPT
    TRP     #5              ; Print result prompt
    TRP     #1              ; Print the lower bound value
    TRP     #5              ; Print newline

    MOVI    R4, #0          ; Initialize loop counter
PRINT_LOOP:
    IFL     R4, PRIME_ARRAY ; Check if loop counter < 20
    BEQ     END_PROGRAM     ; If not, end the program

    ILDR    R5, PRIME_ARRAY, R4 ; Load the prime number from the list
    TRP     #1              ; Print the prime number
    TRP     #5              ; Print newline

    ADDI    R4, #1          ; Increment the loop counter
    BR      PRINT_LOOP      ; Print the next prime number

END_PROGRAM:
    TRP     #0              ; Exit

; Main Program
MAIN:
    LDA     R6, LOWER_BOUND_PROMPT
    TRP     #5              ; Print the lower bound prompt
    TRP     #2              ; Read the lower bound from the user

    CALL    generate_primes ; Generate the first 20 primes
    BR      END_PROGRAM     ; End the program
