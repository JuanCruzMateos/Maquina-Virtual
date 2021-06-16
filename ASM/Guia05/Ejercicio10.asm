; 10. Usando la misma estructura de lista que el ejercicio anterior, crear una subrutina recursiva a la cual
; se le pasa por parámetro el puntero al primer nodo y reordena la lista en sentido inverso retornando
; en el mismo parámetro el nuevo “primer nodo”. (NOTA: no es necesario utilizar el “valor”
; almacenado en cada nodo)
null    equ     -1

main:           mov     bx, 1000        ; bx = #mem de ptr a lista

                mov     [bx], 1

                mov     [1], 3
                mov     [2], 5
                mov     [3], 7
                mov     [4], 9
                mov     [5], 6
                mov     [6], 3
                mov     [7], 12
                mov     [8], -1
                mov     [9], 10
                mov     [10], 7

                push    [bx]
                call    print
                add     sp, 1

                push    [bx]
                call    invertir
                pop     [bx]

                push    [bx]
                call    print
                add     sp, 1
                stop

; Especificacion :: invertir
;   push   < ptr 1 nodo de lista > 
;   call    invertir
;   pop    < ptr nuevo 1 nodo de lista >
invertir:       push    bp
                mov     bp, sp
                push    bx
                push    cx

                mov     bx, [bp+2]
                cmp     bx, null
                jz      finInvertir 

                mov     cx, [bx+1]
                cmp     cx, null
                jz      finInvertir

                push    cx
                call    invertir
                pop     [bp+2]

                mov     [cx+1], bx  
                mov     [bx+1], null
finInvertir:    pop     cx
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
                sys     %2
                
                ; imprimo un espacio
                mov     ax, %910
                mov     dx, bp
                sub     dx, 1
                mov     cx, 1
                sys     %2

                ; actualizo el ptr
                mov     fx, [fx+1]
                jmp     sig
                
newline:        mov     ax, %810
                mov     dx, bp
                sub     dx, 1
                mov     cx, 1
                sys     %2
finPrint:       pop     ax
                pop     cx
                pop     dx
                pop     fx
                add     sp, 1
                mov     sp, bp
                pop     bp
                ret