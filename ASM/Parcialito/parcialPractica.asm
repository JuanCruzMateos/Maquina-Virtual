; Especificación :: generarLista
; OBS: el ptr a la lista debe inicializarse en null antes de la invocación a la subrutina
; push <dir. memoria de ptr a lista>
; push <dir. memoriar V con estructura de usuarios>
; push <id. usuario>
; push <hora>
; push <fecha>
; push <ptr. a directorio del sistema>
; call generarLista
; add sp, 6

; Especificación :: hasPersmisoEjecucion
; push <id. usuario>
; push <ptr. a nodo directorio>
; push <dir. memoriar V con estructura de usuarios>
; call hasPermisoEjecucio
; add sp, 3
; @return AX <- 0, sin permiso; 1, con permiso

; Especificacion :: pertenceGrupo
; push <id. usuario>
; push <id. grupo>
; push <ptr a nodo usuario>
; call perteneceGrupo
; add sp, 3
; @return AX <- 0 no pertenece, 1 pertenece
true equ 1
false equ 0
null equ -1
izq equ 3
der equ 4
generarLista:       push bp
                    mov bp, sp
                    push ax
                    push bx
                    push cx
                    push dx
                    push ex
                    push fx
                    mov bx, [bp+2] ; ptr a nodo arbol
                    cmp bx, null
                    jz finGenerarLista
                    cmp [bx+3], 0 ; si es un archivo
                    jz getPermisos ; veo si tiene los permisos
                    ; recorro
                    mov ex, bx
                    add ex, 4 ; & puntero al primer hijo
recorrer:           cmp [ex], null
                    jz finGenerarLista
                    mov ex, [ex]
                    push [bp+7]
                    push [bp+6]
                    push [bp+5]
                    push [bp+4]
                    push [bp+3]
                    push [ex]
                    call generarLista
                    add sp, 6
                    add ex, 1
                    jmp recorrer
getPermisos:        push [bp+5]
                    push bx
                    push [bp+6]
                    call hasPermisoEjecucion
                    add sp, 3
                    cmp ax, true
                    jn finGenerarLista
                    cmp [bp+3], [bx+9] ; fecha-fechaMod
                    jp finGenerarLista
                    cmp [bp+4], [bx+10] ; hora-horaMod
                    jp finGenerarLista
                    mov fx, [bp+7] ; & ptr a lista
                    mov cx, 3
                    sys %5 ; new
                    mov [dx], [bx]
                    mov [dx+1], [bx+2]
                    mov [dx+2], [fx]
                    mov [fx], dx
finGenerarLista:    pop fx
                    pop ex
                    pop dx
                    pop cx
                    pop bx
                    pop ax
                    mov sp, bp
                    pop bp
                    ret
hasPermisoEjecucion: push bp
                    mov bp, sp
                    push bx
                    push fx
                    push cx
                    mov bx, [bp+3]
                    mov fx, [bx+1] ; permisos
                    cmp [bx+6], [bp+4]
                    jz esowner
                    push [bp+4]
                    push [bx+5]
                    mov cx, [bp+2]
                    push [cx]
                    call pertenceGrupo
                    add sp, 3
                    cmp ax, true
                    jz esgrupo
                    jmp finComp
esowner:            shr fx, 6
                    jmp finComp
esgrupo:            shr fx, 3
                    jmp finComp
finComp:            mov ax, true
                    and fx, 1
                    cmp fx, 1
                    jz finHasPermiso
                    mov ax, false
finHasPerm:         pop cx
                    pop fx
                    pop bx
                    mov sp, bp
                    pop bp
                    ret
pertenceGrupo:      push bp
                    mov bp, sp
                    push cx
                    push bx
                    mov bx, [bp+2]
                    cmp bx, null
                    jz finNull
                    cmp [bx], [bp+4]
                    jz verificar
                    ; hubiese sido magistral si aprovechaba que es un ABB!!
                    ; yendo por derecha o izquierda según el idUsuario buscado
                    push [bp+4]
                    push [bp+3]
                    push [bx+izq]
                    call perteneceGrupo
                    add sp, 3
                    cmp ax, true
                    jz finPertenceGrupo
                    push [bp+4]
                    push [bp+3]
                    push [bx+der]
                    call perteneceGrupo
                    add sp, 3
                    jmp finPertenceGrupo
verificar:          mov cx, bx
                    add cx, 2
                    mov cx, [cx]
                    add cx, [bp+3]
                    mov cx, [cx]    ; <<< [cx-1] (el ajuste de id)
                    cmp cx, false
                    jz finNull
; El grupo podría no existir y tener -1
                    mov ax, true
                    jmp finPertenceGrupo
finNull:            mov ax, false
finPerteneceGrupo:  pop bx
                    pop cx
                    mov sp, bp
                    pop bp
                    ret