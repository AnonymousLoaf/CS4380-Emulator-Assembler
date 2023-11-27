        jmp     MAIN
MAIN    movi    r3, #10000
        movi    r8, #1
loop    sub     r3, r8
        bnz     r3, loop
        trp     #0
