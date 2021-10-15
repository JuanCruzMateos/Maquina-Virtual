; 3. Dado un valor decimal ingresado por teclado, imprimir su valor binario equivalente como una
;    secuencia de ceros y unos.

; Version 1: mostrando del menos significativo al mas significativo

;         mov     ax, %001
;         mov     dx, 1
;         mov     cx, 1
;         sys     1
; sig:    mov     [2], [1]      ; copio el numero
;         shr     [1], 1        
;         and     [2], 1        ; me quedo con el bit menos sig
;         mov     ax, %001
;         mov     dx, 2
;         mov     cx, 1
;         sys     2             ; muestro ese bit
        
;         cmp     [1], 0        ; si llegue a cero corto
;         jnz     sig
;         stop

; Version 2: mostrando del mas significativo al menos significativo

        xor     bx, bx         ; usado para guardar el numero invertido

        mov     ax, %001
        mov     dx, 1
        mov     cx, 1
        sys     1

otro:   mov     [2], [1]       ; invierto el nro y lo guardo en bx
        shr     [1], 1
        and     [2], 1
        shl     bx, 1
        or      bx, [2]
        cmp     [1], 0
        jnz     otro

most:   mov     [1], bx       ; lo muestro
        shr     bx, 1
        and     [1], 1
        mov     ax, %001
        mov     dx, 1
        mov     cx, 1

        sys     2
        cmp     bx, 0
        jnz     most
        stop

