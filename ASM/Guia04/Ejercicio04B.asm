; b. Un número entero a su representación en String.
mask    equ     %30

                mov     ax, %001
                mov     dx, 0
                mov     cx, 1
                sys     1

                push    [0]             ; invocacion de subrutina
                push    20
                call    parseStr
                add     sp, 2

                mov     dx, 20
                sys     4
                stop

parseStr:       push    bp
                mov     bp, sp
                push    ax              ; ptr str
                push    bx              ; ptr inicio
                push    cx
                push    ac

                sys     %f
                mov     ax, [bp+2]      ; #mem inicio str
                mov     cx, [bp+3]
                cmp     cx, 0
                jp      sigue
                mov     [ax], '-'
                add     ax, 1
                mul     cx, -1
sigue:          mov     bx, ax          ; guardo puntero al inicio
nextNum:        div     cx, 10
                or      ac, mask
                mov     [ax], ac
                add     ax, 1
                cmp     cx, 0
                jz      interc
                jmp     nextNum

interc:         mov     [ax], 0
                sub     ax, 1
iter:           cmp     ax, bx
                jn      fin
                swap    [ax], [bx]
                add     bx, 1
                sub     ax, 1
                jmp     iter

fin:            pop     ac
                pop     cx
                pop     bx
                pop     ax
                mov     sp, bp
                pop     bp
                ret
