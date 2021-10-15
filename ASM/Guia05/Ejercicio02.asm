; 2. Los siguientes algoritmos. Transcribirlos en ASM de la MV e indicar cómo quedaría la pila al 
;    final de su ejecución.

; a.
; void main() {
;   int j=3, arreglo[5];
;   arreglo[0] = 1020;
;   arreglo[1] = arreglo[0] | 0x3FF;
;   calculo( j, arreglo);
; }
; void calculo( int x, int vec[]){
;   vec[x] = vec[x-1] & vec[0];
; }
; Nota: el arreglo arreglo[] se ubica en la celda 201 del
; DS, y sucesivas. Use “ARREGLO EQU 201”

; ARREGLO     equ   201
; DIMENSION   equ   5
; main:           mov     [1000], 3
;                 ; mov     [ax+ARREGLO], [ARREGLO]     ; [ARREGLO] -> error porque 201 > 127 
;                 mov     ax, ARREGLO
;                 mov     [ax], 1020
;                 mov     [ax+1], [ax]
;                 or      [ax+1], %3ff
;                 mov     [ax+2], 8
;                 ; invocacion
;                 push    ARREGLO
;                 push    [1000]
;                 call    calculo
;                 add     sp, 2

;                 ; mostrar
;                 mov     dx, ARREGLO
;                 mov     cx, 4
;                 mov     ax, %001
;                 sys     2
;                 stop
                

; calculo:        push    bp
;                 mov     bp, sp
;                 push    ax
;                 push    bx

;                 mov     ax, [bp+3]      ; ax = vec
;                 mov     bx, ax          ; bx = vec

;                 add     ax, [bp+2]
;                 mov     [ax], [ax-1]
;                 and     [ax], [bx]

;                 pop     bx
;                 pop     ax
;                 mov     sp, bp
;                 pop     bp
;                 ret


; b.
; void main() {
;   int vec[9], i=2;
;   vec[i] = 501;
;   vec[0] = 0xF0F0 & vec[i];
;   proce( i, vec );
; }
; void proce( int x, int w[]){
;   w[x-1] = w[x] | w[0];
; }
; Nota: el arreglo vec[] se ubica en la celda 501 del DS,
; y sucesivas

ARREGLO   equ	501
DIMENSION equ   9

main:		mov	    [1000], 2
		    mov	    ax, [1000]
		    add	    ax, ARREGLO
		    mov	    [ax], 501
		    mov	    bx, ARREGLO
		    mov	    [bx], [ax]
		    and 	[bx], %F0F0
    
		    push	ARREGLO
		    push	[1000]
		    call	proce
		    add	    sp, 2

            ; mostrar
            mov     dx, ARREGLO
            mov     cx, 4
            mov     ax, %001
            sys     2
		    stop

proce:		push	bp
		    mov	    bp, sp
		    push	ax
		    push	bx

		    mov	    ax, [bp+3]
		    mov	    bx, ax
		    add	    ax, [bp+2]
    
		    mov	    [ax-1], [ax]
		    or	    [ax-1], [bx]
    
		    pop	    bx
		    pop	    ax
		    mov	    sp, bp
		    pop	    bp
		    ret