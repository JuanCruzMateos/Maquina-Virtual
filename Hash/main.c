#include "hashtable.h"
#include "funcmv.h"


void cargar_funciones(hash_table_t *ht);


int main(int argc, char *argv[]) {
    FILE *binfile;
    memoria_t memoria;
    funct_ptr func;
    operacion op;
    hash_table_t *ht;

    if (argc == 1)
        return -1;
    binfile = fopen(argv[1], "rb");
    if (binfile == NULL)
        return -1;
    inicializar_registros(memoria.registro);
    load_ram(binfile, &memoria);
    fclose(binfile);
    get_flags(argv, argc, &memoria);
    // print_binary(memoria);
    ht = hash_new();
    cargar_funciones(ht);
    if (memoria.flags.c)
        system("cls");
    if (memoria.flags.d)
        disassembler(memoria.ram, memoria.registro);
    memoria.registro[5] = 0;
    while (0 <= memoria.registro[5] && memoria.registro[5] < memoria.registro[0]) {
        op = decodificar_operacion(memoria.ram[memoria.registro[5]]);
        memoria.registro[5] += 1;
        func = (funct_ptr) hash_get(ht, op.codigo_op);
        // if (func == NULL)
        //     printf("problema\n");
        switch (op.estado) {
            case CERO_OP:
                func(NULL, NULL, &memoria);
                break;
            case DOS_OP_REG_IN:
                func(&(memoria.registro[op.valor_a]), &(op.valor_b), &memoria);
                break;
            case DOS_OP_REG_REG:
                func(&(memoria.registro[op.valor_a]), &(memoria.registro[op.valor_b]), &memoria);
                break;
            case DOS_OP_REG_DIR:
                func(&(memoria.registro[op.valor_a]), &(memoria.ram[op.valor_b + memoria.registro[0]]), &memoria);
                break;
            case DOS_OP_DIR_IN:
                func(&(memoria.ram[op.valor_a + memoria.registro[0]]), &(op.valor_b), &memoria);
                break;
            case DOS_OP_DIR_REG:
                func(&(memoria.ram[op.valor_a + memoria.registro[0]]), &(memoria.registro[op.valor_b]), &memoria);
                break;
            case DOS_OP_DIR_DIR:
                func(&(memoria.ram[op.valor_a + memoria.registro[0]]), &(memoria.ram[op.valor_b + memoria.registro[0]]), &memoria);
                break;
            case DOS_OP_IN_IN:
                func(&op.valor_a, &op.valor_b, &memoria);
                break;
            case DOS_OP_IN_REG:
                func(&op.valor_a, &(memoria.registro[op.valor_b]), &memoria);
                break;
            case DOS_OP_IN_DIR:
                func(&op.valor_a, &(memoria.ram[op.valor_b + memoria.registro[0]]), &memoria);
                break;
            case UN_OP_IN:
                func(&op.valor_a, NULL, &memoria);
                break;
            case UN_OP_REG:
                func(&(memoria.registro[op.valor_a]), NULL, &memoria);
                break;
            case UN_OP_DIR:
                func(&(memoria.ram[op.valor_a + memoria.registro[0]]), NULL, &memoria);
                break;
        }
    }
    hash_delete(ht);
    return 0;
}


void cargar_funciones(hash_table_t *ht) {
    hash_put(ht, 0, MOV);  
    hash_put(ht, 1, ADD);
    hash_put(ht, 2, SUB);
    hash_put(ht, 3, SWAP);
    hash_put(ht, 4, MUL);
    hash_put(ht, 5, DIV);
    hash_put(ht, 6, CMP);
    hash_put(ht, 7, SHL);
    hash_put(ht, 8, SHR);
    hash_put(ht, 9, AND);
    hash_put(ht, 10, OR);
    hash_put(ht, 11, XOR);
    hash_put(ht, 0xF0, SYS);
    hash_put(ht, 0xF1, JMP);
    hash_put(ht, 0xF2, JZ);
    hash_put(ht, 0xF3, JP);
    hash_put(ht, 0xF4, JN);
    hash_put(ht, 0xF5, JNZ);
    hash_put(ht, 0xF6, JNP);
    hash_put(ht, 0xF7, JNN);
    hash_put(ht, 0xF8, LDL);
    hash_put(ht, 0xF9, LDH);
    hash_put(ht, 0xFA, RND);
    hash_put(ht, 0xFB, NOT);
    hash_put(ht, 0xFF1, STOP);
}