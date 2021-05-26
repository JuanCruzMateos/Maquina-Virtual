#include "hashtable.h"
#include "funcmv.h"

/* ********************************************************************************************************************
 *                                       EJECUTOR MAQUINA VIRTUAL - GRUPO I                                           *
 * ********************************************************************************************************************/


void cargar_funciones(hash_table_t *ht);


int main(int argc, char *argv[]) {
    FILE *binfile;
    memoria_t memoria;
    funct_ptr func;
    operacion op;
    hash_table_t *ht;
    int mem_absA, mem_absB;
    int check_header, check_flags;

    if (argc == 1) {
        printf(RED); printf("Falta archivo.bin"); printf(RESET);
        return -1;
    }
    binfile = fopen(argv[1], "rb");
    if (binfile == NULL) {
        printf(RED); printf("Archivo inexistente."); printf(RESET);
        return -1;
    }
    check_header = read_header(binfile, &memoria);
    if (check_header == 1) {
        printf(RED); printf("Formato de archivo x.bin incorrecto."); printf(RESET);
        return -1;
    } else if (check_header == 2) {
        printf(RED); printf("Memoria insuficiente."); printf(RESET);
        return -1;
    }
    init_reg(memoria.registro);
    load_cs(binfile, &memoria);
    fclose(binfile);
    check_flags = get_flags(argv, argc, &memoria);
    if (check_flags != 0) {
        printf(YELLOW); printf("Flag invalido. Unicamente permitidos: [-c] [-d] [-b]\n"); printf(RESET);
    }
    // print_binary(memoria);
    ht = hash_new();
    cargar_funciones(ht);
    if (memoria.flags.c)
        system("cls");
    if (memoria.flags.d)
        disassembler(memoria.ram, memoria.registro);
    memoria.registro[5] = 0;
    while ((0 <= memoria.registro[5] && memoria.registro[5] < (memoria.registro[0] & 0xFFFF)) && !memoria.segfault && !memoria.stack_overflow && !memoria.stack_underflow) {
        op = decodificar_operacion(memoria.ram[memoria.registro[5]]);
        memoria.registro[5] += 1;
        func = (funct_ptr) hash_get(ht, op.codigo_op);
        switch (op.estado) {
            case CERO_OP:
                func(NULL, NULL, &memoria);
                break;
            case DOS_OP_REG_INM:
                func(&(memoria.registro[op.valor_a]), &(op.valor_b), &memoria);
                break;
            case DOS_OP_REG_REG:
                func(&(memoria.registro[op.valor_a]), &(memoria.registro[op.valor_b]), &memoria);
                break;
            case DOS_OP_REG_DIR:
                if ((memoria.registro[0] & 0xFFFF) + op.valor_b <= (memoria.registro[0] >> 16))
                    func(&(memoria.registro[op.valor_a]), &(memoria.ram[op.valor_b + (memoria.registro[0] & 0xFFFF)]), &memoria);
                else
                    memoria.segfault = 1;
                break;
            case DOS_OP_REG_IND:
                mem_absB = dir_mem_abs_indirecto(op.valor_b, memoria.registro, &(memoria.segfault));
                if (!memoria.segfault)
                    func(&(memoria.registro[op.valor_a]), &(memoria.ram[mem_absB]), &memoria);
                break;
            case DOS_OP_DIR_INM:
                func(&(memoria.ram[op.valor_a + (memoria.registro[0] & 0xFFFF)]), &(op.valor_b), &memoria);
                break;
            case DOS_OP_DIR_REG:
                func(&(memoria.ram[op.valor_a + (memoria.registro[0] & 0xFFFF)]), &(memoria.registro[op.valor_b]), &memoria);
                break;
            case DOS_OP_DIR_DIR:
                func(&(memoria.ram[op.valor_a + (memoria.registro[0] & 0xFFFF)]), &(memoria.ram[op.valor_b + (memoria.registro[0] & 0xFFFF)]), &memoria);
                break;
            case DOS_OP_DIR_IND:
                mem_absB = dir_mem_abs_indirecto(op.valor_b, memoria.registro,&(memoria.segfault));
                if (!memoria.segfault)
                    func(&(memoria.ram[op.valor_a + (memoria.registro[0] & 0xFFFF)]), &(memoria.ram[mem_absB]), &memoria);
                break;
            case DOS_OP_INM_INM:
                func(&op.valor_a, &op.valor_b, &memoria);
                break;
            case DOS_OP_INM_REG:
                func(&op.valor_a, &(memoria.registro[op.valor_b]), &memoria);
                break;
            case DOS_OP_INM_DIR:
                func(&op.valor_a, &(memoria.ram[op.valor_b + (memoria.registro[0] & 0xFFFF)]), &memoria);
                break;
            case DOS_OP_INM_IND:
                mem_absB = dir_mem_abs_indirecto(op.valor_b, memoria.registro, &(memoria.segfault));
                if (!memoria.segfault)
                    func(&op.valor_a, &(memoria.ram[mem_absB]), &memoria);
                break;
            case DOS_OP_IND_INM:
                // printf("op.valora = %d, valor en memoria = %d\n", op.valor_a, memoria.ram[dir_mem_abs_indirecto(op.valor_a, memoria.registro, &(memoria.segfault))]);
                // printf("op.valorb = %d, valor en memoria = %d", op.valor_a, memoria.ram[dir_mem_abs_indirecto(op.valor_a, memoria.registro, &(memoria.segfault))]);
                mem_absA = dir_mem_abs_indirecto(op.valor_a, memoria.registro, &(memoria.segfault));
                if (!memoria.segfault)
                    func(&(memoria.ram[mem_absA]), &op.valor_b, &memoria);
                break;
            case DOS_OP_IND_REG:
                mem_absA = dir_mem_abs_indirecto(op.valor_a, memoria.registro, &(memoria.segfault));
                if (!memoria.segfault)
                    func(&(memoria.ram[mem_absA]), &(memoria.registro[op.valor_b]), &memoria);
                break;
            case DOS_OP_IND_DIR:
                mem_absA = dir_mem_abs_indirecto(op.valor_a, memoria.registro, &(memoria.segfault));
                if (!memoria.segfault)
                    func(&(memoria.ram[mem_absA]), &(memoria.ram[op.valor_b + (memoria.registro[0] & 0xFFFF)]), &memoria);
                break;
            case DOS_OP_IND_IND:
                mem_absA = dir_mem_abs_indirecto(op.valor_a, memoria.registro, &(memoria.segfault));
                if (!memoria.segfault)
                    mem_absB = dir_mem_abs_indirecto(op.valor_b, memoria.registro, &(memoria.segfault));
                if (!memoria.segfault)
                    func(&(memoria.ram[mem_absA]), &(memoria.ram[mem_absB]), &memoria);
                break;
            case UN_OP_INM:
                func(&op.valor_a, NULL, &memoria);
                break;
            case UN_OP_REG:
                func(&(memoria.registro[op.valor_a]), NULL, &memoria);
                break;
            case UN_OP_DIR:
                func(&(memoria.ram[op.valor_a + (memoria.registro[0] & 0xFFFF)]), NULL, &memoria);
                break;
            case UN_OP_IND:
                mem_absA = dir_mem_abs_indirecto(op.valor_a, memoria.registro, &(memoria.segfault));
                if (!memoria.segfault)
                    func(&(memoria.ram[mem_absA]), NULL, &memoria);
                break;
        }
    }
    if (memoria.segfault) {
        printf(RED); printf("Segmentation Fault."); printf(RESET);
    } else if (memoria.stack_overflow) {
        printf(RED); printf("Stack-overflow"); printf(RESET);
    } else if (memoria.stack_underflow) {
        printf(RED); printf("Stack-underflow"); printf(RESET);
    }
    hash_delete(ht);
    return 0;
}


void cargar_funciones(hash_table_t *ht) {
    // dos operandos
    hash_put(ht,  0, MOV);  
    hash_put(ht,  1, ADD);
    hash_put(ht,  2, SUB);
    hash_put(ht,  3, SWAP);
    hash_put(ht,  4, MUL);
    hash_put(ht,  5, DIV);
    hash_put(ht,  6, CMP);
    hash_put(ht,  7, SHL);
    hash_put(ht,  8, SHR);
    hash_put(ht,  9, AND);
    hash_put(ht, 10, OR);
    hash_put(ht, 11, XOR);
    hash_put(ht, 12, SLEN);
    hash_put(ht, 13, SMOV);
    hash_put(ht, 14, SCMP);

    // un operando
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
    hash_put(ht, 0xFC, PUSH);
    hash_put(ht, 0xFD, POP);
    hash_put(ht, 0xFE, CALL);

    // cero operandos
    hash_put(ht, 0xFF0, RET);
    hash_put(ht, 0xFF1, STOP);
}
