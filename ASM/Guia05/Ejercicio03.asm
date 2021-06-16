; 3. Transcribir a assembler el siguiente código:
; int z = 100;
; int vec[100];
;
; int functionl() {
;   int s;
;   int i;
;   vec[0] = 1;
;   for (i = 1; i < z; i++)
;       vec[i] = vec[i-1] * i;
;   s = sum(z);
;   print_int(s);
; }
;
; int sum(int n) {
;   int s = 0;
;   int i;
;   for (i = 0; i < n; i++)
;       s = s + vec[i];
;   return s;
; }
; Nota: La variable z se ubica en la celda 0 del ES y el arreglo vec[] se ubica en la celda 1 del ES, y sucesivas.
