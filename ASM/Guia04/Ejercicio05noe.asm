; Interpretar una operación matemática con 2 operandos escrita en un string y que se muestre el
; resultado por pantalla.
                mov ax, 0
                mov dx, 10
                mov cx, 20
                sys 3

                push 10 ;inicio
                push 50 ;direccion resultado
                call split
                add sp, 2
                mov ax, %1
                mov dx, 50
                mov cx, 1
                sys 2
                stop

split:          push bp
                mov bp, sp
                sub sp, 3       ; uso 3 variables locales 
                push ax
                push bx
                push cx
                mov cx, 0
                mov ax, [bp+3]

loop1:          cmp [ax], ' ' 
                jz operador
                mov bx, [ax]
                and bx, %F
                mul cx, 10
                add cx, bx 
                add ax, 1
                jmp loop1 
        

operador:       mov [bp-1], cx
                add ax, 1
                mov [bp-2], [ax]
                add ax, 1       ; espacio en blanco
                add ax, 1

                mov cx, 0     
loop2:          cmp [ax], 0
                jz opera
                mov bx,[ax]     
                and bx, %F
                mul cx, 10
                add cx, bx
                add ax, 1
                jmp loop2    
        
opera:          mov [bp-3], cx
                mov ax, [bp-1]
                mov bx, [bp-3]
                cmp [bp-2], '+'
                jz suma
                cmp [bp-2], '-'     
                jz resta
                cmp [bp-2], '*'
                jz prod
                div ax, bx
                jmp fin
suma:           add ax, bx
                jmp fin
resta:          sub ax, bx
                jmp fin
prod:           mul ax, bx

fin:            mov bx, [bp+2]
                mov [bx], ax
                pop cx
                pop bx
                pop ax 
                add sp, 3
                mov sp, bp
                pop bp
                ret
