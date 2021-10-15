;Especificación de invocación 
            push        <N Dir del ptr lista 2>
            push        <N Dir del ptr lista 1>
            push        <N Dir del ptr lista vacia>
            call        merge
            add         sp, 3
            
;Especificación de invocación 
            push        <Fecha>
            push        <ptr String DNI>
            push        <ptr String Nombre>
            push        <N Dir del ptr lista vacia>
            call        ADD
            add         sp, 4

            AX <- <ptr a nodo de lista>


;Otras especificaciones
null        equ         -1
fecha       equ         4
nombre      equ         3
dni         equ         2
sig         equ         1

;Código de la subrutina
MERGE:	    push        bp
            mov         bp, sp
            push        ax
            push        bx
            push        dx
            push        ex
            push        fx

            mov         ax, [bp+2]
            mov         bx, [bp+3]
            mov         dx, [bp+4]

inicio:     cmp         [bx], null
            jz          cambio
            cmp         [dx], null
            jz          agregarB        ; la primera es null -> continuo hasta que termine
            jmp         compara

cambio:     swap        bx, dx
            cmp         [bx], null
            jz          fin
            jmp         agregarB

compara:    mov         ex, [bx]
            mov         fx, [dx]
            mov         ex, [ex+nombre]
            mov         fx, [fx+nombre]
            scmp        [ex], [fx]
            jz          iguales
            jn          agregarB
            swap        bx, dx
            jmp         agregarB

iguales:    mov         dx, [dx]
            add         dx, sig
            jmp         agregarB

agregarB:   mov         bx, [bx]
            push        [bx+fecha]
            push        [bx+nombre]
            push        [bx+dni]
            push        ax
            call        ADD
            add         sp, 4
            jmp         inicio


fin:        pop         fx
            pop         ex
            pop         dx
            pop         bx
            pop         ax
            mov         sp, bp
            pop         bp
            ret