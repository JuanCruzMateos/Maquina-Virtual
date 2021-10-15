; 9. Escribir una subrutina que permite fusionar 2 listas dinámicas ordenadas con números positivos. La
; subrutina recibe por parámetro los punteros a ambas listas y devuelve en AX el puntero al primer
; nodo resultante de la fusión. La subrutina debe modificar los punteros de los nodos de las listas.

null    equ     -1

main:           mov     bx, 1000        ; bx = #mem de ptr a lista
                mov     cx, 1001        ; cx = #mem de ptr a lista

                mov     [bx], 1
                mov     [cx], 7

                mov     [1], 3
                mov     [2], 11
                mov     [3], 7
                mov     [4], -1
                mov     [5], 6
                mov     [6], 3
                mov     [7], 2
                mov     [8], 9
                mov     [9], 4
                mov     [10], 13
                mov     [11], 5
                mov     [12], 5
                mov     [13], 10
                mov     [14], -1

                push    [bx]
                call    print
                add     sp, 1

                push    [cx]
                call    print
                add     sp, 1

                push    [cx]
                push    [bx]
                call    fusionar
                add     sp, 2

                push    ax
                call    print
                add     sp, 1
                stop

; Especificacion :: fusionar -> Tlista fusionar(TLista L1, TLista L2)
;   push   < ptr Lista 2 > 
;   push   < ptr Lista 1 > 
;   call    fusionar
;   add     sp, 2
;   AX  <-  ptr al primer nodo fusionado 
fusionar:       push    bp
                mov     bp, sp
                push    bx
                push    cx
                push    fx

                mov     bx, [bp+2]  ; lista 1
                mov     cx, [bp+3]  ; lista 2

                cmp     bx, null
                jz      pasarCX
                cmp     cx, null
                jz      pasarBX
                jmp     verPrimero
pasarCX:        mov     ax, cx
                jmp     finFusionar
pasarBX:        mov     ax, bx
                jmp     finFusionar
verPrimero:     cmp     [bx], [cx]
                jp      primeroCX
                mov     ax, bx
                mov     fx, bx
                mov     bx, [bx+1]
                jmp     notNull
primeroCX:      mov     ax, cx
                mov     fx, cx
                mov     cx, [cx+1]
notNull:        cmp     bx, null
                jz      intercambiar
                cmp     cx, null
                jz      appendBX
                jmp     comparar
intercambiar:   swap    bx, cx
                cmp     bx, null
                jz      finFusionar
appendBX:       mov     [fx+1], bx
                jmp     finFusionar
comparar:       cmp     [bx], [cx]
                jp      agregarCX
                mov     [fx+1], bx
                mov     fx, [fx+1]                                                
                mov     bx, [bx+1]
                jmp     notNull
agregarCX:      mov     [fx+1], cx
                mov     fx, [fx+1]
                mov     cx, [cx+1]
                jmp     notNull
finFusionar:    pop     fx
                pop     cx
                pop     bx
                mov     sp, bp
                pop     bp
                ret
; Especificacion :: print -> void print(TLista L)
;   push   < ptr a lista >
;   call    print
;   add     sp, 1
print:          push    bp
                mov     bp, sp
                sub     sp, 1
                push    fx
                push    dx
                push    cx
                push    ax

                mov     [bp-1], ' '         ; separador
                mov     fx, [bp+2]          ; fx = ptr nodo
sig:            cmp     fx, null
                jz      newline

                ; imprimo el numero
                mov     ax, %901
                mov     dx, fx
                mov     cx, 1
                sys     2
                
                ; imprimo un espacio
                mov     ax, %910
                mov     dx, bp
                sub     dx, 1
                mov     cx, 1
                sys     2

                ; actualizo el ptr
                mov     fx, [fx+1]
                jmp     sig
                
newline:        mov     ax, %810
                mov     dx, bp
                sub     dx, 1
                mov     cx, 1
                sys     2
finPrint:       pop     ax
                pop     cx
                pop     dx
                pop     fx
                add     sp, 1
                mov     sp, bp
                pop     bp
                ret