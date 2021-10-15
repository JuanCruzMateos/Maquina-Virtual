; Dado un árbol n-ario que representa la estructura organizacional de una empresa.
; Generar una lista dinámica con los nombres de los empleados a un mismo nivel. 
; Escribir la subrutina GENLISTA que reciba como parámetros: (1) puntero a la raiz del arbol, (2) nivel que
; debe copiar y (3) la dirección de memoria que tiene el puntero al primer nodo de la lista (inicialmente en
; null). La subrutina debe recorrer el árbol in-order hasta el nivel indicado y crear la lista solo con los
; punteros a nombres de las personas a dicho nivel.
null    equ     -1

                mov     ax, 500
                mov     [ax], null

                push    100
                push    2
                push    ax
                call    genlista
                add     sp, 3
                
; push  < ptr a raiz >
; push  < nivel >
; push  < & ptr a lista >
; call genLista
; add  sp, 3
genlista:       push    bp
                mov     bp, sp

                push    [bp+4]      ; ptr raiz
                push    [bp+3]      ; nivel buscado
                push    1           ; nivel act
                push    [bp+2]      ; & ptr lista
                call    genera
                add     sp, 4

                mov     sp, bp
                pop     bp
                ret

; push  < ptr a raiz >
; push  < nivel buscado >
; push  < nivel actual >
; push  < & ptr a lista >
; call genera
; add  sp, 4
genera:         push    bp
                mov     bp, sp

                mov     bx, [bp+5]
                mov     ax, [bp+3]

                cmp     [bp+4], ax
                jz      agregar
                jn      finGenera

                add     ax, 1       ; nivel_act++
                add     bx, 3       ; apunto al 1° hijo

recorrer:       cmp     [bx], null
                jz      finGenera

                push    [bx]
                push    [bp+4]
                push    ax
                push    [bp+2]
                call    genera
                add     sp, 4

                add     bx, 1
                jmp     recorrer

agregar:        mov     cx, 2
                sys     %5
                mov     ex, [bp+2]     ; nuevo a ex &ptr lista
                mov     [dx], [fx+2]   ; asigno ptr a nombre
                mov     [dx+1], [ex]   ; act->sig = *L
                mov     [ex], dx       ; *L = act

finGenera:      pop     
                mov     sp, bp
                pop     bp
                ret

null EQU -1
; PUSH 500 (*a *nodo)
; PUSH 2
; PUSH 100 (*raiz)
; CALL GENLISTA
; ADD SP, 3
GENLISTA:   push    BP
            mov     BP, SP

            push    [BP+4]
            push    [BP+3]
            push    1
            push    [BP+2]
            call    GENERA
            add     sp, 4
            mov     sp, bp
            pop     BP
            ret


; subrut recursiva:
; PUSH <[^nodo list]>
; PUSH <nivel buscado>
; PUSH <nivel actual>
; PUSH <^nodo árbol>
; CALL GENERA
; ADD SP, 3
; POP <[^nodo list]>
GENERA:     push    bp
            mov     bp, sp
            push    ax
            push    bx
            push    cx
            push    dx
            mov     bx, [bp+2]
            mov     cx, [bp+3]
            cmp     [bp+4],cx
            jz      AGREGA
            jn      FIN
            add     bx, 3
            add     cx, 1
            jmp     RECORRE
FIN:        pop     dx
            pop     cx
            pop     bx
            pop     ax
            mov     sp, bp
            pop     bp
            ret
RECORRE:    cmp     [bx], null
            jz      FIN
            push    [bp+5]
            push    [bp+4]
            push    cx
            push    [bx]
            call    GENERA
            add     sp, 3
            pop     [bp+5]
            add     bx, 1
            jmp     RECORRE
AGREGA:     push    2
            call    NEW
            add     sp, 1
            mov     dx,[bp+5]   ; mueve a dx, &ptr lista
            mov     [dx],ax     ; asigna la dir del nuevo nodo
            mov     [ax],[bx+2] ; asigna al nuevo nodo el ptr al nombre
            add     ax, 1       ; ax apunta al &sig
            mov     [ax], null  ; sig = null
            mov     [bp+5], ax  ; avanzo en la lista
            jmp     FIN
                