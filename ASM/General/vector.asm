; Especificacion :: printVec ::  void printVec(char **vec, int n)
;   push    < n: dimension logica del vector >
;   push    < ptr a vec >
;   call    printVec
;   add     sp, 2
printVec:       push    bp
                mov     bp, sp
                push    ax
                push    bx
                push    dx
                push    fx

                xor     bx, bx
                mov     fx, [bp+2]
iterPrint:      cmp     bx, [bp+3]
                jp      finPrint
                mov     dx, [fx]        ; en fx tengo (vec + i), [fx] == *(vec + i)
                mov     ax, %800
                sys     %4
                add     bx, 1
                add     fx, 1
                jmp     iterPrint
finPrint:       pop     fx
                pop     dx
                pop     bx
                pop     ax
                mov     sp, bp
                pop     bp
                ret