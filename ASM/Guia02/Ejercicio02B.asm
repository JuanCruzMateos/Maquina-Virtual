;   2. Dada una lista de ceros y unos que se ingresa por teclado, imprimir el valor decimal
;      equivalente. El fin de la lista se indica con un número distinto de 0 y 1.
;      b. La lista se ingresa del bit más significativo al menos significativo.

            xor     [1], [1]
            xor     [2], [2]

otro:       mov     ax, %001
            mov     dx, 1
            mov     cx, 1
            sys     1

            cmp     [1], 0 ; si es cero -> calcula
            jz      sigue
            cmp     [1], 1 ; si no es uno -> fin 
            jnz     fin

sigue:      shl     [2], 1
            or      [2], [1]
            jmp     otro

fin:        mov     ax, %001
            mov     dx, 2
            mov     cx, 1
            sys     2
            stop
