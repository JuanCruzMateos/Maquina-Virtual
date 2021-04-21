#include "func_mv.h"

// registros:
//  0 - DS
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

void (*instruccion_dos_op[]) (int *, int *) = {MOV, ADD, SUB, SWAP, MUL, DIV, CMP, SHL, SHR, AND,  OR, XOR};
void (*instruccion_un_op[]) (int *)         = {SYS, JMP,  JZ,   JP,  JN, JNZ, JNP, JNN, LDL, LDH, RND, NOT};

void load_ram(FILE *arch, int *mem, int *DS) {
    int i = 0, x;

    while (fread(&x, sizeof(int), 1, arch) == 1) {
        mem[i++] = x;
    }
    *DS = i;
}

void print_binary(int *ram, int DS) {
    int i = 0;

    while (i < DS) {
        printf(" [%04d] %02X %02X %02X %02X\n", i, (ram[i] >> 24) & 0xFF, (ram[i] >> 16) & 0xFF, (ram[i] >> 8) & 0xFF, ram[i] & 0xFF);
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

void modificar_CC(int resultado) {
    // CC == registro[8]
    int N = resultado & 0x80000000;
    int Z = resultado == 0;
    registro[8] = N | Z;
}

void MOV(int *a, int *b) {
    *a = *b;
}

void ADD(int *a, int *b) {
    *a += *b;
    modificar_CC(*a);
}

void SUB(int *a, int *b) {
    *a -= *b;
    modificar_CC(*a);
}

void SWAP(int *a, int *b) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

void MUL(int *a, int *b) {
    *a += *b;
    modificar_CC(*a);
}

void DIV(int *a, int *b) {
    *a = *a / *b;
    registro[9] = *a % *b;
    modificar_CC(*a);
}

void CMP(int *a, int *b) {
    modificar_CC(*a - *b);
}

void SHL(int *a, int *b) {
    *a = *a << *b;
    modificar_CC(*a);
}

void SHR(int *a, int *b) {
    *a = *a >> *b;
    modificar_CC(*a);
}

void AND(int *a, int *b) {
    *a = *a & *b;
    modificar_CC(*a);
}

void OR(int *a, int *b) {
    *a = *a | *b;
    modificar_CC(*a);
}

void XOR(int *a, int *b) {
    *a = *a ^ *b;
    modificar_CC(*a);
}

void sys_read() {
    int has_prompt = (registro[10] & 0x800) == 0;
    int formato  = registro[10] & 0xF;
    int scan_char = registro[10] & 0x100;
    char buffer[256];
    int i, j;

    if (scan_char) {
        if (has_prompt)
            printf("[%04d]:", registro[13]);
        fflush(stdin);
        gets(buffer);
        i = 0;
        j = registro[13] + registro[0];
        while (buffer[i] != '\n' && j <= registro[12] + registro[0]) {
            ram[j] = buffer[i];
            i += 1;
            j += 1;
        }
        ram[j] = '\0';
    } else {
        for (i = registro[13]; i < registro[13] + registro[12]; i++) {
            if (has_prompt)
                printf("[%04d]:", i);
            switch (formato) {
                case 1: scanf("%d", &ram[i + registro[0]]); break;
                case 4: scanf("%o", &ram[i + registro[0]]); break;
                case 8: scanf("%x", &ram[i + registro[0]]); break;
            }
        }
    }
}

void sys_write() {
    int has_prompt = (registro[10] & 0x800) == 0;
    int has_end = (registro[10] & 0x100) == 0;
    int formato = registro[10] & 0x1F;
    int i;

    for (i = registro[13]; i < + registro[13] + registro[12]; i++) {
        if (has_prompt)
            printf("[%04d]:", i);
        switch (formato) {
            case 1:  printf("%d", ram[i + registro[0]]); break;
            case 4:  printf("@%o", ram[i + registro[0]]); break;
            case 8:  printf("%%%X", ram[i + registro[0]]); break;
            case 16: printf("%c", ram[i + registro[0]] & 0xFF); break;
            default:
                registro[10] & 0x10 ? printf("%c ", ram[i + registro[0]] & 0xFF) : printf(" ");
                registro[10] & 0x08 ? printf("%%%X ", ram[i + registro[0]]) : printf(" ");
                registro[10] & 0x04 ? printf("@%o ", ram[i + registro[0]]) : printf(" ");
                registro[10] & 0x01 ? printf("%d ", ram[i + registro[0]]) : printf(" ");
                break;
        }
        if (has_end)
            printf("\n");
    }
}

void sys_breakpoint() {
    // TODO
}

void SYS(int *a) {
    if (*a == 1)  // scanf
        sys_read();
    else if (*a == 2)  // printf
        sys_write();
    else  // breakpoint
        sys_breakpoint();
}

void JMP(int *a) {
    registro[5] = *a;
}


void JP(int *a) {
    if ((registro[8] & 0x80000000) == 0)
        registro[5] = *a;
}

void JN(int *a) {
    if (registro[8] & 0x80000000)
        registro[5]= *a;
}

void JZ(int *a) {
    if (registro[8] & 0x1)
        registro[5] = *a;
}

void JNZ(int *a) {
    if ((registro[8] & 0x1) == 0)
        registro[5] = *a;
}

void JNP(int *a) {
    // TODO 
    // COMO ??? 
}

void JNN(int *a) {
    // TODO
    // COMO ??? 
}

void LDH(int *a) {
    registro[9] = (registro[9] & 0x3FFFFFFF) | ((*a & 0x3) << 30);
}

void LDL(int *a) {
    registro[9] = (registro[9] & 0xFFFFFFFC) | (*a & 0x3);
}

void RND(int *a) {
    // TODO: segunda parte
    // Carga en el primer operando un número aleatorio entre 0 y el valor del segundo operando
    // -> dos operandos ? 
    // *a = rand();
}

void NOT(int *a) {
    *a = ~(*a);
    modificar_CC(*a);
}

void STOP() {
    registro[5] = -1;
}


void print_codigo(int inst_actual) {
    system("cls");
    printf("Codigo: ");

}


// print registros:
void print_registros() {
    printf("Registros:\n");
    printf("| DS = %10d | %15s | %15s | %15s |\n", registro[0], " ", " ", " ");
    printf("| %15s | IP = %10d | %15s | %15s |\n", " ", registro[5], " ", " ");
    printf("| CC = %10d | AC = %10d | AX = %10d | BX = %10d |\n", registro[8], registro[9], registro[10], registro[11]);
    printf("| CX = %10d | DX = %10d | EX = %10d | FX = %10d |\n", registro[12], registro[13], registro[14], registro[15]);
}
