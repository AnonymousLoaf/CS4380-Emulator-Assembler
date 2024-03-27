TWO      .INT  2
       JMP   MAIN
MAIN   LDR   R3,  TWO
       ADDI  R3,  3
       TRP   #1
       MULI  R3,  5
       TRP   #1
       DIVI  R3,  5
       TRP   #1
       TRP   #0