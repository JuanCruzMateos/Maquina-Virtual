#include "func_mv.h"

// registros:
//  0 - DO
//  5 - IP
//  8 - CC
//  9 - AC
// 10 - AX
// 11 - BX
// 12 - CX
// 13 - DX
// 14 - EX
// 15 - FX
int registro[CANT_REG] = {0};
// ram
int ram[CANT_RAM] = {0};

void (*instruccion_dos_op[]) (int *, int *) = {MOV,ADD,SUB,SWAP,MUL,DIV,CMP,SHL,SHR,AND,OR,XOR};
void (*instruccion_un_op[]) (int *) = {SYS,JMP,JZ,JP,JN,JNZ,JNP,JNN,LDL,LDH,RND,NOT};

void load_ram(FILE *arch, int *mem, int *DS) {
    int i = 0, x;

    while (fread(&x, sizeof(int), 1, arch) == 1)
        mem[i++] = x;
    *DS = i;
}

void print_bin(int *ram, int DS) {
    int i = 0, num;

    while (i < DS) {
        printf(" [%04d] %02X %02X %02X %02X\n", i, (ram[i] >> 24) & 0xFF, (ram[i] >> 16) & 0xFF, (ram[i] >> 8) & 0xFF, ram[i] & 0xFF);
        i += 2;
    }
}

int cantidad_operandos(int instrucccion_hex) {
    if ((instrucccion_hex & 0xFF0FFFFF) == 0xFF000000)
        return 0;
    else if ((instrucccion_hex & 0xF03F0000) == 0xF0000000)
        return 1;
    else
        return 2;
}

int codigo_operacion(int instruccion_hex, int cant_op) {
    if (cant_op == 2)
        return (instruccion_hex >> 28) & 0xF;
    else if (cant_op == 1)
        return (instruccion_hex >> 24) & 0xFF;
    else
        return (instruccion_hex >> 20) & 0xFFF;
}

int tipo_operando_a(int instruccion_hex, int cant_op) {
    if (cant_op == 0)
        return -1;
    else if (cant_op == 1)
        return (instruccion_hex >> 22) & 0x3;
    else
        return (instruccion_hex >> 26) & 0x3;
}

int tipo_operando_b(int instruccion_hex, int cant_op) {
    if (cant_op == 0 || cant_op == 1)
        return -1;
    else
        return (instruccion_hex >> 24) & 0x3;
}

int valor_operando_a(int instruccion_hex, int cant_op) {
    if (cant_op == 0)
        return -1;
    else if (cant_op == 1)
        return instruccion_hex & 0xFFFF;
    else
        return (instruccion_hex >> 12) & 0xFFF;
}

int valor_operando_b(int instruccion_hex, int cant_op) {
    if (cant_op == 0 || cant_op == 1)
        return -1;
    else
        return instruccion_hex & 0xFFF;
}


estados setEstado(int cant_op, int tipo_a, int tipo_b) {
    estados estado;

    switch (cant_op) {
        case 0: 
            estado = CERO_OP; 
            break;
        case 1:
            switch (tipo_a) {
                case 0: estado = UN_OP_IN; break;
                case 1: estado = UN_OP_REG; break;
                case 2: estado = UN_OP_DIR; break;
            }
            break;
        case 2: 
            if (tipo_a == 1) {
                switch (tipo_b) {
                    case 0: estado = DOS_OP_REG_IN; break;
                    case 1: estado = DOS_OP_REG_REG; break;
                    case 2: estado = DOS_OP_REG_DIR; break;
                }
            } else if (tipo_a == 2) {
                switch (tipo_b) {
                    case 0: estado = DOS_OP_DIR_IN; break;
                    case 1: estado = DOS_OP_DIR_REG; break;
                    case 2: estado = DOS_OP_DIR_DIR; break;
                }
            }
            break;
    }
    return estado;
}

operacion decodificar_operacion(int instruccion_hex) {
    operacion op;
    int cant_op = cantidad_operandos(instruccion_hex);

    op.codigo_op = codigo_operacion(instruccion_hex, cant_op);
    op.tipo_a = tipo_operando_a(instruccion_hex, cant_op);
    op.valor_a = valor_operando_a(instruccion_hex, cant_op);
    op.tipo_b = tipo_operando_b(instruccion_hex, cant_op);
    op.valor_b = valor_operando_b(instruccion_hex, cant_op);
    op.estado = setEstado(cant_op, op.tipo_a, op.tipo_b);
    return op;
}

void MOV(int *a, int *b) {
    *a = *b;
    
}

void ADD(int *a, int *b) {
    *a += *b;
}

void SUB(int *a, int *b) {
    *a -= *b;
}

void SWAP(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

void MUL(int *a, int *b) {
    *a += *b;
}

void DIV(int *a, int *b) {
    *a = *a / *b;
    registro[9] = *a % *b;
}

void CMP(int *a, int *b) {
}

void SHL(int *a, int *b) {
    *a = *a << *b;
}

void SHR(int *a, int *b) {
    *a = *a >> *b;
}

void AND(int *a, int *b) {
    *a = *a & *b;
}

void OR(int *a, int *b) {
    *a = *a | *b;
}

void XOR(int *a, int *b) {
    *a = *a ^ *b;
}

void SYS(int *a) {
    // if (*a == 1)
    //     // scanf
    // else if (*a == 2)
    //     // printf()
    // else
    //     // breakpoint
}

void JMP(int *a) {

}

void JZ(int *a) {

}

void JP(int *a) {

}

void JN(int *a) {

}

void JNZ(int *a) {

}

void JNP(int *a) {

}

void JNN(int *a) {

}

void LDL(int *a) {

}

void LDH(int *a) {

}

void RND(int *a) {

}

void NOT(int *a) {
    *a = ~(*a);
}

void STOP() {
    registro[5] = -1;
}
