;   7. Dado un número natural, imprimir un 1 si es primo y un 0 si no lo es.

            mov     ax, %001
            mov     dx, 1
            mov     cx, 1
            sys     1

            cmp     [1], 2
            jz      noprimo

            mov     [2], 1      ; asumo que de entrada el numero es primo
            mov     fx, 2       ; empiezo probando dividir por 2
iter:       cmp     [1], fx     ; hasta n-1 -> mas eficiente sqrt(n) ?
            jnp     fin         ; cuando fx == n -> no es primo -> fin
            mov     ex, [1]
            div     ex, fx
            cmp     ac, [0]
            jz      noprimo
            add     fx, 1
            jmp     iter

noprimo:    mov     [2], 0

fin:        mov     ax, %001
            mov     dx, 2
            mov     cx, 1
            sys     2
            stop
