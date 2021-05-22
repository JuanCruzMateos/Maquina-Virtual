    \\ASM DATA=10 EXTRA=3000 STACK=5000
UNO  EQU   %1
tres EQU %3
cero  equ 0
    mov    [1], #-10
    mov    [2], cero
    add    [1], [2]
    mov    ax, %001
    mov    dx, TRES
    mov    cx, UNO
hola:    sys    1
    add    [1], [3]
    sys    %f
    mov    dx, uno
DOS     EQU #2
    sys    DOS
    stop