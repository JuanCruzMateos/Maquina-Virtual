str1  equ  "maria"
str2  equ   "caro"

            ldh     3
            ldl     str1
            mov     ex, ac      ;str1
            ldh     3
            ldl     str2
            mov     fx, ac      ; str2
            scmp    [ex], [fx]
            mov     [0], cc
            mov     ax, %008
            mov     dx, 0
            mov     cx, 1
            sys     2
            stop