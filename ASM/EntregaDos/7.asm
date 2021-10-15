\\ASM EXTRA=20
;Inicializa HEAP (manual)
        ldh     2
        ldl     0
        mov     [ac], ac
        add     [ac], 1
; Escribe mensaje al usuario
msg     equ     "Escriba palabras seguidas de ENTER (en blanco para terminar)"
        ldh     3
        ldl     msg
        mov     ax, %800
        mov     cx, 1000  
        mov     dx, ac
        sys     %4
        mov     cx, 1 
; Lee una palabra en DS[0]
        ldh     2
        ldl     0
ini:    mov     ax, %900
        mov     cx, 1000
        mov     dx, 0
        sys     %3
        slen    cx, [0]
        cmp     cx, 0           ;Si está vacía... 
        jz      finlee          ;...termina la lectura 
        mov     dx, [ac]        ;Sino, muevo el HEAP a DX        
        add     [ac], cx      ;Incremento el HEAP para reservar la memoria 
        add     [ac], 1       ;Uno más por el \0
        smov    [dx], [0]       ;Agrega la palabra en el ES 
        jmp     ini
finlee: ldl     1
        mov dx, ac              ;Inicializa 1 para comenzar el recorrido 
        ldl     0
        mov cx, [ac]          
        sub cx, 1               ;Posiciona a cx en el último \0
next:   cmp dx, cx              ;Termina cuando DX llega al último \0
        jz  fin
may:    and [dx], %DF        ;Pasa a MAYUSCULAS
        cmp [dx], 0          ;Si encuentra un \0 ...
        jnz sig         
        mov [dx], %20        ;... pone un espacio para contactenar 
sig:    add dx, 1               
        jmp next
; Muestra cadena concatenada y pasada a mayusculas
fin:    SYS %F ;mostrar el ES
        ldl     1
        ldh     2
        mov     dx, ac
        mov     ax, %900
        sys     %4 
        stop 


        



