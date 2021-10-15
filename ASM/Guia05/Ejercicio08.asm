; 8. Escribir un programa en assembler que reciba dos listas de números (positivos, ordenados en
; forma creciente) y las intercale formando una tercera lista ordenada. BX y CX contienen las
; direcciones de inicio de las listas a intercalar, cuyos fines se indican con un número negativo. DX
; contiene la dirección inicial de la lista resultante.

null    equ     -1

main:           mov     bx, 1000        ; ptr a l1
                mov     cx, 1001        ; ptr a l2
                mov     dx, 1002        ; ptr a l3

                mov     [bx], 21
                mov     [cx], 38

                ; lista 1
                mov     [21], 2
                mov     [22], 27
                mov     [23], 29
                mov     [24], 29
                mov     [25], 16
                mov     [26], 23
                mov     [27], 14
                mov     [28], 25
                mov     [29], -1
                mov     [30], -1

                ; lista 2
                mov     [38], 3
                mov     [39], 42
                mov     [40], 21
                mov     [41], 44
                mov     [42], 11
                mov     [43], 40
                mov     [44], 23
                mov     [45], 46
                mov     [46], -1
                mov     [47], -1

                push    [bx]
                call    print
                add     sp, 1

                push    [cx]
                call    print
                add     sp, 1

                push    dx
                push    [cx]
                push    [bx]
                call    intercalar
                add     sp, 3

                push    [dx]
                call    print
                add     sp, 1
                stop

intercalar:     push    bp
                mov     bp, sp
                push    ax
                push    bx
                push    cx
                push    dx
                push    fx

                mov     ax, [bp+2]      ; ptr a l1
                mov     bx, [bp+3]      ; ptr a l2
                mov     fx, [bp+4]      ; &ptr a l3
inicio:         cmp     [ax], null
                jz      cambio          ; intercambio de manera que ax == null caundo fin
                cmp     [bx], null  
                jz      agregarA        ; si bx == null (puedo haver intercambiado) 
                jmp     comparar

cambio:         swap    ax, bx
                cmp     [ax], null
                jz      agregarNull
                jmp     agregarA
comparar:       cmp     [ax], [bx]
                jn      agregarA

                mov     cx, 2
                sys     5
                mov     [dx], [bx]
                mov     [fx], dx
                mov     fx, dx
                add     fx, 1

                mov     bx, [bx+1]
                jmp     inicio
agregarA:       mov     cx, 2
                sys     5
                mov     [dx], [ax]
                mov     [fx], dx
                mov     fx, dx
                add     fx, 1

                mov     ax, [ax+1]
                jmp     inicio 
agregarNull:    mov     cx, 2
                sys     5
                mov     [dx], null
                mov     [dx+1], null
                mov     [fx], dx
finIntercalar:  pop     fx
                pop     dx
                pop     cx
                pop     bx
                pop     ax
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

                mov     [bp-1], ' '
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
