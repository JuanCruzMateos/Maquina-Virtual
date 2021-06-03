; 5. Utilizando el algoritmo anterior, construya un programa que calcule el factorial de un número
;    ingresado por teclado.

            mov     fx, 1

            mov     ax, 1
            mov     dx, 0
            mov     cx, 1
            sys     1

            cmp     [0], 0     ; si es cero o uno -> print 1
            jz      fin
            cmp     [0], 1
            jz      fin
            jmp     otro

;  Algoritmo para MUL
mult:       xor     ax, ax      ; inicializo donde guardo el resultado
            
            cmp     bx, 0       ; verifico si alguno es cero -> salto a fin directo
            jz      sig
            cmp     cx, 0
            jz      sig

            cmp     bx, 0
            jn      signocx
            cmp     cx, 0
            jn      cambiar

calc:       add     ax, bx      ;   +/+  ||  -/+
            add     cx, -1
            cmp     cx, 0
            jnz     calc
            jmp     sig            

signocx:    cmp     cx, 0
            jn      cambiar
            jmp     calc

cambiar:    mul     bx, -1     ; convierto en -/+ || +/+
            mul     cx, -1
            jmp     calc

; Fin algotimo para MUL

otro:       mov     bx, [0]
            mov     cx, fx
            jmp     mult
        
sig:        mov     fx, ax
            add     [0], -1
            cmp     [0], 0
            jnz     otro

fin:        mov     [0], fx
            mov     ax, 1
            mov     dx, 0
            mov     cx, 1
            sys     2
            stop



;   Utilizando intruccion MUL
;         mov     fx, 1      ; donde guardo el resultado

;         mov     ax, 1
;         mov     dx, 0
;         mov     cx, 1
;         sys     1

;         cmp     [0], 0     ; si es cero o uno -> print 1
;         jn      fin
;         cmp     [0], 1
;         jn      fin

; otro:   mul     fx, [0]
;         add     [0], -1
;         cmp     [0], 0
;         jnz     otro


; fin:    mov     [0], fx
;         mov     ax, 1
;         mov     dx, 0
;         mov     cx, 1
;         sys     2
;         stop

