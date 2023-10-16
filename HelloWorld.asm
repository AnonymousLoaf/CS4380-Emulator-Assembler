H       .Byt        'H'
e       .byt        'e'
l       .BYT        'l'
o       .BYT        'o'
w       .BYT        'w'
r       .BYT        'r'
d       .BYT        'd'
Y       .BYT        'Y'
y       .BYT        'y'
u       .BYT        'u'
V       .BYT        'V'
M       .BYT        'M'
i       .BYT        'i'
s       .BYT        's'
k       .BYT        'k'
n       .BYT        'n'
g       .BYT        'g'
PLUS    .BYT        '+'
MIN     .BYT        '-'
TIMES   .BYT        '*'
DIVIDE  .BYT        '/'
EQ      .BYT         '='
G       .BYT        'G'
b       .BYT        'b'
SPACE   .BYT        ' '  ; 0x20 ;
NL      .BYT        '\n' ; 0x0A ;
ONE     .INT        #1
TWO     .int        #02  ; 0x02
THR     .INT        #3
FIV     .INT        #5
        JMP     MAIN ;does this help?

MAIN    LDB         R3,          H
        TRP         #3
        LDB         R3,          e
        TRP         #3
        LDB         R3,          l
        TRP         #3
        TRP         #3
        LDB         R3,          o
        TRP         #3
        LDB         R3,          SPACE
        TRP         #3
        LDB         R3,          w
        TRP         #3
        LDB         R3,          o
        TRP         #3
        LDB         R3,          r
        TRP         #3
        LDB         R3,          l
        TRP         #3
        LDB         R3,          d
        TRP         #3
        LDB         R3,          NL
        TRP         #3
        JMP         ADDTest

ADDTest LDR         R3,         ONE
        TRP         #1
        LDB         R3,         SPACE
        TRP         #3
        LDB         R3,         PLUS
        TRP         #3
        LDB         R3,         SPACE
        TRP         #3
        LDR         R3,         ONE
        TRP         #1
        LDB         R3,         SPACE
        TRP         #3
        LDB         R3,         EQ
        TRP         #3
        LDB         R3,         SPACE
        TRP         #3
        LDR         R1,          ONE
        MOV         R3,          R1
        ADD         R3,          R1
        TRP         #1
        LDB         R3,          NL
        TRP         #3
        JMP         SUBTest
        TRP         #0

MULTest LDR         R3,         TWO
        TRP         #1
        LDB         R3,         SPACE
        TRP         #3
        LDB         R3,         TIMES
        TRP         #3
        LDB         R3,         SPACE
        TRP         #3
        LDR         R3,         THR
        TRP         #1
        LDB         R3,         SPACE
        TRP         #3
        LDB         R3,         EQ
        TRP         #3
        LDB         R3,         SPACE
        TRP         #3
        LDR         R1,         TWO
        LDR         R3,         THR
        MUL         R3,         R1
        TRP         #1
        LDB         R3,          NL
        TRP         #3
        JMP         DIVTest   

DIVTest LDR         R3,         FIV
        TRP         #1
        LDB         R3,         SPACE
        TRP         #3
        LDB         R3,         DIVIDE
        TRP         #3
        LDB         R3,         SPACE
        TRP         #3
        LDR         R3,         TWO
        TRP         #1
        LDB         R3,         SPACE
        TRP         #3
        LDB         R3,         EQ
        TRP         #3
        LDB         R3,         SPACE
        TRP         #3
        LDR         R1,          TWO
        LDR         R3,          FIV
        DIV         R3,          R1
        TRP         #1
        LDB         R3,          NL
        TRP         #3
        JMP         VMWORK

SUBTest LDR         R3,         TWO
        TRP         #1
        LDB         R3,         SPACE
        TRP         #3
        LDB         R3,         MIN
        TRP         #3
        LDB         R3,         SPACE
        TRP         #3
        LDR         R3,         ONE
        TRP         #1
        LDB         R3,         SPACE
        TRP         #3
        LDB         R3,         EQ
        TRP         #3
        LDB         R3,         SPACE
        TRP         #3
        LDR         R1,          ONE
        LDR         R3,          TWO
        SUB         R3,          R1
        TRP         #1
        LDB         R3,          NL
        TRP         #3
        JMP         MULTest    

        
VMWORK  LDB         R3,          Y
        TRP         #3
        LDB         R3,          o
        TRP         #3
        LDB         R3,          u
        TRP         #3
        LDB         R3,          r
        TRP         #3
        LDB         R3,          SPACE
        TRP         #3
        LDB         R3,          V
        TRP         #3
        LDB         R3,          M
        TRP         #3
        LDB         R3,          SPACE
        TRP         #3
        LDB         R3,          i
        TRP         #3
        LDB         R3,          s
        TRP         #3
        LDB         R3,          SPACE
        TRP         #3
        LDB         R3,          w
        TRP         #3
        LDB         R3,          o
        TRP         #3
        LDB         R3,          r
        TRP         #3
        LDB         R3,          k
        TRP         #3
        LDB         R3,          i
        TRP         #3
        LDB         R3,          n
        TRP         #3
        LDB         R3,          g
        TRP         #3
        LDB         R3,          NL
        TRP         #3
        LDB         R3,          G
        TRP         #3
        LDB         R3,          o
        TRP         #3
        LDB         R3,          o
        TRP         #3
        LDB         R3,          d
        TRP         #3
        LDB         R3,          b
        TRP         #3
        LDB         R3,          y
        TRP         #3
        LDB         R3,          e
        TRP         #3
        TRP         #0
        