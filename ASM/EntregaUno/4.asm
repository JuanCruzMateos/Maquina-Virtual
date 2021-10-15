    xor     [%FF], [%FF]
    add     [%FF], %F 
    mov     ax, %8
    sys      %F
    mov     cx, 1
    ;mov     dx, %FFF
    mov     dx, %FF
    mov     fx, 31
CMP1:   cmp     fx, 0 
        jn      fin 
        mov     ex, fx
        sub     fx, 1
        div     ex, 4
        cmp     ac, 0 
        jz      line
        or    ax, %100
        sys     %f
SYS2:    sys     2
    shl     [%FF], 1
    jmp     CMP1
line: and    ax, %EFF
        sys   %f
    jmp     SYS2
FIN: stop    

