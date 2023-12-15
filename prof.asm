test  .INT  #3
    JMP MAIN

MAIN LDA  R3, test
     MOVI R7, #50
     ISTR R7, R3
     LDR R3, test
     TRP #1
     TRP #98
     TRP #0