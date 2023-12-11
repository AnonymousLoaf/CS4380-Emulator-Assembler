a  .INT  #5
b  .INT  #6
   JMP MAIN

MAIN TRP #98
     CALL  ADDR
     TRP #1
     TRP #0

ADDR  LDR  R5, a
      LDR  R6, b
      ADD  R5, R6
      STR  R3, R5
      RET