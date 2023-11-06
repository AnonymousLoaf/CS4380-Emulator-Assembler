H    .Byt       'J'
Two   .Byt       #2
ASCII 	.BYT  #255
POUND .byt   '#'
NL      .byt  '\n'
S			.bYt  '#83'
NONE        .byt

    JMP         MAIN

MAIN LDB         R3, NONE
     TRP         #1
     LDB         R3, NL
     TRP         #3
     MOV         R3, R5
     TRP         #3
     LDB         R3, NL
     TRP         #3