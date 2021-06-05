; 5. Interpretar una operacion matematica con 2 operandos escrita en un string y que se muestre el
;    resultado por pantalla.

;    Ejemplo: entrada: “45 + 20” se muestra por pantalla: “65”
;    Los operadores admitidos son: suma (+), resta (-), multiplicacion (*) y division(/)

prompt   equ    "Ingresar operacion: numeros y signo separados por un espacio"
maskPI    equ     %F  
maskPS    equ     %30

                ldh     3
                ldl     prompt
                mov     ax, %800    ; omitir prompt
                mov     dx, ac
                sys     %4
                mov     ax, %000    ; escribir prompt
                mov     dx, 0
                mov     cx, 15
                sys     %3          ; leo el str

                push    0           ; ptr string cuenta
                push    50          ; ptr string resultado
                call    calc
                add     sp, 2

                mov     dx, 50
                sys     %4
                stop

; inicio <calc>
; Especificacion
;   push    <ptr str cuenta>
;   push    <ptr str resultado>
;   call    calc
;   add     sp, 2
;   (return en ax)
calc:           push    bp
                mov     bp, sp
                sub     sp, 1       ; aux1 = n1 -> var automaticas
                push    ax          ; uso subrutinas que usan ax
                push    cx          ; recorrer
                push    bx          ; inicio de str
                push    fx          ; operacion
                push    ex          ; #mem del nuevo str

                mov     cx, [bp+3]
                mov     bx, cx      ; guardo el inicio

nxt:            add     cx, 1
                cmp     [cx], ' '
                jz      primNum
                jmp     nxt

primNum:        mov     [cx], 0
                push    bx
                call    parseInt
                add     sp, 1
                mov     [bp-1], ax  ; aux1 = int(valor1)

                add     cx, 1
                mov     fx, [cx]    ; guardo operando en fx
                add     cx, 1

                push    cx
                call    parseInt
                add     sp, 1

                cmp     fx, '+'
                jz      sumar
                cmp     fx, '-'
                jz      restar
                cmp     fx, '*'
                jz      multiplicar

                div     [bp-1], ax
                jmp     convertirStr
sumar:          add     [bp-1], ax
                jmp     convertirStr
restar:         sub     [bp-1], ax
                jmp     convertirStr
multiplicar:    mul     [bp-1], ax
                jmp     convertirStr

convertirStr:   push    [bp-1]
                push    [bp+2]
                call    parseStr
                add     sp, 2    

finCalc:        pop     ex
                pop     fx
                pop     bx
                pop     cx
                pop     ax
                add     sp, 1
                mov     sp, bp
                pop     bp
                ret

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