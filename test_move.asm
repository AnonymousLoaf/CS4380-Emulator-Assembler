INT_ONE  .INT  #1
INT_TWO  .INT  #2
BYTE_ONE .BYT  'O'
BYTE_TWO .BYT  'T'
IO_EMP   .INT
IT_EMP   .INT
BO_EMP   .BYT
BT_EMP   .BYT
NL       .BYT  '#10'

    JMP     MAIN

MAIN    LDB  R0, NL
        MOV  R3, R0
        TRP  #3
        LDR  R1, INT_ONE
        STR  R1, IO_EMP
        LDR  R3, IO_EMP
        TRP  #1
        MOV  R3, R0
        TRP  #3
        LDR  R2, INT_TWO
        STR  R2, IT_EMP
        LDR  R3, IT_EMP
        TRP  #1
        MOV  R3, R0
        TRP  #3
        LDB  R5, BYTE_ONE
        STB  R5, BO_EMP
        LDB  R3, BO_EMP
        TRP  #3
        MOV  R3, R0
        TRP  #3
        LDB  R6, BYTE_TWO
        STB  R6, BT_EMP
        LDB  R3, BT_EMP
        TRP  #3
        MOV  R3, R0
        TRP  #3
        TRP  #0
