test  .INT  #1
    JMP MAIN

MAIN LDR  R3,  test
     TRP #1
     MOVI R3, 'H'
     TRP #3
     TRP #0