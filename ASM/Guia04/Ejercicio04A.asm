; a. Un String que representa un número entero (positivo o negativo) en base 10 a un número.

mask   equ   %F  

            mov     ax, %000
            mov     dx, 10
            mov     cx, 20
            sys     %3          ; sys read -> #mem 10 del DS, 20 chars

            push    10          ; invocacion
            call    parseInt    ; return en AX
            add     sp, 1

            mov     [0], ax
            mov     ax, %001
            mov     dx, 0
            mov     cx, 1
            sys     %2
            stop

parseInt:   push    bp
            mov     bp, sp
            push    bx
            push    cx
            push    ex
            push    fx

            xor     ax, ax      ; inicializo ret
            mov     ex, 1       ; registro de signo
            mov     bx, [bp+2]  ;

            cmp     [bx], '-'
            jnz     inicio
            mov     ex, -1      ; si neg -> ex = -1
            add     bx, 1       ; salteo el signo
            add     [bp+2], 1   ; salteo el signo

inicio:     slen    cx, [bx]
            sys     %f
            sub     cx, 1
            add     bx, cx
            mov     fx, 1
iter:       cmp     bx, [bp+2]
            jn      fin
            mov     cx, [bx]
            and     cx, mask
            mul     cx, fx
            add     ax, cx
            mul     fx, 10
            sub     bx, 1
            jmp     iter
fin:        mul     ax, ex
            pop     fx
            pop     ex
            pop     cx
            pop     bx
            mov     sp, bp
            pop     bp
            ret
