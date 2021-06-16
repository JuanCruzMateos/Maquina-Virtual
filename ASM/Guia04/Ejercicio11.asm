; 11. Crear una estructura que permita cargar 2 matrices. Resolver mediante subrutinas:
;   a. Hacer una subrutina que permita hacer la suma o resta entre ambas matrices.
;   b. Hacer una subrutina para validar si es posible multiplicar ambas matrices.
;   c. Hacer una subrutina que multiplique ambas matrices (si es posible).
;   d. Hacer una subrutina que imprima prolijamente una matriz por pantalla. 
;      Nota: utilizar las funciones creadas en los ejercicios anteriores de manejo de String para convertir una fila de números
;      de la matriz a un String e imprimirlo como tal.
;   e. Hacer una subrutina que permita copiar una matriz en forma transpuesta.

prompt      equ     "Ingrese elementos de la matriz (3x3):"
espacio     equ     " "
; reemplazar espacio por memoria dinamica y despues liberar
DIMENSION   equ     10

matriz1     equ     0
matriz2     equ     100

matriz3     equ     150

                
                ldh     3
                ldl     prompt
                mov     ax, %800
                mov     dx, ac
                sys     4

                push    3
                push    3
                push    matriz1
                call    cargarMat
                add     sp, 3

                ldh     3
                ldl     prompt
                mov     ax, %800
                mov     dx, ac
                sys     4

                push    3
                push    3
                push    matriz2
                call    cargarMat
                add     sp, 3

                push    3
                push    3
                push    matriz1
                call    printMat
                add     sp, 3

                push    3
                push    3
                push    matriz2
                call    printMat
                add     sp, 3

                ; push    1
                ; push    3
                ; push    3
                ; push    matriz2
                ; push    matriz1
                ; call    sumarMat
                ; add     sp, 5
                
                ; push    3
                ; push    3
                ; push    matriz1
                ; call    printMat
                ; add     sp, 3

                push    201
                push    200
                push    matriz3
                push    3
                push    3
                push    matriz2
                push    3
                push    3
                push    matriz1
                call    multMat
                add     sp, 9

                push    [200]
                push    [201]
                push    matriz3
                call    printMat
                add     sp, 3

                stop

; cargarMat -> void cargarMat(int mat[][DIM], int n, int m)
;   push  < cant. columnas >
;   push  < cant. filas >
;   push  < puntero a primera calda de mat >
;   call  cargarMat
;   add   sp, 3
cargarMat:      push    bp
                mov     bp, sp
                push    ax
                push    cx
                push    dx
                push    ex
                push    fx

                mov     ex, 0
cargarI:        cmp     ex, [bp+3]
                jp      finCargarMat
                mov     fx, 0
cargarJ:        cmp     fx, [bp+4]
                jp      fincargarJ
                mov     dx, DIMENSION
                mul     dx, ex
                add     dx, fx
                add     dx, [bp+2]
                mov     cx, 1
                mov     ax, %001
                sys     1
                add     fx, 1
                jmp     cargarJ
fincargarJ:     add     ex, 1
                jmp     cargarI
finCargarMat:   pop     fx
                pop     ex
                pop     dx
                pop     cx
                pop     ax
                mov     sp, bp
                pop     bp
                ret

; printMat -> void printMat(int mat[][DIM], int n, int m)
;   push  < cant. columnas >
;   push  < cant. filas >
;   push  < puntero a primera calda de mat >
;   call  printMat
;   add   sp, 3
printMat:       push    bp
                mov     bp, sp
                push    ax
                push    cx
                push    dx
                push    ex
                push    fx
                push    ac

                mov     ex, 0

printI:         cmp     ex, [bp+3]
                jp      finPrintMat
                mov     fx, 0
printJ:         cmp     fx, [bp+4]
                jp      finPrintJ
                mov     dx, DIMENSION
                mul     dx, ex
                add     dx, fx
                add     dx, [bp+2]
                mov     cx, 1
                mov     ax, %901
                sys     2
                ldh     3
                ldl     espacio
                mov     dx, ac
                mov     ax, %900
                sys     4
                add     fx, 1
                jmp     printJ

finPrintJ:      ldh     3
                ldl     espacio
                mov     dx, ac
                mov     ax, %800
                sys     4
                add     ex, 1
                jmp     printI
finPrintMat:    ldh     3
                ldl     espacio
                mov     dx, ac
                mov     ax, %800
                sys     4
                push    ac
                push    fx
                push    ex
                push    dx
                push    cx
                push    ax
                mov     sp, bp
                pop     bp
                ret

; sumarMat -> void sumarMat(int mat1[][DIM], int mat2[][DIM], int n, int m, int op)
;   push  < op :: 0=sumar, 1=restar >
;   push  < cant. columnas >
;   push  < cant. filas >
;   push  < puntero a primera calda de mat2 >
;   push  < puntero a primera calda de mat1 >
;   call  sumarMat
;   add   sp, 5
;   obs: devuelve en mat1 el resultado de la operacion entre las matrices
sumarMat:       push    bp
                mov     bp, sp
                push    ax
                push    bx
                push    ex
                push    fx

                mov     ex, 0
sumarMatI:      cmp     ex, [bp+4]
                jp      finSumarMat
                mov     fx, 0               
sumarMatJ:      cmp     fx, [bp+5]
                jp      finSumarMatJ
                mov     ax, DIMENSION
                mul     ax, ex
                add     ax, fx
                add     ax, [bp+2]
                mov     bx, DIMENSION
                mul     bx, ex
                add     bx, fx
                add     bx, [bp+3]
                cmp     0, [bp+6]
                jn      restar
                add     [ax], [bx]
                jmp     otroJ
restar:         mov     bx, [bx]
                not     bx
                add     bx, 1
                add     [ax], bx
otroJ:          add     fx, 1
                jmp     sumarMatJ
finSumarMatJ:   add     ex, 1
                jmp     sumarMatI
finSumarMat:    pop     fx
                pop     ex
                pop     bx
                pop     ax
                mov     sp, bp
                pop     bp
                ret

; multiplicable -> int multiplicable(int col1, int filas2)
;   push  < nro de filas de mat2 >
;   push  < nro de columnas de mat1 >
;   call  multiplicable
;   add   sp, 2
;   AX    <- devuelve 0 si no multiplicables, 1 si multiplicables 
multiplicable:  push    bp
                mov     bp, sp

                cmp     [bp+2], [bp+3]
                jnz     nomult
                mov     ax, 1
                jmp     finMulti
nomult:         mov     ax, 0                
finMulti:       mov     sp, bp
                pop     bp
                ret

multMat:        push    bp
                mov     bp, sp
                sub     sp, 2
                push    ax
                push    bx
                push    cx
                push    dx
                push    ex
                push    fx
                push    ac

                sys     15

                push    [bp+6]
                push    [bp+4]
                call    multiplicable
                add     sp, 2

                cmp     ax, 1
                jn      finMultMat

                mov     ax, [bp+9]
                mov     bx, [bp+10]
                mov     [ax], [bp+3]
                mov     [bx], [bp+7]

                mov     ex, 0
multMatIterI:   cmp     ex, [ax]
                jp      finMultMat
                mov     fx, 0
multMatIterJ:   cmp     fx, [bx]
                jp      finMultIterJ

                mov     ac, DIMENSION
                mul     ac, ex
                add     ac, fx
                add     ac, [bp+8]
                mov     [ac], 0

                mov     dx, 0
multMatIterK:   cmp     dx, [bp+4]
                jp      finMultIterK

                mov     [bp-1], DIMENSION
                mul     [bp-1], ex                 
                add     [bp-1], dx                 
                add     [bp-1], [bp+2]                 
                mov     cx, [bp-1]
                mov     [bp-1], [cx]

                mov     [bp-2], DIMENSION
                mul     [bp-2], dx                 
                add     [bp-2], fx                 
                add     [bp-2], [bp+5]                 
                mov     cx, [bp-2]
                mov     [bp-2], [cx]

                mul     [bp-1], [bp-2]
                add     [ac], [bp-1]

                add     dx, 1
                jmp     multMatIterK
finMultIterK:   add     fx, 1
                jmp     multMatIterJ
finMultIterJ:   add     ex, 1
                jmp     multMatIterI

finMultMat:     pop     ac
                pop     fx
                pop     ex
                pop     dx
                pop     cx
                pop     bx
                pop     ax
                add     sp, 2
                mov     sp, bp
                pop     bp
                ret