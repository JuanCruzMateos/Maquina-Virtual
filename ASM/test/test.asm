    \\ASM data=10 STACK=30
UNO  EQU   %1
juan EQU "JUAN"
tres EQU %3
cero  equ 0
    sys    %f
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
    mov    ax, 5
    mov    bx, [   ax    -   dos  ]
    mov    [5], bx
cinco       equ 5
    mov     dx, cinco
    sys     dos
    jp      [ex+15]
    stop