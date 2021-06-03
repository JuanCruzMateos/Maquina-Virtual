;Especificación de invocación 

; ... (completar)
            PUSH    1
            PUSH    30
            PUSH    40
            CALL 	MERGE
            ADD     SP, 3
; ... (completar)

;Otras especificaciones

;Código de la subrutina
MERGE:	    push        bp
            mov         bp, sp
            push        ex
            push        fx

            mov         ex, [bp+4]
            mov         fx, [bp+3]
            strcmp      [ex+2], [fx+2]
            jn          primero             ; el primero es menor
            jz          iguales             ; el segundo es menor
                                            ; son iguales

            push        [bp+2]
            push        [fx+2]
            push        [fx+3]
            push        [fx+4]
            call        ADD
            add         sp, 4
            
cont:       

primero:

iguales: