; 2. Escribir una subrutina que emule el comportamiento de la instrucción SMOV, 
;    (como si esta no existiera).

        mov     ax, 0
        mov     dx, 10
        mov     cx, 10
        sys     %3
        
        push    10      ; origen
        push    60      ; destino
        call    copy
        add     sp, 2

        mov     dx, 60
        sys     %4
        stop

copy:   push    bp
        mov     bp, sp
        push    ax      ; origen
        push    bx      ; destino

        mov     ax, [bp+3]
        mov     bx, [bp+2]

iter:   cmp     [ax], 0
        jz      fin
        mov     [bx], [ax]
        add     ax, 1
        add     bx, 1
        jmp     iter


fin:    mov     [bx], 0
        pop     bx
        pop     ax
        mov     sp, bp
        pop     bp
        ret

