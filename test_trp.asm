BYTE_1  .BYT
BYTE_2  .BYT
INT_1   .INT
INT_2   .INT
NL      .BYT    '#10'
        JMP     MAIN


MAIN    TRP  #2
        TRP  #1
        LDB  R3, NL
        TRP  #3
        TRP  #4
        TRP  #3
        LDB  R3, NL
        TRP  #3
        TRP  #0
