    \\ASM DATA=10 EXTRA=3000 STACK=5000
UNO  EQU   %1
UNO EQU %3
inicial equ %A
    mov    [1], inicial
    mov    [2], %1
    add    [1], [2]
    mov    ax, %001
    mov    dx, 3
    mov    cx, UNO
INICIAL:    sys    1
    add    [1], [3]
    sys    %f
    mov    dx, 1
DOS     EQU #2
    sys    DOS
    stop