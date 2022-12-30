; codigo com dois rotulos na mesma linha
; o output ficou meio esquisito
SECTION TEXT
ROTULO: ROTULO:
INICIA: 
    INPUT N
    INPUT N+1
LOAD N
MUL N+1
OUTPUT N
FIM: STOP FIM:
SECTION DATA
OI: CONST 10
N: SPACE 3
