;DIRECCIONAMIENTO INDIRECTO
\\ASM DATA=60 EXTRA=50 STACK=50
ant		EQU 	-1
sig		EQU		1
;inicializo los registros
		mov 	ax, 1
		mov		bx, 5
        mov     cx, 2        
;relleno la memoria de 1 a 10
otro:	cmp		ax, 7
		jz		sigue
		mov		[ax], ax
		add		ax, 1
		jmp 	otro
;recupero registros y multiplico x10
sigue:	mul 	[CX+ant],10
		mul 	[Cx],10
		mul 	[cx+sig],10
		mul 	[bx-1],10
		mul 	[Bx],10
		mul 	[BX-ant],10
        mov     [ax],[cx-sig]
		push 	[ax]
		mov 	BP, SP
		add		[BP], 1
		LDL		1
		LDH		2
		mov		dx, ac
		LDH		0
		mov		[dx], [ac]
		mov		[dx+sig], [ac+sig]
		mov		[dx+2], [ac+2]
;imprime todo 
		mov 	ax, %1        
		mov		cx, 3
		sys 	%2		
		mov		dx, 1
		mov		cx, 7
		sys 	%2		
		mov 	dx, bp
		mov		cx, 1
		sys 	%2
		sys		%F
		stop
		stop
		stop
		stop
		stop