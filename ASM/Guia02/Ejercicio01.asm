;   1. Calcular e imprimir el promedio de una lista de numeros naturales que se ingresan por teclado
;      El fin de la lista se indica cin un numero negativo.

            xor     [1], [1]    ; suma = 0
            xor     [2], [2]    ; cant = 0

leer:       mov     ax, %001
            mov     dx, 3
            mov     cx, 1
            sys     1           ; leo el dato

            cmp     [3], 0      ; si es genativo -> corto
            jn      mostrar

            add     [1], [3]    ; suma += dato
            add     [2], 1      ; cant++
            jmp     leer

mostrar:    cmp     [2], 0
            jz      fin         ; evito division por cero

            div     [1], [2]    ; calculo el resultado
fin:        mov     ax, %001    ; muestro
            mov     dx, 1
            mov     cx, 1
            sys     2
            stop

