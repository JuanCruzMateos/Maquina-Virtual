; Utilizando el algoritmo anterior, construya un programa que calcule el factorial de un numero ingresado por teclado.
             mov     ax, %001
             mov     dx, 1
             mov     cx, 1
             sys       1
             mov     [2], 1
             mov     [3], 1
iter:        cmp     [1], [3]
             jn     fin    
             mul     [3], [2]
             add     [2], 1
             jmp     iter
fin:         mov    dx, 3
             sys    2 
             stop

            ; [1] -> 3
            ; [2] -> 1
            ; [3] -> 1 resultado

; ; Leo dos numeros, los guardo en BX y CX
; ; en AX debe guardar su producto		
;  		mov DX, 1
; 		mov CX, 2
; 		mov AX, %001
; 		sys 1	;Leo dos dato y los guardo en posicion 1 y 2 (desde el DS)
; 		mov BX, [1]
; 		mov CX, [2]
; 		xor AX, AX ; reseteo el valor de AX
; 		mov FX, BX ; guardo uno de los dos numeros en FX				
; 		cmp FX, 0 ; verifico si primer valor es negativo
; 		jn siguex ; si es negativo salta a sigue x
; 		jz fin ; si es cero salta a fin
; 		cmp CX, 0 ; verifico si segundo valor es negativo
; 		jn siguey ;si el segundo es negativo salta a sigue y
; 		jz fin ; si es cero salta a fin

; 	; ambos operandos positivos, resultado positivo
; 	sigue:	cmp FX, 0
; 		jz fin		
; 		add AX, CX
; 		sub FX, 1
; 		jmp sigue

; 	siguex:	cmp CX, 0 ; comparo segundo operando
; 		jn sigue3 ; salta si los dos operandos son negativos
; 		jz fin		

; 	; primer operando negativo, segundo operando positivo, resultado negativo
; 	sigue1:	cmp FX, 0 ; 
; 		jz fin
; 		sub AX, CX
; 		add FX, 1
; 		jmp sigue1
; 	; primer operando positivo, segundo operando negativo, resultado negativo
; 	siguey: cmp FX, 0 ; 
; 		jz fin
; 		add AX, CX
; 		sub FX, 1
; 		jmp siguey
; 	; ambos operandos negativos, resultado positivo
; 	sigue3: cmp FX, 0 ; 
; 		jz fin
; 		sub AX, CX
; 		add FX, 1
; 		jmp sigue3
; 	fin:    mov [1], AX
; 		mov DX, 1
; 		mov CX, 1
; 		mov AX, %001
; 		sys 2 		
; 		stop