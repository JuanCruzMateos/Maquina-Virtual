; 7. Crear una estructura de árbol binario que permita cargar ordenados números ingresados por el
;    usuarios (indicando el último número con un número negativo). Armar subrutinas para:
; a. Agregar un nodo al árbol.
; b. Mostrar el árbol inorden.
; c. Mostrar el árbol preorden.
; d. Mostrar el árbol postorden.
; e. Solicitar un número e informar si el mismo se encuentra o no en el árbol.
; f. Crear una subrutina que indique la cantidad de niveles que tiene el árbol.

true    equ     1
false   equ     0
izq     equ     1
der     equ     2
null    equ    -1

main:               mov     fx, 100     ; ptr root
                    mov     [fx], null
otro:               mov     dx, 0
                    mov     cx, 1
                    mov     ax, %001
                    sys     1
                    cmp     [0], 0
                    jn      mostrar
                    push    [0]
                    push    fx
                    call    insertarNodo
                    add     sp, 2
                    jmp     otro

mostrar:            mov     ax, %810    ; para bajar de linea
                    mov     [0], ' '
                    mov     dx, 0
                    mov     cx, 1

                    push    [fx]
                    call    preorden
                    add     sp, 1
                    sys     2

                    push    [fx]
                    call    inorden
                    add     sp, 1
                    sys     2

                    push    [fx]
                    call    postorden
                    add     sp, 1
                    sys     2

                    mov     ax, %001
                    mov     dx, 0
                    mov     cx, 1
                    sys     1

busco:              push    [0]
                    push    [fx]
                    call    esta
                    add     sp, 2
                    mov     [1], ax
                    mov     ax, %001
                    mov     dx, 1
                    sys     2
nivel:              push    [fx]
                    call    cantNiveles
                    add     sp, 1
                    mov     [0], ax
                    mov     ax, %001
                    mov     dx, 0
                    mov     cx, 1
                    sys     2
                    stop


; Especificaion :: insertarNodo -> void insertarNodo(TArbol *A, int valor)   
;   ---  ABB  ---
;   push    < valor a agregar en el arbol >
;   push    < direccion de ptro a raiz > // &Arbol
;   call    insertarNodo
;   add     sp, 2
insertarNodo:       push    bp
                    mov     bp, sp
                    push    ax
                    push    bx
                    push    cx
                    push    dx

                    mov     ax, [bp+2]      ; ax == ptr a nodo
                    cmp     [ax], null  
                    jnz     comparar

                    mov     cx, 3
                    sys     5
                    mov     [dx], [bp+3]
                    mov     [dx+izq], null
                    mov     [dx+der], null
                    mov     [ax], dx
                    jmp     finAgregarNodo
comparar:           mov     ax, [ax]        ; importante !!! 
                    mov     bx, ax
                    cmp     [bp+3], [ax]    ; valor < root->act
                    jp      irDerecha
                    add     bx, izq
                    jmp     recurivo
irDerecha:          add     bx, der
recurivo:           push    [bp+3]
                    push    bx
                    call    insertarNodo
                    add     sp, 3
finAgregarNodo:     pop     dx
                    pop     cx
                    pop     bx
                    pop     ax
                    mov     sp, bp
                    pop     bp
                    ret

; fin :: insertarNodo


; Especificaion :: inorden -> void inorden(TArbol A)   
;   push    < ptro a raiz >
;   call    inorden
;   add     sp, 1
inorden:            push    bp
                    mov     bp, sp
                    push    ax
                    push    bx
                    push    cx
                    push    dx
                    push    fx

                    mov     fx, [bp+2]
                    cmp     fx, null
                    jz      finInorden

                    push    [fx+izq]
                    call    inorden
                    add     sp, 1

                    mov     ax, %901
                    mov     dx, fx
                    mov     cx, 1
                    sys     2

                    mov     cx, 1       ; pido memoria para imprimir un ' ' entre los numeros
                    sys     5
                    mov     [dx], ' '
                    mov     ax, %910
                    mov     cx, 1
                    sys     2
                    sys     6

                    push    [fx+der]
                    call    inorden
                    add     sp, 1
finInorden:         pop     fx
                    pop     dx
                    pop     cx
                    pop     bx
                    pop     ax
                    mov     sp, bp
                    pop     bp
                    ret
; fin :: inorden


; Especificaion :: preorden -> void preorden(TArbol A)   
;   push    < ptro a raiz >
;   call    preorden
;   add     sp, 1
preorden:           push    bp
                    mov     bp, sp
                    push    ax
                    push    bx
                    push    cx
                    push    dx
                    push    fx

                    mov     fx, [bp+2]
                    cmp     fx, null
                    jz      finPreorden

                    mov     ax, %901
                    mov     dx, fx
                    mov     cx, 1
                    sys     2

                    mov     cx, 1
                    sys     5
                    mov     [dx], ' '
                    mov     ax, %910
                    mov     cx, 1
                    sys     2
                    sys     6

                    push    [fx+izq]
                    call    preorden
                    add     sp, 1

                    push    [fx+der]
                    call    preorden
                    add     sp, 1
finPreorden:        pop     fx
                    pop     dx
                    pop     cx
                    pop     bx
                    pop     ax
                    mov     sp, bp
                    pop     bp
                    ret
; fin :: preorden


; Especificaion :: postorden -> void postorden(TArbol A)   
;   push    < ptro a raiz >
;   call    postorden
;   add     sp, 1
postorden:          push    bp
                    mov     bp, sp
                    push    ax
                    push    bx
                    push    cx
                    push    dx
                    push    fx

                    mov     fx, [bp+2]
                    cmp     fx, null
                    jz      finPostorden

                    push    [fx+izq]
                    call    postorden
                    add     sp, 1

                    push    [fx+der]
                    call    postorden
                    add     sp, 1

                    mov     ax, %901
                    mov     dx, fx
                    mov     cx, 1
                    sys     2

                    mov     cx, 1
                    sys     5
                    mov     [dx], ' '
                    mov     ax, %910
                    mov     cx, 1
                    sys     2
                    sys     6

finPostorden:       pop     fx
                    pop     dx
                    pop     cx
                    pop     bx
                    pop     ax
                    mov     sp, bp
                    pop     bp
                    ret
; fin :: postorden


; int esta(TArbol A, int x) { 
;     if (A == NULL)
;         return 0;
;     else {
;         if (A->clave == x)
;             return 1;
;         else {
;             if (x < A->clave)
;                 return esta(A->izq, x);
;             else
;                 return esta(A->der, x);
;         }
;     }
; }
; Especificacion :: estaABB  -> int estaABB(TArbol A, int x)
;   --- ABB ---
;   push    < valor a busca > 
;   push    < ptr a nodo >
;   call    estaABB
;   add     sp, 2
;   AX <-  1 == esta, 0 == no esta

estaABB:            push    bp
                    mov     bp, sp
                    push    fx

                    mov     fx, [bp+2]
                    cmp     fx, null
                    jz      noencontrado

                    cmp     [bp+3], [fx]
                    jnz     continuar

                    mov     ax, true
                    jmp     finEstaABB

continuar:          cmp     [bp+3], [fx]
                    jp      buscarDerecha
                    push    [bp+3]
                    push    [fx+izq]
                    call    estaABB
                    add     sp, 2
                    jmp     finEstaABB
buscarDerecha:      push    [bp+3]
                    push    [fx+der]
                    call    estaABB
                    add     sp, 2
                    jmp     finEstaABB
noencontrado:       mov     ax, false
finEstaABB:         pop     fx
                    mov     sp, bp
                    pop     bp
                    ret
; fin :: estaABB


; int esta(TArbol A, int x) {
;     if (A == NULL)
;         return 0;
;     else {
;         if (A->dato == x)
;             return 1;
;         else 
;             return esta(A->izq, x) || esta(A->der, x);
;     }
; }
; Especificacion :: esta  -> int esta(TArbol A, int x)
;   push    < valor a busca > 
;   push    < ptr a nodo >
;   call    esta
;   add     sp, 2
;   AX <-  1 == esta, 0 == no esta
esta:               push    bp
                    mov     bp, sp
                    push    fx

                    mov     fx, [bp+2]
                    cmp     fx, null
                    jz      noencont        ; si es null -> no esta devuelvo 0

                    cmp     [bp+3], [fx]
                    jnz     cont

                    mov     ax, true        ; si encontre devuelvo 1
                    jmp     finEsta         

cont:               push    [bp+3]          ; busco en subArbol izq
                    push    [fx+izq]
                    call    esta
                    add     sp, 2
                    
                    cmp     ax, true        ; si esta en subArbol izq corto recursion
                    jz      finEsta

                    push    [bp+3]          ; busco en el derecho
                    push    [fx+der]
                    call    esta
                    add     sp, 2

                    jmp     finEsta         ; el resultado queda definido por el valor que trae AX
                                            ; en la rama derecha
noencont:           mov     ax, false
finEsta:            pop     fx
                    mov     sp, bp
                    pop     bp
                    ret
; fin :: esta

; Especificacion :: cantNiveles -> int cantNiveles(TArbol A) 
;   push    < ptr a nodo >
;   call    cantNiveles
;   add     sp, 1
;   AX  <-  cantidad de niveles del arbol

cantNiveles:        push    bp
                    mov     bp, sp
                    push    fx
                    push    dx
                    push    cx

                    mov     fx, [bp+2]                    
                    cmp     fx, null
                    jnz     sigo

                    mov     ax, 0
                    jmp     finCantNiveles
sigo:               push    [fx+izq]
                    call    cantNiveles
                    add     sp, 1
                    mov     cx, ax

                    push    [fx+der]
                    call    cantNiveles
                    add     sp, 1
                    mov     dx, ax

                    cmp     cx, dx
                    jp      izqMayor

                    mov     ax, dx
                    jmp     unoMas
izqMayor:           mov     ax, cx
unoMas:             add     ax, 1                    
finCantNiveles:     pop     cx
                    pop     dx
                    pop     fx
                    mov     sp, bp
                    pop     bp
                    ret
