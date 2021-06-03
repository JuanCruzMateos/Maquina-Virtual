; 4. Suponiendo que no existe la instrucción MUL, crear un algoritmo que reciba en BX y CX los dos
;    valores y retorne en AX su producto

            mov     ax, %001
            mov     dx, 1
            mov     cx, 2
            sys     1               ; leo el valor

            xor     ax, ax
            mov     bx, [1]
            mov     cx, [2]
            
            cmp     bx, 0           ; verifico si alguno es cero -> salto a fin directo
            jz      fin
            cmp     cx, 0
            jz      fin

            cmp     bx, 0
            jn      signocx
            cmp     cx, 0
            jn      cambiar

calc:       add     ax, bx          ;   +/+  ||  -/+
            add     cx, -1
            cmp     cx, 0
            jnz     calc
            jmp     fin            

signocx:    cmp     cx, 0
            jn      cambiar
            jmp     calc

cambiar:    mul     bx, -1         ; si es +/- || -/-  convierto en  -/+ || +/+ para calcular
            mul     cx, -1
            jmp     calc

fin:        mov     [0], ax
            mov     ax, 1
            mov     dx, 0
            mov     cx, 1
            sys     2
            stop

