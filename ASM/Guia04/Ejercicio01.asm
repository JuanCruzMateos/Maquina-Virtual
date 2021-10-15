;  1. Hacer una rutina recursiva que calcule la potencia de 2 números pasados como parámetro
;     OBS -> devolver en AX por defecto

        ; leo los datos
            mov     ax, %001
            mov     dx, 0
            mov     cx, 2
            sys     1

        ; invoco la rutina
            push    [0]         ; base
            push    [1]         ; exp
            call    pot
            add     sp, 2
        
        ; muestro el resultado
            mov     [2], ax
            mov     ax, %001
            mov     dx, 2
            mov     cx, 1
            sys     2
            stop

pot:        push    bp
            mov     bp, sp
            push    bx
            cmp     [bp+2], 0   ; if (base != 0)
            jnz     sigo
            mov     ax, 1
            jmp     fin

sigo:       mov     bx, [bp+2]  ; bx = exp
            sub     bx, 1       ; exp = exp-1
            push    [bp+3]      ; base
            push    bx          ; exp
            call    pot
            add     sp, 2
            mul     ax, [bp+3]


fin:        pop     bx
            mov     sp, bp
            pop     bp
            ret

