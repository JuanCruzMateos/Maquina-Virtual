        mov  [10], 5
        mov  [11], 6
        mul  [10], [11]
        mov     ax, %001
        mov     dx, 10
        mov     cx, 1
        sys     2
        stop