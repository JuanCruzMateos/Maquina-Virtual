;   9. Modificar el ejercicio anterior de modo que antes de ingresar la lista se lea un valor que
;      represente una máscara con la cual se debe realizar un AND a cada número de la lista antes
;      de calcular la cantidad de bits en 1.
           
            mov     ax, 1
            mov     dx, 0
            mov     cx, 1
            sys     1

            mov     fx, [0]     ; leo el valor en [0] y lo guardo en fx

sig:        mov     ax, 1
            mov     dx, 0
            mov     cx, 1
            sys     1

            cmp     [0], 0
            jn      fin

            xor     ex, ex      ; acumular

iter:       cmp     [0], 0
            jz      show
            mov     bx, [0]
            shr     [0], 1
            and     bx, fx      ; aplico el AND
            add     ex, bx
            jmp     iter

show:       mov     [2], ex
            mov     ax, %001
            mov     dx, 2
            mov     cx, 1
            sys     2
            jmp     sig
fin:        stop
