; 8. Cargar palabras en un vector hasta que se lea una palabra vacía y ordenarlas alfabéticamente.
;    Nota: considerar palabras de 20 caracteres como máximo.
prompt  equ "Ingrese palabras (enter para terminar): "
vector  equ  0

maxlen  equ  21

                ldh     3
                ldl     prompt
                mov     ax, %800
                mov     dx, ac
                sys     4

                mov     fx, 0       ; var i
loadVector:     mov     cx, maxlen
                sys     %5
                mov     [fx + vector], dx
                add     fx, 1 
                sub     cx, 1

