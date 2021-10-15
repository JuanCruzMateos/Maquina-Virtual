    mov [10], %41
    SHL [10], 8
    OR  [10], %61
    MOV DX, 10
    MOV CX, 1
    MOV AX, %01F
    SYS %2
    STOP