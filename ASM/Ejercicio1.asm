	xor FX, FX
	mov [2], 0
	mov AX, %001
	mov DX, 1
	mov CX, 1
otro: 	sys 1
	cmp [1], 0
	jn sigue
	add [2], [1]
	add FX, 1
	jmp otro
sigue: 	cmp 0, FX
	jz fin
	div [2], FX
	fin: mov DX, 2
	sys 2
	stop
