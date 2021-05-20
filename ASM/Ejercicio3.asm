; se ingresa un valor decimal y se muestra su representacion en binario
; se muestra primero digito mas significativo
		xor BX, BX ;inicializo registro BX
lee: 		mov DX, 1
		mov CX, 1
		mov AX, %001
		sys 1	;Leo un dato
again: 		mov [3], [1]  ; copio la posicion de memoria 1 en la 3
		and [3], %1
		shl BX, 1
		or BX, [3]
		shr [1], 1
		jnz again
		mov DX, 2
		mov CX, 1
		mov AX, %001
muestro:	mov [2], BX
		and [2], %1
		sys 2 ;muestro
		shr BX, 1
		jnz muestro
		stop
