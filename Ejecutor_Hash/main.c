#include "func_mv.h"
#include "hashtable.h"

// registro[0] = DS
// registro[5] = IP


void sumar(int *a, int *b, int *c) {
    *a = *b + *c;
}

void restar(int *a, int *b, int *c) {
    *a = *b - *c;
}


int main(int argc, char const *argv[]) {
    memoria_t memoria;
    operacion op;
    FILE *binfile;
    char *fname = "../../ASM/fibo.bin";
    hash_table_t *ht;


    ht = hash_crear();
    hash_guardar(ht, 1, sumar);
    hash_guardar(ht, 2, restar);

    int a = 0;
    int b = 5;
    int c = 3;

    hash_obtener(ht, 2)(&a, &b, &c);
    system("color 03");
    printf("a = %d, b = %d, c = %d\n", a, b, c);

//    hash_guardar(ht, 0, MOV);
//    hash_guardar(ht, 1, ADD);
//    hash_guardar(ht, 2, SUB);
//    hash_guardar(ht, 3, SWAP);
//    hash_guardar(ht, 4, MUL);
//    hash_guardar(ht, 5, DIV);
//    hash_guardar(ht, 6, CMP);
//    hash_guardar(ht, 7, SHL);
//    hash_guardar(ht, 8, SHR);
//    hash_guardar(ht, 9, AND);
//    hash_guardar(ht, 10, OR);
//    hash_guardar(ht, 11, XOR);
//
//    hash_guardar(ht, 0xF0, SYS);
//    hash_guardar(ht, 0xF1, JMP);
//    hash_guardar(ht, 0xF2, JZ);
//    hash_guardar(ht, 0xF3, JP);
//    hash_guardar(ht, 0xF4, JN);
//    hash_guardar(ht, 0xF5, JNZ);
//    hash_guardar(ht, 0xF6, JNP);
//    hash_guardar(ht, 0xF7, JNN);
//    hash_guardar(ht, 0xF8, LDL);
//    hash_guardar(ht, 0xF9, LDH);
//    hash_guardar(ht, 0xFA, RND);
//    hash_guardar(ht, 0xFB, NOT);
//
//    hash_guardar(ht, 0xFF1, STOP);

//    binfile = fopen(fname, "rb");
//    if (binfile == NULL)
//        return -1;
//    load_ram(binfile, &memoria);
//    print_binary(memoria);
//
//    memoria.registro[5] = 0;
//    while (0 <= memoria.registro[5] && memoria.registro[5] < memoria.registro[0]) {
//        op = decodificar_operacion(memoria.ram[memoria.registro[5]]);
//        memoria.registro[5] += 1;
//
//
//    }
    return 0;
}
