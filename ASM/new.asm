    \\ASM  extra=20
    carlos equ  "Carlos"
    mov cx, 10
    sys %5
    sys %f
    ; mov dx, 123
    mov ax, %000
    sys %3
    sys %4
    mov     cx, 5
    sys %5
    sys %f
        sys %3
    sys %4
        mov     cx, 1
    sys %5
        sys %f
    stop