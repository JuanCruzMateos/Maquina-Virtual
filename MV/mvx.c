#include "func_mv.h"

// reg[0] = DS
// reg[5] = IP

int main(int argc, char const *argv[]) {
    int instr_hex;
    operacion op;
    FILE *binfile;

    binfile = fopen(argv[1], "rb");
    if (binfile == NULL)
        return -1;
    load_ram(binfile, ram, &registro[0]);
    fclose(binfile);
    print_bin(ram, registro[0]);

    registro[5] = 0;
    printf("reg[5] = %d, reg[0] = %d\n", registro[5], registro[0]);
    // printf("primera instruccion %X\n", ram[registro[5]]);
    // printf("cant op = %d\n", cantidad_operandos(ram[registro[5]]));
    // printf("cod op = %d\n", codigo_operacion(ram[registro[5]], cantidad_operandos(ram[registro[5]])));
    // printf("tipo a = %d\n", tipo_operando_a(ram[registro[5]], cantidad_operandos(ram[registro[5]])));
    // printf("tipo b = %d\n", tipo_operando_b(ram[registro[5]], cantidad_operandos(ram[registro[5]])));
    // printf("valor a = %d\n", valor_operando_a(ram[registro[5]], cantidad_operandos(ram[registro[5]])));
    // printf("valor b = %d\n", valor_operando_b(ram[registro[5]], cantidad_operandos(ram[registro[5]])));
    // printf("estado = %d\n", setEstado(cantidad_operandos(ram[registro[5]]),tipo_operando_a(ram[registro[5]], cantidad_operandos(ram[registro[5]])),  tipo_operando_b(ram[registro[5]], cantidad_operandos(ram[registro[5]]))));

    while (0 <= registro[5] && registro[5] < registro[0]) {
        op = decodificar_operacion(ram[registro[5]]);
        // printf("cod = %03X \ntipoA = %02X \ntipoB = %02X \nvalorA = %02X \nvalorB = %02X\n\n", op.codigo_op, op.tipo_a, op.tipo_b, op.valor_a, op.valor_b);
        registro[5] += 2;
        switch (op.estado) {
            case CERO_OP:
                STOP(); break;
            case DOS_OP_REG_IN:
                (*instruccion_dos_op[op.codigo_op])(&registro[op.tipo_a], &(op.valor_b));
                break;
            case DOS_OP_REG_REG:
                (*instruccion_dos_op[op.codigo_op])(&registro[op.tipo_a], &registro[op.tipo_b]);
                break;
            case DOS_OP_REG_DIR:
                (*instruccion_dos_op[op.codigo_op])(&registro[op.tipo_a], &ram[op.valor_b + registro[0]]);
                break;
            case DOS_OP_DIR_IN:
                (*instruccion_dos_op[op.codigo_op])(&ram[op.valor_a + registro[0]], &(op.valor_b));
                break;
            case DOS_OP_DIR_REG:
                (*instruccion_dos_op[op.codigo_op])(&ram[op.valor_a + registro[0]], &registro[op.tipo_b]);
                break;
            case DOS_OP_DIR_DIR:
                (*instruccion_dos_op[op.codigo_op])(&ram[op.valor_a + registro[0]], &ram[op.valor_b + registro[0]]);
                break;
            case UN_OP_IN:
                (*instruccion_un_op[op.codigo_op & 0xF])(&op.tipo_a);
                break;
            case UN_OP_REG:
                (*instruccion_un_op[op.codigo_op & 0xF])(&registro[op.tipo_a]);
                break;
            case UN_OP_DIR:
                (*instruccion_un_op[op.codigo_op & 0xF])(&ram[op.valor_a + registro[0]]);
                break;
        }
    }
    return 0;
}
