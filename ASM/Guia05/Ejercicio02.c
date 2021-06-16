// gcc -S -fverbose-asm -O2 foo.c
// gcc -Wall -S test.c

void calculo( int x, int vec[]){
  vec[x] = vec[x-1] & vec[0];
}


void main() {
  int j=3, arreglo[5];
  arreglo[0] = 1020;
  arreglo[1] = arreglo[0] | 0x3FF;
  calculo( j, arreglo);
}