    mov [1], 'H'
    mov [2], 'o'
    mov [3], 'l'
    mov [4], 'a'
    mov [5], %00
    mov DX, 1
    mov CX, 4
    mov AX, %010
    sys %2
    stop