; 2. Dada una lista de ceros y unos que se ingresa por teclado, imprimir el valor decimal
;    equivalente. El fin de la lista se indica con un número distinto de 0 y 1.
; a. La lista se ingresa del bit menos significativo al más significativo.

            xor     [1], [1]
            xor     [2], [2]
            mov      fx, 1

otro:       mov     ax, %001
            mov     dx, 1
            mov     cx, 1
            sys     1

            cmp     [1], 0      ; si es cero -> calcula
            jz      sigue
            cmp     [1], 1      ; si no es uno -> fin 
            jnz     fin

sigue:      mul     [1], fx
            add     [2], [1]
            mul     fx, 2
            jmp     otro

fin:        mov     ax, %001
            mov     dx, 2
            mov     cx, 1
            sys     2
            stop
