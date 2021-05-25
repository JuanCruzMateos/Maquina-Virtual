    mov     [5], %AA
    mov     ax, 5
    mov     bx, [AX]
    mov     [0], bx
    mov     ax, %008
    mov     dx, 0
    mov     cx, 1
    sys     2
    stop