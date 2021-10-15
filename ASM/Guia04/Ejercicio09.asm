; 9. Idem el ejercicio anterior pero el usuario carga una lista de palabras separadas por comas.


; 8. Cargar palabras en un vector hasta que se lea una palabra vacía y ordenarlas alfabéticamente.
;    Nota: considerar palabras de 20 caracteres como máximo.
prompt  equ "Ingrese palabras separadas por comas (enter para terminar): "
vector  equ  100
maxlen  equ  21
; maxlen = 20 + 1 -> para el \0, sino el caracter 20 sera el \0

                ldh     3
                ldl     prompt
                mov     ax, %800
                mov     dx, ac
                sys     4

                mov     dx, 20
                mov     cx, 80
                sys     3
                ; sys     4

                push    99
                push    vector
                push    20
                call    cargarVector
                add     sp, 3

                ; mov     dx, 99
                ; mov     cx, 1
                ; mov     ax, %001
                ; sys     2
ordenar:        cmp     [99], 0
                jz      fin
                mov     fx, [99]
                push    fx
                push    vector
                call    selectionSort
                add     sp, 2

mostrar:        push    fx
                push    vector
                call    printVec
                add     sp, 2
                jmp     fin
fin:            stop

; Especificacion :: cargarVector ::  void cargarVector(char *str, int *vec, int *n)
;   push    < ptr a n -> dim logica >
;   push    < ptr a vec >
;   push    < ptr a str >
;   call    cargarVector
;   add     sp, 3
cargarVector:   push    bp
                mov     bp, sp
                push    ax
                push    bx
                push    cx
                push    dx
                push    ex
                push    fx
                push    ac

                sys     %f
                mov     ax, [bp+3]      ; vec
                mov     bx, [bp+4]      ; *n
                mov     ex, [bp+2]      ; *str
                
                slen    ac, [ex]
                add     ac, 1
                add     ac, ex
                xor     [bx], [bx]
                mov     fx, ex
ciclo:          cmp     ex, ac
                jp      finCargarVec
iter:           cmp     [fx], 0
                jz      finstr
                cmp     [fx], 44    ; 44 == ','
                jz      comma
                add     fx, 1
                jmp     iter

comma:          mov     [fx], 0
finstr:         mov     cx, maxlen
                add     cx, 1
                sys     5
                smov    [dx], [ex]
                mov     [ax], dx
                add     ax, 1
                add     [bx], 1
                add     fx, 1
                mov     ex, fx
                jmp     ciclo
finCargarVec:   pop     ac
                pop     fx
                pop     ex
                pop     dx
                pop     cx
                pop     bx
                pop     ax
                mov     sp, bp
                pop     bp
                ret


; Especificacion :: selectionSort ::  void selection_sort(char **vec, int n)
;   push    < n: dimension logica del vector >
;   push    < ptr a vec >
;   call    selectionSort
;   add     sp, 2

varMin   equ   1  
varI     equ   2  
varJ     equ   3  
selectionSort:  push    bp
                mov     bp, sp
                sub     sp, 3
                push    ax
                push    bx
                push    ex
                push    fx

                sys     %f

                mov     ax, [bp+3]              ; ax = n
                sub     ax, 1                   ; ax = n - 1
                xor     [bp-varI], [bp-varI]    ; [bp-2] == i = 0

iterI:          cmp     [bp-varI], ax           ; i < n - 1
                jp      finSort
                mov     bx, [bp+2]              ; bx = vec
                add     bx, [bp-varI]           ; bx = vec + i
                mov     [bp-varMin], [bp-varI]  ; min = i

                mov     [bp-varJ], [bp-varI]    ; j = i
iterJ:          add     [bp-varJ], 1            ; j = i + 1
                cmp     [bp-varJ], [bp+3]       ; j < n           
                jp      finIterJ
                mov     ex, [bp+2]              ; ex = vec
                add     ex, [bp-varJ]           ; ex = vec + j
                mov     ex, [ex]                ; ex = *(vec + j)
                mov     fx, [bp+2]              ; fx = vec
                add     fx, [bp-varMin]         ; fx = vec + min
                mov     fx, [fx]                ; fx = *(vec + min)
                scmp    [ex], [fx]              ; 
                jp      iterJ
                mov     [bp-varMin], [bp-varJ]  ; min = j
                jmp     iterJ
finIterJ:       cmp     [bp-varMin], [bp-varI]  ; min != i
                jz      nextI
                mov     fx, [bp+2]              ; fx = vec
                add     fx, [bp-varMin]         ; fx = vec + min
                swap    [bx], [fx]              ; *(vec + i), *(vec + min) = *(vec + min), *(vec + i)
nextI:          add     [bp-varI], 1            ; i = i + 1
                ;push    [bp+3]
                ;push    [bp+2]
                ;call    printVec
                ;add     sp, 2
                jmp     iterI                       

finSort:        pop     fx
                pop     ex
                pop     bx
                pop     ax
                add     sp, 3
                mov     sp, bp
                pop     bp
                ret

; Especificacion :: printVec ::  void printVec(char *+vec, int n)
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
