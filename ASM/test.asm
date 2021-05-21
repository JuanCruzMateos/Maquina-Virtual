    \\ASM DATA=10 EXTRA=3000 STACK=5000
HOLA  EQU   1
    mov    [1], 0
    mov    [2], %1
    add    [1], [2]
    mov    ax, %001
    mov    dx, 3
    mov    cx, 1
    sys    1
    add    [1], [3]
    sys    %f
    mov    dx, 1
    sys    2
    stop