#include "func_mv.h"


void load_ram(FILE *arch, memoria_t *memoria) {
    int i = 0, x;

    while (fread(&x, sizeof(int), 1, arch) == 1) {
        (*memoria).ram[i++] = x;
    }
    memoria->registro[0] = i;
}


void print_binary(memoria_t memoria){
    int i = 0;

    while (i < memoria.registro[0]) {
        printf("[%04d] %02X %02X %02X %02X\n", i, (memoria.ram[i] >> 24) & 0xFF, (memoria.ram[i] >> 16) & 0xFF, (memoria.ram[i] >> 8) & 0xFF, memoria.ram[i] & 0xFF);
        i += 1;
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


// VERIFICAR NEGATIVOS


int valor_operando_a(int instruccion_hex, int cant_op) {
    int valor;

    if (cant_op == 0)
        return -1;
    else {
        if (cant_op == 1) {
            valor = instruccion_hex & 0xFFFF;
            valor |= valor & 0x8000 ? 0xFFFF0000 : 0x0;
        } else {
            valor = (instruccion_hex >> 12) & 0xFFF;
            valor |= valor & 0x800 ? 0xFFFFF000 : 0x0;
        }
        return valor;
    }
}

int valor_operando_b(int instruccion_hex, int cant_op) {
    int valor;

    if (cant_op == 0 || cant_op == 1)
        return -1;
    else {
        valor = instruccion_hex & 0xFFF;
        valor |= valor & 0x800 ? 0xFFFFF000 : 0x0;
        return valor;
    }
}

// ---


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


void MOV(int *a, int *b, int *c) {
    *a = *b;
}

void ADD(int *a, int *b, int *c) {}
void SUB(int *a, int *b, int *c) {}
void SWAP(int *a, int *b, int *c) {}
void MUL(int *a, int *b, int *c) {}
void DIV(int *a, int *b, int *c) {}
void CMP(int *a, int *b, int *c) {}
void SHL(int *a, int *b, int *c) {}
void SHR(int *a, int *b, int *c) {}
void AND(int *a, int *b, int *c) {}
void OR(int *a, int *b, int *c) {}
void XOR(int *a, int *b, int *c) {}
// un operando
void SYS(int *a, int *b, int *c) {}
void JMP(int *a, int *b, int *c) {}
void JZ(int *a, int *b, int *c) {}
void JP(int *a, int *b, int *c) {}

void JN(int *a, int *b, int *c) {
}



void JNZ(int *a, int *b, int *c) {


}


void JNP(int *a, int *b, int *c) {

}


void JNN(int *a, int *b, int *c) {


}


void LDL(int *a, int *b, int *c) {

}


void LDH(int *a, int *b, int *c) {

}

void RND(int *a, int *b, int *c) {

}

void NOT(int *a, int *b, int *c) {

}

// cero operandos
void STOP(int *a, int *b, int *c) {

}
