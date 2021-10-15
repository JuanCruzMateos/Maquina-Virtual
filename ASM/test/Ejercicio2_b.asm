	xor BX, BX
lee: 	mov DX, 1
	mov CX, 1
	mov AX, %001
	sys 1
	cmp 1, [1]
	jz sigue
	cmp 0, [1]
	jnz fin
sigue: 	shl BX, 1
	add BX, [1]
	jmp lee
fin: 	mov [2], BX
	mov AX, %001
	mov CX, 1
	mov DX, 2
	sys 2
	stop