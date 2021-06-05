; 6. Interpretar una operación matemática escrita en notación polaca inversa y calcular el resultado.
;     - Notación aritmética: (20 + 3) * 6
;     - La misma operación escrita en notación polaca inversa: 20 3 + 6 *
;   La ventaja de este tipo de notación es que no es necesario utilizar paréntesis y sus operaciones
;   pueden ser resueltas utilizando una pila.
;   Ejemplo: entrada: “20 3 + 6 *” salida: “138”
prompt    equ   "Ingresar operacion en notacion polaca inversa: numeros y signo separados por un espacio"
operacion equ   "Operacion: "
resultado equ   "Resultado: "

                ldh     3
                ldl     prompt
                mov     ax, %800
                mov     dx, ac
                sys     4

                mov     ax, %900
                ldl     operacion
                mov     dx, ac
                sys     4

                mov     dx, 0
                mov     cx, 15
                sys     3

                push    0           ; ptr str operacion
                push    50          ; ptr resultado
                call    polaca
                add     sp, 2

                ldh     3
                ldl     resultado
                mov     ax, %900
                mov     dx, ac
                sys     4

                mov     ax, %800
                mov     dx, 50
                sys     4
                stop

; Especificacion:
;   push    <ptr string operacion>
;   push    <ptr string resultado>
;   call    polaca
;   add     sp, 2
polaca:         push    bp
                mov     bp, sp
                push    ax    ; parseInt devuelve en ax
                push    bx     ; tipo
                push    cx     ; act
                push    ex
                push    fx

                mov     cx, [bp+3]

ciclo:          cmp     [cx], 0
                jz      finPolaca
                mov     bx, [cx]
                cmp     bx, ' '
                jz      sig
                and     bx, %F0
                cmp     bx, %20
                jz      esSigno

                mov     ex, cx      ; guardo el inicio
iter:           cmp     [cx], ' '
                jz      cont
                add     cx, 1
                jmp     iter

cont:           mov     [cx], 0
                push    bx
                call    parseInt
                add     sp, 1
                push    ax
                jmp     sig
esSigno:        pop     ex
                pop     fx

                cmp     [cx], '+'
                jz      sumar
                cmp     [cx], '-'
                jz      restar
                cmp     [cx], '*'
                jz      multiplicar

                div     fx, ex
                jmp     poner
sumar:          add     fx, ex
                jmp     poner
restar:         sub     fx, ex
                jmp     poner
multiplicar:    mul     fx, ex

poner:          push    fx
sig:            add     cx, 1
                jmp     ciclo

finPolaca:      push    [bp+2]
                call    parseStr
                add     sp, 2
                pop     fx
                pop     ex
                pop     cx
                pop     bx
                pop     ax
                mov     sp, bp
                pop     bp
                ret
maskPI    equ    %F  

; inicio <parseInt>
; especificarion
;   push    <ptr a str>
;   call    parseInt
;   add     sp, 1
;   (return en ax)
parseInt:       push    bp
                mov     bp, sp
                push    bx
                push    cx
                push    ex
                push    fx

                xor     ax, ax      ; inicializo ret
                mov     ex, 1       ; registro de signo
                mov     bx, [bp+2]  ;

                cmp     [bx], '-'
                jnz     inicioPI
                mov     ex, -1      ; si neg -> ex = -1
                add     bx, 1       ; salteo el signo
                add     [bp+2], 1   ; salteo el signo

inicioPI:       slen    cx, [bx]
                sub     cx, 1
                add     bx, cx
                mov     fx, 1
iterPI:         cmp     bx, [bp+2]
                jn      finPI
                mov     cx, [bx]
                and     cx, maskPI
                mul     cx, fx
                add     ax, cx
                mul     fx, 10
                sub     bx, 1
                jmp     iterPI
finPI:          mul     ax, ex
                pop     fx
                pop     ex
                pop     cx
                pop     bx
                mov     sp, bp
                pop     bp
                ret 
; fin <parseInt>


; inicio <parseStr>
; Especificacion
;   push    <numero>
;   push    <ptr str>
;   call    parseStr
;   add     sp, 2
maskPS    equ     %30
parseStr:       push    bp
                mov     bp, sp
                push    ax              ; ptr str
                push    bx              ; ptr inicio
                push    cx
                push    ac

                mov     ax, [bp+2]      ; #mem inicio str
                mov     cx, [bp+3]
                cmp     cx, 0
                jp      siguePS
                mov     [ax], '-'
                add     ax, 1
                mul     cx, -1
siguePS:        mov     bx, ax          ; guardo puntero al inicio
nextNumPS:      div     cx, 10
                or      ac, maskPS
                mov     [ax], ac
                add     ax, 1
                cmp     cx, 0
                jz      intercPS
                jmp     nextNumPS

intercPS:       mov     [ax], 0
                sub     ax, 1
iterPS:         cmp     ax, bx
                jn      finPS
                swap    [ax], [bx]
                add     bx, 1
                sub     ax, 1
                jmp     iterPS

finPS:          pop     ac
                pop     cx
                pop     bx
                pop     ax
                mov     sp, bp
                pop     bp
                ret
; fin <parseStr>    