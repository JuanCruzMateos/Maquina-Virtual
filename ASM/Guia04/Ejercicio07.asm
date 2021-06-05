; 7. Solicitar una frase ingresada por el usuario y determinar si es palindroma, sin contar espacios,
;    signos de puntuacion o acentos. Por ejemplo: si ingresa ”La ruta nos aporto otro paso natural.” se
;    debe escribir “Es frase palindroma”.

;       TODO -> no ignora acentos
toUpper       equ  %DF
prompt  equ "Ingrese una frase:"
palindroma equ "Es frase palindroma"
nopalibdroma equ "No es frase palindroma"

                mov     ax, %800  ; omite prompt
                ldh     3
                ldl     prompt
                mov     dx, ac
                sys     4         ; escribo indicacion

                mov     dx, 0
                mov     cx, 100
                sys     %3        ; leo frase

                push    0         ; ptr String
                call    esPal
                add     sp, 1

                ldh     3
                cmp     ax, 1
                jn      nopal
                ldl     palindroma
                jmp     most
nopal:          ldl     nopalibdroma
most:           mov     dx, ac
                mov     ax, %800
                sys     4
                stop

; falta ignorar acentos
esPal:          push    bp
                mov     bp, sp
                push    ex
                push    fx
                push    cx
                push    dx

                sys     15
                mov     ax, 1        ; inicializo en 1
                mov     ex, [bp+2]   ; ptr inicio str
                slen    fx, [ex]     ; calculo longitud
                sub     fx, 1        ; menos 1
                add     fx, ex       ; ubico puntero al final

iter:           cmp     fx, ex
                jn      fin

                mov     cx, [ex]    ; no modifico el String original
                mov     dx, [fx]

                cmp     cx, %41
                jn      salto1
                cmp     dx, %41
                jn      salto2

                and     cx, toUpper
                and     dx, toUpper
                cmp     cx, dx
                jnz     notPal
                add     ex, 1
                sub     fx, 1
                jmp     iter

salto1:         add     ex, 1
                jmp     iter
salto2:         sub     fx, 1
                jmp     iter

notPal:         mov     ax, 0
fin:            pop     dx
                pop     cx
                pop     fx
                pop     ex
                mov     sp, bp
                pop     bp
                ret
