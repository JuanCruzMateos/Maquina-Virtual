\\ASM DATA=208 EXTRA=100
; probando la memoria dinámica
        xor     ex, ex
; NEW 4 celdas -> DS[1]
        mov     cx, 4
        sys     %5
        add     ex, 1
        mov     [ex], dx
; NEW 8 celdas -> DS[2]
        mov     cx, 8
        sys     %5
        add     ex, 1
        mov     [ex], dx
; NEW 10 celdas -> DS[3]
        mov     cx, 10
        sys     %5
        add     ex, 1
        mov     [ex], dx
; NEW 5 celdas -> DS[4]
        mov     cx, 5
        sys     %5
        add     ex, 1
        mov     [ex], dx
; NEW 5 celdas -> DS[5]
        mov     cx, 5
        sys     %5
        add     ex, 1
        mov     [ex], dx
; FREE 10 celdas <- DS[3]
        mov     dx, [3]
        sys     %6
; FREE 4 celdas <- DS[1]
        mov     dx, [1]
        sys     %6
; NEW 6 celdas -> DS[6]
        mov     cx, 6
        sys     %5
        add     ex, 1
        mov     [ex], dx
; FREE 5 celdas <- DS[4]
        mov     dx, [4]
        sys     %6
        add     ex, 1
        mov     [ex], -1 ;marcador
        xor     fx, fx
copy:   cmp     ex, 14
        jnn     fin
        add     fx, 1
        mov     dx, [fx]
        sub     dx, 1
        add     ex, 1
        mov     [ex], [dx]
        jmp     copy
fin:    mov     ax, %8
        mov     cx, 14
        mov     dx, 1
        sys     %2      
        sys     %f
        stop