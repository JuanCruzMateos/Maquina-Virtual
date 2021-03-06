#include "funcmv.h"

char *tags[][12] = {{"STOP",    "",    "",     "",    "",    "",    "",    "",    "",   "",    "",    ""},
                    {"SYS" , "JMP",  "JZ",   "JP",  "JN", "JNZ", "JNP", "JNN", "LDL", "LDH","RND", "NOT"},
                    {"MOV" , "ADD", "SUB", "SWAP", "MUL", "DIV", "CMP", "SHL", "SHR", "AND", "OR", "XOR"},
                    {"AC"  , "AX"  ,  "BX",  "CX",   "DX",  "EX",  "FX",   "",    "",    "",   "",   "" }};

void load_ram(FILE *arch, memoria_t *memoria) {
    int i = 0, x;
    while (fread(&x, sizeof(int), 1, arch) == 1) {
        (*memoria).ram[i++] = x;
    }
    memoria->registro[0] = i;
}

void get_flags(char *argv[], int argc, memoria_t *memoria) {
    memoria->flags.b = memoria->flags.c = memoria->flags.d = 0;
    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-b") == 0)
                memoria->flags.b = 1;
            else if (strcmp(argv[i], "-c") == 0)
                memoria->flags.c = 1;
            else if (strcmp(argv[i], "-d") == 0)
                memoria->flags.d = 1;
        }
    }
}

void inicializar_registros(int *reg) {
    for (int i = 0; i < CANT_REG; i++)
        reg[i] = 0;
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
            if (tipo_a == 0) {
                switch (tipo_b) {
                    case 0: estado = DOS_OP_IN_IN; break;
                    case 1: estado = DOS_OP_IN_REG; break;
                    case 2: estado = DOS_OP_IN_DIR; break;
                }
            } else if (tipo_a == 1) {
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

void modificar_CC(int resultado, int *reg) {
    int N = resultado & 0x80000000;
    int Z = resultado == 0;
    reg[8] = N | Z;
}

// dos operandos
void MOV(int *a, int *b, memoria_t *mem) {
    *a = *b;
}

void ADD(int *a, int *b, memoria_t *mem) {
    *a += *b;
    modificar_CC(*a, mem->registro);
}

void SUB(int *a, int *b, memoria_t *mem) {
    *a -= *b;
    modificar_CC(*a, mem->registro);
}

void SWAP(int *a, int *b, memoria_t *mem) {
    int aux = *a;
    *a = *b;
    *b = aux;
}

void MUL(int *a, int *b, memoria_t *mem) {
    *a *= *b;
    modificar_CC(*a, mem->registro);
}

void DIV(int *a, int *b, memoria_t *mem) {
    mem->registro[9] = *a % *b;
    *a /= *b;
    modificar_CC(*a, mem->registro);
}

void CMP(int *a, int *b, memoria_t *mem) {
    modificar_CC(*a - *b, mem->registro);
}

void SHL(int *a, int *b, memoria_t *mem) {
    *a = *a << *b;
    modificar_CC(*a, mem->registro);
}

void SHR(int *a, int *b, memoria_t *mem) {
    *a = *a >> *b;
    modificar_CC(*a, mem->registro);
}

void AND(int *a, int *b, memoria_t *mem) {
    *a = *a & *b;
    modificar_CC(*a, mem->registro);
}

void OR(int *a, int *b, memoria_t *mem) {
    *a = *a | *b;
    modificar_CC(*a, mem->registro);
}

void XOR(int *a, int *b, memoria_t *mem) {
    *a = *a ^ *b;
    modificar_CC(*a, mem->registro);
}

void sys_read(int *ram, int *registro) {
    int has_prompt = (registro[10] & 0x800) == 0;
    int formato  = registro[10] & 0xF;
    int scan_char = registro[10] & 0x100;
    char buffer[256];
    int i, j;

    if (scan_char) {
        if (has_prompt)
            printf("[%04d]: ", registro[13]);
        fflush(stdin);
        fgets(buffer, 256, stdin);
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

// verificar si el caracter es imprimible

void sys_write(int *ram, int *registro) {
    int has_prompt = (registro[10] & 0x800) == 0;
    int has_end = (registro[10] & 0x100) == 0;
    int formato = registro[10] & 0x1F;
    int i;

    if (has_prompt)
        printf("[%04d]:", registro[13]);
    for (i = registro[13]; i < registro[13] + registro[12]; i++) {
        switch (formato) {
            case 1:  printf("%d", ram[i + registro[0]]); break;
            case 4:  printf("@%08o", ram[i + registro[0]]); break;
            case 8:  printf("%%%08X", ram[i + registro[0]]); break;
            case 16: printf("%c", ram[i + registro[0]] & 0xFF); break;
            default:
                registro[10] & 0x10 ? printf("%c ", ram[i + registro[0]] & 0xFF) : printf(" ");
                registro[10] & 0x08 ? printf("%%%08X ", ram[i + registro[0]]) : printf(" ");
                registro[10] & 0x04 ? printf("@%08o ", ram[i + registro[0]]) : printf(" ");
                registro[10] & 0x01 ? printf("%d ", ram[i + registro[0]]) : printf(" ");
                break;
        }
        if (has_end)
            printf("\n");
    }
}

void print_registros(int *registro) {
    printf("\nRegistros:\n");
    printf("| DS = %15d | %20s | %20s | %20s |\n", registro[0], " ", " ", " ");
    printf("| %20s | IP = %15d | %20s | %20s |\n", " ", registro[5], " ", " ");
    printf("| CC = %15d | AC = %15d | AX = %15d | BX = %15d |\n", registro[8], registro[9], registro[10], registro[11]);
    printf("| CX = %15d | DX = %15d | EX = %15d | FX = %15d |\n\n", registro[12], registro[13], registro[14], registro[15]);
}

void disassembler(int *ram, int *registro) {
    int inicio = (registro[5] - 5 >= 0) ? registro[5] - 5 : 0;
    int fin = (registro[5] + 5 < registro[0]) ? registro[5] + 5 : registro[0];
    char mem[8], hex[12];
    char *mnem, op_1[15], op_2[15];
    operacion op;
    int i;

    if (fin - inicio < 10) {
        if (fin == registro[0] && inicio != 0)
            inicio = (fin - 10 >= 0) ? fin - 10 : 0;
        else if (inicio == 0 && fin != registro[0])
            fin = (inicio + 10 <= registro[0]) ? inicio + 10 : registro[0];
    }
    printf("\n");
    for (i = inicio; i < fin; i++) {
        op = decodificar_operacion(ram[i]);
        i == registro[5] ? sprintf(mem, ">[%04d]", i) : sprintf(mem, " [%04d]", i);
        sprintf(hex, "%02X %02X %02X %02X", (ram[i] >> 24) & 0xFF, (ram[i] >> 16) & 0xFF, (ram[i] >> 8) & 0xFF, ram[i] & 0xFF);
        // mnemotico
        if (0 <= op.codigo_op && op.codigo_op <= 11)
            mnem = tags[2][op.codigo_op];
        else if (240 <= op.codigo_op && op.codigo_op <= 251)
            mnem = tags[1][op.codigo_op & 0xF];
        else
            mnem = tags[0][0];
        // operando a
        switch (op.tipo_a) {
            case -1: sprintf(op_1, "%s", ""); break;
            case  0: sprintf(op_1, "%d", op.valor_a); break;
            case  1: sprintf(op_1, "%s", tags[3][op.valor_a - 9]); break;
            case  2: sprintf(op_1, "[%d]", op.valor_a); break;
        }
        // operando b
        switch (op.tipo_b) {
            case  0: sprintf(op_2, "%d", op.valor_b); break;
            case  1: sprintf(op_2, "%s", tags[3][op.valor_b - 9]); break;
            case  2: sprintf(op_2, "[%d]", op.valor_b); break;
        }
        printf("%s %s    %2d: %5s %5s %s", mem, hex, i+1, mnem, " ", op_1);
        (op.tipo_b == -1) ? printf("\n") : printf(", %s\n", op_2);
    }
    print_registros(registro);
}

void sys_breakpoint(int *ram, int *registro, flags_t flags) {
    static int saltear_bp = 0;
    char buffer[30];
    char *nums;
    int i, n1, n2;

    if (!saltear_bp) {     
        // if (flags.c)
        //     system("cls");
        if (flags.d)
            disassembler(ram, registro);
        if (flags.b) {
            printf("[%04d] cmd: ", registro[5]);
            fgets(buffer, 15, stdin);
            if (buffer[0] == '\n')
                saltear_bp = 1;
            else if (buffer[0] != 'p') {
                nums = strtok(buffer, " ");
                n1 = atoi(nums);
                nums = strtok(NULL, " ");
                if (nums != NULL)
                    n2 = atoi(nums);
                else
                    n2 = n1;
                for (i = n1; i <= n2; i++)
                    printf("[%04d]: %04X %04X %6d\n", i, (ram[i] & 0xFFFF0000) >> 16, ram[i] & 0xFFFF, ram[i]);
            }
        }
    }
}

// un operando
void SYS(int *a, int *b, memoria_t *mem) {
    switch (*a) {
        case  1: sys_read(mem->ram, mem->registro); break;
        case  2: sys_write(mem->ram, mem->registro);break;
        case  3: /* str_read */ break;
        case  4: /* str_write*/ break;
        case  5: /* new */ break;
        case  6: /* free */ break;
        case  7: system("cls"); break;
        case 15: sys_breakpoint(mem->ram, mem->registro, mem->flags); break;
    }
}

void JMP(int *a, int *b, memoria_t *mem) {
    mem->registro[5] = *a;
}

void JZ(int *a, int *b, memoria_t *mem) {
    if ((mem->registro[8] & 0x1) == 1)
        mem->registro[5] = *a;
}

void JP(int *a, int *b, memoria_t *mem) {
    if ((mem->registro[8] & 0x80000000) == 0)
        mem->registro[5] = *a;
}

void JN(int *a, int *b, memoria_t *mem) {
    if (mem->registro[8] & 0x80000000)
        mem->registro[5]= *a;
}

void JNZ(int *a, int *b, memoria_t *mem) {
   if ((mem->registro[8] & 0x1) == 0)
        mem->registro[5] = *a;
}

void JNP(int *a, int *b, memoria_t *mem) {
    if (mem->registro[8] & 0x80000000 || mem->registro[8] & 0x00000001)
        mem->registro[5] = *a;
}

void JNN(int *a, int *b, memoria_t *mem) {
    if ((mem->registro[8] & 0x80000000) == 0) 
        mem->registro[5] = *a;
}

void LDL(int *a, int *b, memoria_t *mem) {
    mem->registro[9] = (mem->registro[9] & 0xFFFFFFFF) | (*a & 0xFFFF);
}

void LDH(int *a, int *b, memoria_t *mem) {
    mem->registro[9] = (mem->registro[9] & 0xFFFFFFFF) | ((*a & 0xFFFF) << 16);
}

void RND (int *a, int *b, memoria_t *mem) {
    // implementar
}

void NOT(int *a, int *b, memoria_t *mem) {
    *a = ~(*a);
    modificar_CC(*a, mem->registro);
}

// cero operandos
// func(NULL, NULL, &mem)
void STOP(int *a, int *b, memoria_t *mem) {
    mem->registro[5] = -1;
}
