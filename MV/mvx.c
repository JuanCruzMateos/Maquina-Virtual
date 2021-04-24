#include "func_mv.h"

// registro[0] = DS
// registro[5] = IP

int main(int argc, char const *argv[]) {
    FILE *binfile;
    operacion op;
    int i;

    flags.b = flags.c = flags.d = 0;
    if (argc > 2) {
        for (i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-b") == 0)
                flags.b = 1;
            else if (strcmp(argv[i], "-c") == 0)
                flags.c = 1;
            else if (strcmp(argv[i], "-d") == 0)
                flags.d = 1;
        }
    }


    binfile = fopen(argv[1], "rb");
    if (binfile == NULL)
        return -1;
    load_ram(binfile, ram, &registro[0]);
    fclose(binfile);
    // printf("RAM: \n");
    // print_binary(ram, registro[0]);
    
    registro[5] = 0;
    if (flags.c)
        system("cls");
    if (flags.d)
        disassembler();
    while (0 <= registro[5] && registro[5] < registro[0]) {
        op = decodificar_operacion(ram[registro[5]]);
        // printf("instruccion: %02X %02X %02X %02X\n", (ram[registro[5]] >> 24) & 0xFF, (ram[registro[5]] >> 16) & 0xFF, (ram[registro[5]] >> 8) & 0xFF, ram[registro[5]] & 0xFF);
        // printf("cod = %03X \ntipoA = %02X \ntipoB = %02X \nvalorA = %02X \nvalorB = %02X\n\n", op.codigo_op, op.tipo_a, op.tipo_b, op.valor_a, op.valor_b);
        registro[5] += 1;
        switch (op.estado) {
            case CERO_OP:
                STOP(); break;
            case DOS_OP_REG_IN:
                (*instruccion_dos_op[op.codigo_op])(&registro[op.valor_a], &(op.valor_b));
                break;
            case DOS_OP_REG_REG:
                (*instruccion_dos_op[op.codigo_op])(&registro[op.valor_a], &registro[op.valor_b]);
                break;
            case DOS_OP_REG_DIR:
                (*instruccion_dos_op[op.codigo_op])(&registro[op.valor_a], &ram[op.valor_b + registro[0]]);
                break;
            case DOS_OP_DIR_IN:
                (*instruccion_dos_op[op.codigo_op])(&ram[op.valor_a + registro[0]], &(op.valor_b));
                break;
            case DOS_OP_DIR_REG:
                (*instruccion_dos_op[op.codigo_op])(&ram[op.valor_a + registro[0]], &registro[op.valor_b]);
                break;
            case DOS_OP_DIR_DIR:
                (*instruccion_dos_op[op.codigo_op])(&ram[op.valor_a + registro[0]], &ram[op.valor_b + registro[0]]);
                break;
            case DOS_OP_IN_IN:
                (*instruccion_dos_op[op.codigo_op])(&op.valor_a, &op.valor_b);
                break;
            case DOS_OP_IN_REG:
                (*instruccion_dos_op[op.codigo_op])(&op.valor_a, &registro[op.valor_b]);
                break;
            case DOS_OP_IN_DIR:
                (*instruccion_dos_op[op.codigo_op])(&op.valor_a, &ram[op.valor_b + registro[0]]);
                break;
            case UN_OP_IN:
                (*instruccion_un_op[op.codigo_op & 0xF])(&op.valor_a);
                break;
            case UN_OP_REG:
                (*instruccion_un_op[op.codigo_op & 0xF])(&registro[op.valor_a]);
                break;
            case UN_OP_DIR:
                (*instruccion_un_op[op.codigo_op & 0xF])(&ram[op.valor_a + registro[0]]);
                break;
        }
        // getchar();
    }
    print_registros();
    return 0;
}
