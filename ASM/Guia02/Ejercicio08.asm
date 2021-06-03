; 8. Se ingresan una serie de números naturales, terminada con un número negativo. Mostrar por
;    cada número ingresado la cantidad de bits en 1 que contiene su representación binaria.

sig:        mov     ax, 1
            mov     dx, 0
            mov     cx, 1
            sys     1

            cmp     [0], 0
            jn      fin

            xor     ex, ex    ; acumular

iter:       cmp     [0], 0
            jz      show
            mov     bx, [0]
            shr     [0], 1
            and     bx, 1
            add     ex, bx
            jmp     iter

show:       mov     [2], ex
            mov     ax, %001
            mov     dx, 2
            mov     cx, 1
            sys     2
            jmp     sig
fin:        stop
