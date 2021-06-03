; \\ASM DATA=4 EXTRA=10 STACK=70
titulo equ  "--- TORRE DE HANOI ---"
msg    equ  "Ingrese la cantidad de discos:"
strA  equ "Torre A" 
strB  equ "Torre B"
strC  equ "Torre C"
torreA equ 0
torreB equ 1
torreC equ 2
strAUX    equ 1    
; ubicación de los str 
step   equ 4
        mov     [step], 0
        ldh     2
        ldl     50
        mov     dx, ac
        ldh     3
        ldl     msg
        smov    [dx], [ac]
        ldh     2
        ldl     10
        mov     dx, ac
        ldh     3
        ldl     titulo
        smov    [dx], [ac]
        mov     ax, %800
        sys     %4
        add     dx, 40
        mov     ax, %900
        sys     %4
        mov     dx, 0 
        mov     cx, 1
        mov     ax, %801
        sys     %1
        mov     cx, strAUX
        ; Ubico los nombres de las torres
        ldh     3
        ldl     strA 
        mov     [cx+torreA], ac
        ldl     strB 
        mov     [cx+torreB], ac
        ldl     strC 
        mov     [cx+torreC], ac

        push    torreC ;torre auxiliar (aux)
        push    torreB ;torre destino  (dtn)
        push    torreA ;torre origen   (org)
        push    [0] ;cantidad de discos a mover
        call    hanoi
        add     sp, 5
        stop
; Hanoi
; parametros: (1) Cant discos, (2) origen, (3) destino, (4) aux
discos equ 2
origen equ 3
destino equ 4
aux equ 5
hanoi:  push    bp
        mov     bp, sp
        push    cx      
        
        mov     cx, [BP+discos]
        cmp     cx, 0   ;si la cantidad a mover es 0...
        jz      finh    ;...no hace nada 
        sub     cx, 1
        
        push    [BP+destino] ;destion pasa a auxiliar
        push    [BP+aux] ;auxiliar es el nuevo destino
        push    [BP+origen] ;el origen se mantiene 
        push    cx
        call    hanoi
        add     sp, 4
        
        push   [BP+destino]  ;destino 
        push   [BP+origen]  ;origen 
        call   print
        add    sp, 2 
        
        push    [BP+origen] ;el origen pasa a auxiliar
        push    [BP+destino] ;el destino se mantiene
        push    [BP+aux] ;el auxiliar pasa a ser origen 
        push    cx
        call    hanoi
        add     sp, 4
        jmp     finh
         
finh:   pop     cx
        mov     sp, bp    
        pop     bp
        ret

desde  equ  2
hasta  equ  3 
flecha equ " -> "
paso equ "PASO "
print:  push    bp
        mov     bp, sp
        push    ax
        push    bx
        push    cx
        push    dx
        ldh     3
        ldl     paso
        mov     dx, ac
        mov     bx, 1
        mov     ax, %900
        sys     %4
        mov     dx, step
        add     [dx], 1     
        mov     cx, 1
        mov     ax, %901
        sys     %2
        mov     dx, [bp+desde]
        add     dx, strAUX
        mov     dx, [dx]
        mov     ax, %900
        mov     bx, 1
        sys     %4
        ldh     3
        ldl     flecha
        mov     dx, ac
        mov     ax, %900
        mov     bx, 1
        sys     %4
        mov     dx, [bp+hasta]
        add     dx, strAUX
        mov     dx, [dx]
        mov     ax, %800
        mov     bx, 1
        sys     %4        

finp:   pop     dx
        pop     cx
        pop     bx
        pop     ax
        mov     sp, bp    
        pop     bp
        ret
          