; 1. Indicar en los siguientes casos de invocación a funciones, como sería la invocación a la subrutina
; en ASM de la MV. Considerar que las variables a, b y c se encuentran en las posiciones de
; memoria 1001, 1002 y 1003 respectivamente.
;  a -> 1001
;  b -> 1002
;  c -> 1003

;       a. proce(a, b, c);

            push    [1003]
            push    [1002]
            push    [1001]
            call    proce
            add     sp, 3

;       b. c = func(*a, b);

            push    [1002]
            mov     ax, [1001]
            push    [ax]
            call    func
            add     sp, 2
            mov     [1003], ax

;       c. proce (&a, *b, *c);

            mov     ax, [1003]
            push    [ax]
            mov     ax, [1002]
            push    [ax]
            push    1001
            call    proce
            add     sp, 3
