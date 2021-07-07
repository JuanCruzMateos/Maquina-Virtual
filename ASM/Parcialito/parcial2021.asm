; Especificacion: generarLista
;  - protocolo C:
;  - void list(TArbol *A, TFecha fecha, THora hora, int idUsuario, TUsuarios *usrs, Tlist *list);
;
;   OBS: el ptr a la lista debe inicializarse en null antes de invocar a la subrutina
;   
;   push  < dir. memoria ptr a lista >
;   push  < dir. memoria V con estructura de usuarios >
;   push  < id usuario >
;   push  < hora >
;   push  < fecha >
;   push  < ptr a directorio del sistema >
;   call  generarLista
;   add   sp, 6

; Especificacion: lista
;  N : id 
; N+1: ptr nombre
; N+2: ptr siguente

; Especificacion: hasPermisoEjecucion
;   
;   push  < id usuario >
;   push  < ptr a directorio del sistema >
;   push  < dir. memoria V con estructura de usuarios >
;   call  hasPermisoEjecucion
;   add   sp, 3
;   @return ax == 1, tiene permiso; ax == 0, no tiene permiso

; Especificacion: pertenceGrupo
;   
;   push  < id usuario >
;   push  < id grupo >
;   push  < ptr nodo usuario >
;   call  pertenceGrupo
;   add   sp, 3
;   @return ax == 1, pertence; ax == 0, no pertence

null    equ     -1
true    equ      1
false   equ      0
izq     equ      3
der     equ      4


generarLista:           push    bp
                        mov     bp, sp
                        push    ax
                        push    bx
                        push    cx
                        push    dx
                        push    ex
                        push    fx
                        
                        mov     bx, [bp+2]        ; ptr a nodo de arbol
                        cmp     bx, null
                        jz      finGenerarLista

                        cmp     [bx+3], 0         ; si es un archivo
                        jz      getPermisos       ; veo si tiene permisos

                        ; recorro la estructura
                        mov     ex, bx
                        add     ex, 4            ; & puntero al primer hijo -> ex == X+4
recorrer:               cmp     [ex], null       ; si el dir esta vacio en X+4 habra un -1 (null)
                        jz      finGenerarLista

                        mov     ex, [ex]         ; ex == H
                        push    [bp+7]
                        push    [bp+6]
                        push    [bp+5]
                        push    [bp+4]
                        push    [bp+3]
                        push    [ex]            ; [ex] == puntero a un dir X 
                        call    generarLista
                        add     sp, 6

                        add     ex, 1           ; ex == H+1
                        jmp     recorrer
                    
getPermisos:            push    [bp+5]          ; id usuario
                        push    bx              ; ptr a nodo de arbol (X)
                        push    [bp+6]          ; puntero a estructura de usuarios (V)
                        call    hasPermisoEjecucion
                        add     sp, 3

                        cmp     ax, true
                        jn      finGenerarLista ; no tiene permiso

                        cmp    [bp+3], [bx+9]   ; fecha - fechaMod
                        jp     finGenerarLista

                        cmp    [bp+4], [bx+10]  ; hora - horaMod
                        jp     finGenerarLista

                        mov     fx, [bp+7]      ; & ptr a lista
                        mov     cx, 3           ; resevo tres celdas consecutivas
                        sys     %5
                        mov     [dx], [bx]      ; N : id
                        mov     [dx+1], [bx+2]  ; N+1: ptr nombre
                        mov     [dx+2], [fx]    ; N+2: ptr sig
                        mov     [fx], dx        ; inserto siempre al comienzo de la lista

finGenerarLista:        pop     fx
                        pop     ex
                        pop     dx
                        pop     cx
                        pop     bx
                        pop     ax
                        mov     sp, bp
                        pop     bp
                        ret


hasPermisoEjecucion:    push    bp
                        mov     bp, sp
                        push    bx
                        push    fx
                        push    cx

                        mov     bx, [bp+3]     ; ptr nodo arbol
                        mov     fx, [bx+1]     ; fx == permisos

                        cmp     [bx+6], [bp+4] 
                        jz      esOwner

                        push    [bp+4]        ; id usuario
                        push    [bx+5]        ; id grupo
                        mov     cx, [bp+2]  
                        push    [cx]          ; ptr a nodo usuario
                        call    perteneceGrupo
                        add     sp, 3

                        cmp     ax, true
                        jz      esGrupo

                        and     fx, 1        ; es otro
                        jmp     setAX

esOwner:                shr     fx, 6
                        jmp     setAX

esGrupo:                shr     fx, 3

setAX:                  mov     ax, true
                        and     fx, 1
                        cmp     fx, 1
                        jz      finHasPerEjec
                        mov     ax, false
finHasPerEjec:          pop     cx
                        pop     fx
                        pop     bx
                        mov     sp, bp
                        pop     bp
                        ret


perteneceGrupo:        push     bp
                       mov      bp, sp
                       push     cx
                       push     bx

                       cmp      [bp+2], null
                       jz       noPertenece
                       
                       mov      cx, [bp+2]
                       cmp      [cx], [bp+4]       ; id nodo - id usuario
                       jz       verificar
                       jn       buscarDerecha
                       
                       push     [bp+4]
                       push     [bp+3]
                       push     [cx+izq]
                       call     perteneceGrupo
                       add      sp, 3
                       jmp      finPerteneceGrupo   ; a la vuelta de la recu no hago nada
buscarDerecha:         push     [bp+4]
                       push     [bp+3]
                       push     [cx+der]
                       call     perteneceGrupo
                       add      sp, 3
                       jmp      finPerteneceGrupo   ; idem
verificar:             mov      bx, cx
                       add      bx, 2
                       mov      bx, [bx]
                       add      bx, [bp+3]
                       sub      bx, 1
                       mov      bx, [bx]
                       cmp      bx, 1
                       jnz      noPertenece
pertence:              mov      ax, true
                       jmp      finPerteneceGrupo
noPertenece:           mov      ax, false
finPerteneceGrupo:     pop      bx
                       pop      cx
                       mov      sp, bp
                       pop      bp
                       ret
