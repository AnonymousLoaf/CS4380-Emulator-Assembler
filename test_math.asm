INT_ONE  .INT    #1
INT_FIVE .INT    #5
BYTE_2   .BYT    #2
BYTE_8   .BYT    #8
NL       .BYT    '\n'
     JMP   MAIN


MAIN  LDB  R2, BYTE_2
      LDR  R1, INT_ONE
      LDB  R8, BYTE_8
      LDR  R5, INT_FIVE

      ; Move 8 into R3
      MOV  R3, R8
      ; Add 2 + 1
      ADD  R3, R1
      TRP  #1

      LDB  R3, NL
      TRP  #3
      
      ; Move 8 into R3
      MOV  R3, R8
      ; Subtract 8 - 5
      SUB  R3, R5
      TRP  #1

      LDB  R3, NL
      TRP  #3
      
      ; Move 5 into R3
      MOV  R3, R5
      ; Multiply 5 * 8
      MUL  R3, R8
      TRP  #1

      LDB  R3, NL
      TRP  #3

      MOV  R3, R8
      DIV  R3, R2
      TRP  #1

      LDB  R3, NL
      TRP  #3
      TRP  #98
      TRP  #0