hola equ "Hola"
chau equ "Holas"
    ; mov  dx, 0
    ; mov  cx, 15
    ; mov  ax, %000
    ; sys  %3
    ; slen [10], [0]
    ; sys %f
    ; mov  ax, %001
    ; smov    [20], [0]

    mov ax, 0
    shl ax, 16
    or ax, hola
    mov bx, 0
    shl bx, 16
    or bx, chau

    scmp ax, bx
    mov ax, %001
    mov  dx, cc
    mov cx, 1
    ; mov  cx, 1
    sys  %2

    ; sys %4
    stop 
