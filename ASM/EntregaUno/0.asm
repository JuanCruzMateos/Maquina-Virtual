\\ASM DATA=10 EXTRA=10
; escribe str del code segment
prueba EQU "Prueba"
    mov     ax, %000 
    mov     cx, 1000
    ldl     prueba
    ldh     3
    mov     dx, ac
    sys     %4
; [0015]: Prueba
; escribe nro del extra segment
    ldl     1
    ldh     2
    sys   %f
    mov     dx, ac
    mov     [dx],123
    mov     ax, %001 
    mov     cx, 1 
    sys %f
    sys     %2 
; [0032]: 123
    stop
