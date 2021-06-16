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