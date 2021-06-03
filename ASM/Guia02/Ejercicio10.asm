; 10. Hacer un programa que permita ingresar una lista de números, que finaliza con un número
;     negativo. Luego el programa pedirá ingresar n-1 números de la misma lista, y al finalizar la
;     carga deberá mostrar el número que falta de la lista original. Nota: los números de la primera
;     lista pueden ser ingresados en cualquier orden. Ejemplo: se ingresan 4,5,3,7,-1 luego 3,7,4 y
;     muestra 5.

            xor     fx, fx      ; suma = 0
            xor     ex, ex      ; cant = 0
            mov     bx, 1
            
            mov      ax, 1
            mov      dx, 0
            mov      cx, 1
leer:       sys      1

            cmp     [0], 0
            jn      segunda
            add     fx, [0]
            add     ex, 1
            jmp     leer

segunda:    sys     1
            mul     [0], -1
            add     fx, [0]
            add     bx, 1
            cmp     ex, bx
            jnz     segunda

            mov     [0], fx
            sys     2
            stop


;           otra forma -> usando xor

;             xor     fx, fx      ; suma = 0
;             xor     ex, ex      ; cant = 0
;             mov     bx, 1
            
;             mov      ax, 1
;             mov      dx, 0
;             mov      cx, 1
; leer:       sys      1
;             cmp     [0], 0
;             jn      segunda
;             xor     fx, [0]
;             add     ex, 1
;             jmp     leer

; segunda:    sys     1
;             xor     fx, [0]
;             add     bx, 1
;             cmp     ex, bx
;             jnz     segunda

;             mov     [0], fx
;             sys     2
;             stop
