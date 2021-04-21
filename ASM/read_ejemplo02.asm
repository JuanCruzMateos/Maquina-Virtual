    mov  ax, %100     ;lee caracter a caracter guandando uno por celda
    mov  dx, 11
    mov  cx, 50
    sys  %1
    mov  ax, %800     ;omite promp
    or   ax, %100     ;omitir endln despues de cada celda
    or   ax, %010     ; imprime char
    sys  %2
    stop