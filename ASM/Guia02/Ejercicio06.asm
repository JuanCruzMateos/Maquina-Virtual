; 6. Suponiendo que no existe la instrucción DIV, crear un algoritmo que reciba en BX y CX los dos
;    valores y retorne en AX el resultado de la división, dejando en DX el resto.
    
    ; solo para numeros positivos
            mov     ax, 1
            mov     dx, 0
            mov     cx, 2
            sys     1               ; leo dos valores en las pos [0] y [1]

            cmp     [1], 0
            jz      nodiv           ; evito division por cero -> salto y solo muestro 0

            cmp     [0], 0          ; si divido por cero -> solo muestro cero
            jz      nodiv

            xor     ax, ax          ; registro donde calculo el resultado
            mov     bx, [0]
            mov     cx, [1]

            ; cmp     cx, 0
            ; jn      mulcx

            mul     cx, -1

iter:       add     ax, 1
            add     bx, cx
            cmp     bx, [1]
            jp      iter
            mov     dx, bx
            jmp     fin

nodiv:      mov     ax, 0
            mov     dx, 0

fin:        mov     [0], ax
            mov     [1], dx
            mov     ax, 1
            mov     dx, 0
            mov     cx, 2
            sys     2
            stop

