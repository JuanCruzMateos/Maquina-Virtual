; 3. Hacer una subrutina que concatene 2 Strings, recibiendo sus direcciones como parámetros.

            mov     ax, %000
            mov     dx, 10
            mov     cx, 20
            sys     %3          ; sys read -> #mem 10 del DS, 20 chars

            mov     dx, 50
            sys     %3          ; sys read -> #mem 50 del DS, 20 chars

            mov     ax, 80      ; #mem del nuevo string concat

            push    10          ; invocacion de subrutina
            push    50
            push    ax
            call    concat
            add     sp, 3

            mov     dx, ax      ; muestro el str concat 
            mov     ax, %000
            sys     %4
            stop


concat:     push    bp
            mov     bp, sp
            push    ax
            push    bx
            push    cx

            mov     ax, [bp+4]
            mov     bx, [bp+3]
            mov     cx, [bp+2]

verif:      cmp     [ax], 0     ; si fin de str -> intercambio
            jz      interc

            mov     [cx], [ax]
            add     ax, 1
            add     cx, 1
            jmp     verif

interc:     swap    ax, bx    
            cmp     [ax], 0     ; si no es 0 recorro hasta llegar a cero, salto a intercambio, da cero, y salto a fin
            jz      fin         ; si el otro es cero salto a fin
            jmp     verif

fin:        mov     [cx], 0     ; agrego char nulo a fin del str concat
            pop     cx
            pop     bx
            pop     ax
            mov     sp, bp
            pop     bp
            ret
