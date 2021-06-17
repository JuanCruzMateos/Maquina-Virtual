#include "funcmv.h"
#include <time.h>

char *tags[][16] = {{"RET" ,"STOP",    "",    "",     "",    "",    "",    "",    "",    "",   "",    "",     "",    "" ,      "",   "" },
                    {"SYS" , "JMP",  "JZ",   "JP",  "JN", "JNZ", "JNP", "JNN", "LDL", "LDH","RND", "NOT", "PUSH",  "POP",  "CALL",   "" },
                    {"MOV" , "ADD", "SUB", "SWAP", "MUL", "DIV", "CMP", "SHL", "SHR", "AND", "OR", "XOR", "SLEN", "SMOV",  "SCMP",   "" },
                    {"DS"  ,  "SS",  "ES",   "CS",  "HP",  "IP",  "SP",  "BP",  "CC",  "AC", "AX",  "BX",   "CX",   "DX",    "EX",  "FX"}};


int read_header(FILE *arch, memoria_t *memoria) {
    int ident, ds, es, ss, cs;

    fread(&ident, sizeof(int), 1, arch);
    if (ident != 0x4D563231)
        return 1;
    else {
        fread(&ds, sizeof(int), 1, arch);
        fread(&ss, sizeof(int), 1, arch);
        fread(&es, sizeof(int), 1, arch);
        fread(&cs, sizeof(int), 1, arch);
        if (ds+es+ss+cs > CANT_RAM) {
            return 2;
        } else {
            memoria->registro[3] = (cs << 16);
            memoria->registro[0] = (ds << 16) | (cs & 0xFFFF);
            memoria->registro[2] = (es << 16) | ((cs + ds) & 0xFFFF);
            memoria->registro[1] = (ss << 16) | ((cs + ds + es) & 0xFFFF);
            return 0;
        }
    }
}

void init_reg(int *reg) {
    // HP = -1
    reg[4] = -1;
    for (int i = 5; i < CANT_REG; i++)
        reg[i] = 0;
    // reg[6] == SP
    reg[6] = 0x00010000 | reg[1] >> 16;   // se inicializa con el tamaño de la pila
    // reg[7] == BP
    reg[7] = 0x00010000 | reg[1] >> 16;
}

void load_cs(FILE *arch, memoria_t *memoria) {
    int i = 0, x;

    fseek(arch, sizeof(int) * 5, SEEK_SET);
    while (fread(&x, sizeof(int), 1, arch) == 1) {
        (*memoria).ram[i++] = x;
    }
    memoria->segfault = 0;
    memoria->stack_overflow = 0;
    memoria->stack_underflow = 0;
    memoria->step = 0;
}

int get_flags(char *argv[], int argc, memoria_t *memoria) {
    int invalid_flag = 0;

    memoria->flags.b = memoria->flags.c = memoria->flags.d = 0;
    if (argc > 2) {
        for (int i = 2; i < argc; i++) {
            if (strcmp(argv[i], "-b") == 0)
                memoria->flags.b = 1;
            else if (strcmp(argv[i], "-c") == 0)
                memoria->flags.c = 1;
            else if (strcmp(argv[i], "-d") == 0)
                memoria->flags.d = 1;
            else
                invalid_flag = 1;
        }
    }
    return invalid_flag;
}

void print_binary(memoria_t memoria){
    int i, tam = (memoria.registro[3] >> 16) & 0xFFFF;

    for (i = 0; i < tam; i++) {
        printf("[%04d] %02X %02X %02X %02X\n", i, (memoria.ram[i] >> 24) & 0xFF, (memoria.ram[i] >> 16) & 0xFF, (memoria.ram[i] >> 8) & 0xFF, memoria.ram[i] & 0xFF);
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

int valor_operando_a(int instruccion_hex, int cant_op, int tipo) {
    int valor;

    if (cant_op == 0)
        return -1;
    else {
        if (cant_op == 1) {
            valor = instruccion_hex & 0xFFFF;
            if (tipo == 3) {
                valor &= 0x0FFF;
                valor |= valor & 0x800 ? 0xFFFFF000 : 0x0; 
            } else {
                valor |= valor & 0x8000 ? 0xFFFF0000 : 0x0;
            }
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
                case 0: estado = UN_OP_INM; break;
                case 1: estado = UN_OP_REG; break;
                case 2: estado = UN_OP_DIR; break;
                case 3: estado = UN_OP_IND; break;
            }
            break;
        case 2:
            if (tipo_a == 0) {
                switch (tipo_b) {
                    case 0: estado = DOS_OP_INM_INM; break;
                    case 1: estado = DOS_OP_INM_REG; break;
                    case 2: estado = DOS_OP_INM_DIR; break;
                    case 3: estado = DOS_OP_INM_IND; break;
                }
            } else if (tipo_a == 1) {
                switch (tipo_b) {
                    case 0: estado = DOS_OP_REG_INM; break;
                    case 1: estado = DOS_OP_REG_REG; break;
                    case 2: estado = DOS_OP_REG_DIR; break;
                    case 3: estado = DOS_OP_REG_IND; break;
                }
            } else if (tipo_a == 2) {
                switch (tipo_b) {
                    case 0: estado = DOS_OP_DIR_INM; break;
                    case 1: estado = DOS_OP_DIR_REG; break;
                    case 2: estado = DOS_OP_DIR_DIR; break;
                    case 3: estado = DOS_OP_DIR_IND; break;
                }
            } else if (tipo_a == 3) {
                switch (tipo_b) {
                    case 0: estado = DOS_OP_IND_INM; break;
                    case 1: estado = DOS_OP_IND_REG; break;
                    case 2: estado = DOS_OP_IND_DIR; break;
                    case 3: estado = DOS_OP_IND_IND; break;
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
    op.tipo_a    = tipo_operando_a(instruccion_hex, cant_op);
    op.valor_a   = valor_operando_a(instruccion_hex, cant_op, op.tipo_a);
    op.tipo_b    = tipo_operando_b(instruccion_hex, cant_op);
    op.valor_b   = valor_operando_b(instruccion_hex, cant_op);
    op.estado    = setEstado(cant_op, op.tipo_a, op.tipo_b);
    return op;
}

void modificar_CC(int resultado, int *reg) {
    int N = (resultado & 0x80000000);
    int Z = (resultado == 0);
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
    // printf("a = %d, b = %d\n", *a, *b);
    int res = (*a - *b);
    modificar_CC(res, mem->registro);
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
    int formato    = registro[10] & 0xF;
    int scan_char  = registro[10] & 0x100;
    char buffer[256];
    int i, j;

    fflush(stdin);
    if (scan_char) {
        if (has_prompt)
            printf("[%04d]: ", registro[13]);
        fgets(buffer, 256, stdin);
        i = 0;
        j = registro[13] + (registro[0] & 0xFFFF);
        while (buffer[i] != '\n' && j <= registro[12] + (registro[0] & 0xFFFF)) {
            ram[j] = buffer[i];
            i += 1;
            j += 1;
        }
        ram[j] = 0;
    } else {
        for (i = registro[13]; i < registro[13] + registro[12]; i++) {
            if (has_prompt)
                printf("[%04d]: ", i);
            switch (formato) {
                case 1: scanf("%d", &ram[i + (registro[0] & 0xFFFF)]); break;
                case 4: scanf("%o", &ram[i + (registro[0] & 0xFFFF)]); break;
                case 8: scanf("%x", &ram[i + (registro[0] & 0xFFFF)]); break;
            }
        }
    }
}

int printable(int x) {
    if (32 <= x && x <= 126)
        return x;
    return 46;
}

void sys_write(int *ram, int *registro) {
    int has_prompt = (registro[10] & 0x800) == 0;
    int has_end    = (registro[10] & 0x100) == 0;
    int formato    = registro[10] & 0x1F;
    int ini = (registro[registro[13] >> 16] & 0xFFFF) + (registro[13] & 0xFFFF);
    int i;

    if (formato == 16) {
        if (has_prompt)
            printf("[%04d]: ", ini);
        for (i = ini; i < ini + registro[12]; i++)
            printf("%c", ram[i] & 0xFF);
        if (has_end)
            printf("\n");
    } else {
        // for (i = (registro[13] & 0xFFFF); i < (registro[13] & 0xFFFF) + registro[12]; i++) {
        for (i = ini; i < ini + registro[12]; i++) {
            if (has_prompt)
                printf("[%04d]: ", i);
            switch (formato) {
                case  1: printf("%d ", ram[i]); break;
                case  4: printf("@%08o ", ram[i]); break;
                case  8: printf("%%%08X ", ram[i]); break;
                // case 16: printf("%c ", ram[i] & 0xFF); break;
                default:
                    registro[10] & 0x10 ? printf("%c ", ram[i] & 0xFF) : printf(" ");
                    registro[10] & 0x08 ? printf("%%%08X ", ram[i]) : printf(" ");
                    registro[10] & 0x04 ? printf("@%08o ", ram[i]) : printf(" ");
                    registro[10] & 0x01 ? printf("%d ", ram[i]) : printf(" ");
                    break;
            }
            if (has_end)
                printf("\n");
        }
    }
}

void print_registros(int *registro) {
    printf("\nRegistros:\n");
    printf("| DS = %7s%08X | SS = %7s%08X | ES = %7s%08X | CS = %7s%08X |\n", "", registro[0], "", registro[1], "", registro[2], "", registro[3]);
    printf("| HP = %7s%08X | IP = %15d | SP = %7s%08X | BP = %7s%08X |\n", "", registro[4], registro[5], "", registro[6], "", registro[7]);
    printf("| CC = %15d | AC = %15d | AX = %15d | BX = %15d |\n", registro[8], registro[9], registro[10], registro[11]);
    printf("| CX = %15d | DX = %15d | EX = %15d | FX = %15d |\n\n", registro[12], registro[13], registro[14], registro[15]);
}

void disassembler(int *ram, int *registro) {
    int inicio = (registro[5] - 5 >= 0) ? registro[5] - 5 : 0;
    int fin = (registro[5] + 5 < (registro[0] & 0xFFFF)) ? registro[5] + 5 : (registro[0] & 0xFFFF);
    char mem[8], hex[12];
    char *mnem, op_1[15], op_2[15];
    operacion op;
    int i;
    int aux;

    if (fin - inicio < 10) {
        if (fin == (registro[0] & 0xFFFF) && inicio != 0)
            inicio = (fin - 10 >= 0) ? fin - 10 : 0;
        else if (inicio == 0 && fin != (registro[0] & 0xFFFF))
            fin = (inicio + 10 <= (registro[0] & 0xFFFF)) ? inicio + 10 : (registro[0] & 0xFFFF);
    }
    printf("\n");
    for (i = inicio; i < fin; i++) {
        op = decodificar_operacion(ram[i]);
        i == registro[5] ? sprintf(mem, ">[%04d]", i) : sprintf(mem, " [%04d]", i);
        sprintf(hex, "%02X %02X %02X %02X", (ram[i] >> 24) & 0xFF, (ram[i] >> 16) & 0xFF, (ram[i] >> 8) & 0xFF, ram[i] & 0xFF);
        // mnemotico
        if (0 <= op.codigo_op && op.codigo_op <= 14)
            mnem = tags[2][op.codigo_op];
        else if (240 <= op.codigo_op && op.codigo_op <= 254)
            mnem = tags[1][op.codigo_op & 0xF];
        else
            mnem = tags[0][op.codigo_op & 0xF];
        // operando a
        switch (op.tipo_a) {
            case -1: sprintf(op_1, "%s", ""); break;
            case  0: sprintf(op_1, "%d", op.valor_a); break;
            case  1: sprintf(op_1, "%s", tags[3][op.valor_a]); break;
            case  2: sprintf(op_1, "[%d]", op.valor_a); break;
            case  3: sprintf(op_1, "%d", dir_mem_abs_indirecto(op.valor_a, registro, &aux));
        }
        // operando b
        switch (op.tipo_b) {
            case  0: sprintf(op_2, "%d", op.valor_b); break;
            case  1: sprintf(op_2, "%s", tags[3][op.valor_b]); break;
            case  2: sprintf(op_2, "[%d]", op.valor_b); break;
            case  3: sprintf(op_2, "%d", dir_mem_abs_indirecto(op.valor_b, registro, &aux));
        }
        printf("%s %s    %2d: %5s %5s %s", mem, hex, i+1, mnem, " ", op_1);
        (op.tipo_b == -1) ? printf("\n") : printf(", %s\n", op_2);
    }
    print_registros(registro);
}

void sys_breakpoint(int *ram, int *registro, flags_t flags, int *step) {
    static int saltear_bp = 0;
    char buffer[30];
    char *nums;
    int i, n1, n2;

    fflush(stdin);
    if (!saltear_bp) {     
        *step = 0;
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
                    printf("[%04d]: %04X %04X %c %d\n", i, (ram[i] & 0xFFFF0000) >> 16, ram[i] & 0xFFFF, printable(ram[i]), ram[i]);
            } else {
                *step = 1;
            }
        }
    }
}

void str_read(int *ram, int *registro) {
    int has_prompt = (registro[10] & 0x800) == 0;
    int cod_seg    = registro[13] >> 16;
    int base       = registro[cod_seg] & 0xFFFF;
    int offset     = registro[13] & 0xFFFF;
    int cant_max_char   = registro[12] - 1;
    int tope_mem    = (registro[cod_seg] & 0xFFFF) + (registro[cod_seg] >> 16);
    char buffer[256];
    int i, j;

    if (has_prompt)
        printf("[%04d]: ", base + offset);
    fflush(stdin);
    fgets(buffer, 256, stdin);
    i = 0;
    j = base + offset;
    while ((buffer[i] != '\n') && (i < cant_max_char) && j < tope_mem - 1) {
        ram[j] = buffer[i];
        i += 1; j += 1;
    }
    ram[j] = 0;
}

void str_write(int *ram, int *registro) {
    int has_prompt = (registro[10] & 0x800) == 0;
    int has_end    = (registro[10] & 0x100) == 0;
    int cod_seg    = registro[13] >> 16;
    int base       = registro[cod_seg] & 0xFFFF;
    int offset     = registro[13] & 0xFFFF;

    if (has_prompt)
        printf("[%04d]: ", base + offset);
    for (int i = base + offset; ram[i] != 0; i++) {
        printf("%c", ram[i]);
    }
    if (has_end)
        printf("\n");
}

void init_heap(int *ram, int *registro) {
    int inicio_ES = registro[2] & 0xFFFF;
    int tam_ES    = registro[2] >> 16;

    ram[inicio_ES] = (tam_ES - 1) << 16;
    registro[4] = 0x0000FFFF;
}

void printfmemoria(int *ram, int *registro){
    printf("\n\nMuestro memoria:\n");
    for (int i=registro[2] & 0xFFFF; i<((registro[2] & 0xFFFF)+20);i++){
        printf("[%04d]: %08X = %04X %04X = %04d %04d\n", i-(registro[2] & 0xFFFF),ram[i],ram[i]>> 16,ram[i] & 0xFFFF,ram[i]>> 16,ram[i] & 0xFFFF);
    }
    printf("Fin muestreo\n\n");
}

void es_new(int *ram, int *registro) {
    int inicio_ES   = registro[2] & 0xFFFF;
    int cant_pedida = registro[12];  // CX
    int act_lib, ant_lib, act_ocup, ant_ocup;
    int ultimo_bloque_libre;
    int nuevo_bloque_libre;

    act_lib  = registro[4] >> 16;        // HPH -> libres
    act_ocup = registro[4] & 0xFFFF;     // HPL -> ocupados
    // si ya esta todo asignado
    if (act_lib == 0xFFFF) {
        registro[13] = -1;   // DX = -1;
    } else {
        // recorro de entrada hasta llegar al ultimo
        do {
            ant_lib = act_lib;
            act_lib = ram[inicio_ES + act_lib] & 0xFFFF;
        } while ((act_lib != (registro[4] >> 16)) && (cant_pedida > (ram[inicio_ES + act_lib] >> 16)));
        // si salgo del while es porque o llegue a la cabeza || cant es la que quiero
        // si llegue a la cabeza pregunto si tiene la cantidad que quiero
        if (cant_pedida <= (ram[inicio_ES + act_lib] >> 16)) {
            // puede ser que sea == || < 
            if (cant_pedida == (ram[inicio_ES + act_lib] >> 16)) {
                // si es igual -> dos casos: me piden todo y hpl == -1 || 
                if ((registro[4] & 0xFFFF) == 0xFFFF) {
                    registro[4] = 0xFFFF0000;
                } else {
                    ultimo_bloque_libre = (act_lib == (ram[inicio_ES + act_lib] >> 16));
                    // ant_lib->sig = act_lib->sig;
                    ram[inicio_ES + ant_lib] = (ram[inicio_ES + ant_lib] & 0xFFFF0000) | (ram[inicio_ES + act_lib] & 0xFFFF);
                    do {
                        ant_ocup = act_ocup;
                        act_ocup = ram[inicio_ES + act_ocup] & 0xFFFF;
                    } while (act_ocup != (registro[4] & 0xFFFF) && (act_ocup > act_lib || ((ram[inicio_ES + act_ocup] & 0xFFFF) < act_lib)));
                    // act_lib->sig = act_ocup->sig;
                    ram[inicio_ES + act_lib] = (ram[inicio_ES + act_lib] & 0xFFFF0000) | (ram[inicio_ES + act_ocup] & 0xFFFF);
                    // act_ocup_sig = act->lib;
                    ram[inicio_ES + act_ocup] = (ram[inicio_ES + act_ocup] & 0xFFFF0000) | act_lib;
                    // actualizo HPH y HPL
                    if (ultimo_bloque_libre)
                        registro[4] = 0xFFFF0000 | act_lib;
                    else
                        registro[4] = (registro[4] & 0xFFFF0000) | act_lib;
                }
                registro[13] = 0x00020000 | (act_lib + 1);
            } else {
                // encontre un bloque : lo que me piden es menos de lo que encontre
                // posicion del nuevo bloque libre en memoria (parto)
                nuevo_bloque_libre = act_lib + cant_pedida + 1;

                // creo header del nuevo nodo libre
                if ((ram[inicio_ES + act_lib] & 0xFFFF) != act_lib)
                    ram[inicio_ES + nuevo_bloque_libre] = (((ram[inicio_ES + act_lib] >> 16) - cant_pedida - 1) << 16) | (ram[inicio_ES + act_lib] & 0xFFFF);
                else // es un solo bloque -> el nuevo tambien debe apuntarse a si mismo
                    ram[inicio_ES + nuevo_bloque_libre] = ((((ram[inicio_ES + act_lib] >> 16) - cant_pedida - 1) << 16)) | nuevo_bloque_libre;
                
                // actualizo siguiente del anterior (solo si exite siguiente y no es un solo nodo)
                if (act_lib != ant_lib)
                    ram[inicio_ES + ant_lib] = (ram[inicio_ES + ant_lib] & 0xFFFF0000) | nuevo_bloque_libre;

                // si el el primer bloque que se ocupa
                if (act_ocup == 0xFFFF) {
                    // actualizo HP
                    registro[4] = (nuevo_bloque_libre << 16) | act_lib;
                    // actualizo header del nuevo ocupado
                    ram[inicio_ES + act_lib] =  (cant_pedida << 16) | act_lib;
                } else {
                    // busco el anterior ocupado
                    do {
                        ant_ocup = act_ocup;
                        act_ocup = ram[inicio_ES + act_ocup] & 0xFFFF;
                    } while (act_ocup != (registro[4] & 0xFFFF) && (act_ocup > act_lib || ((ram[inicio_ES + act_ocup] & 0xFFFF) < act_lib)));

                    // actuazlizo header del nuevo nodo ocupado con : tam | sig del anterior ocupado
                    ram[inicio_ES + act_lib] = (cant_pedida << 16) | (ram[inicio_ES + act_ocup] & 0xFFFF);
                    // actualizo header del anterior acupado: siguiente = nuevo nodo ocupado
                    ram[inicio_ES + act_ocup] = (ram[inicio_ES + act_ocup] & 0xFFFF0000) | act_lib;

                    // si ocupo el que es apuntado por HPH -> hago que HPH apunte al sig;
                    if ((registro[4] >> 16) == act_lib)
                        registro[4] = (nuevo_bloque_libre << 16) | act_lib;
                    else
                        registro[4] = (registro[4] & 0xFFFF0000) | act_lib;
                }
                registro[13] = 0x00020000 | (act_lib + 1);
            }
        } else {
            // no hay memoria suficiente en ES
            registro[13] = -1;   // DX = -1;
        }
    }
    // printfmemoria(ram, registro);
}

void compactar(int *ram, int *registro) {
    int inicio_ES = registro[2] & 0xFFFF;
    int ant = registro[4] >> 16;
    int act = ram[inicio_ES+ant] & 0xFFFF;
    int tam_nuevo;

    if (ant != 0xFFFF) {
        while (act != (registro[4] >> 16)) {
            if (ant + (ram[inicio_ES + ant] >> 16) + 1 == act) {
                tam_nuevo = 1 + (ram[inicio_ES + ant] >> 16) + (ram[inicio_ES + act] >> 16);
                ram[inicio_ES + ant] = (tam_nuevo << 16) | (ram[inicio_ES + act] & 0xFFFF);
            } else {
                ant = act;
            }
            act = ram[inicio_ES + act] & 0xFFFF;
        }
    }
    // printfmemoria(ram, registro);
}


void es_free(int *ram, int *registro) {
    int inicio_ES = registro[2] & 0xFFFF;
    int pos_liberar;
    int act_ocup, ant_ocup, act_lib, ant_lib;
    int un_solo_bloque_ocupado;
    int proximo_ocupado;

    // if (reg[13] >> 16 != 2) -> que pasa si no es del ES?
    pos_liberar = registro[13] & 0xFFFF;
    act_lib  = registro[4] >> 16;
    act_ocup = registro[4] & 0xFFFF;

    // si hay memoria por liberar
    if (act_ocup != 0xFFFF) {

        do {
            ant_ocup = act_ocup;
            act_ocup = ram[inicio_ES + act_ocup] & 0xFFFF;
        } while (act_ocup != (registro[4] & 0xFFFF) && !(act_ocup <= pos_liberar && pos_liberar <= (act_ocup + (ram[inicio_ES + act_ocup] >> 16))));

        // si encontre la posicion
        if (act_ocup <= pos_liberar && pos_liberar <= (act_ocup + (ram[inicio_ES + act_ocup] >> 16))) {
            
            // si HPH == -1 :: no hay nada libre 
            if (act_lib == 0xFFFF) {
                    // si act_ocup->sig = act_ocup => todo el bloque esta ocupado
                if ((ram[inicio_ES + act_ocup] & 0xFFFF) == act_ocup) {
                    registro[4] = 0x0000FFFF;
                } else {
                    // hay varios bloques ocupados -> solo libero ese
                    // ant_ocup->sig = act_ocup->sig;
                    ram[inicio_ES + ant_ocup] = (ram[inicio_ES + ant_ocup] & 0xFFFF0000) | (ram[inicio_ES + act_ocup] & 0xFFFF);

                    // actualizo HPH Y HPL
                    // HPH es el nuevo nodo que estoy liberando
                    // if HPL == act_ocup: HPL = act_ocup->sig;
                    if ((registro[4] & 0xFFFF) == act_ocup)
                        registro[4] = (act_ocup << 16) | (ram[inicio_ES + act_ocup] & 0xFFFF);
                    else
                        registro[4] = (act_ocup << 16) | (registro[4] & 0xFFFF);

                    // actualizo header del nuevo nodo que estoy liberando
                    ram[inicio_ES + act_ocup] = (ram[inicio_ES + act_ocup] & 0xFFFF0000) | act_ocup;
                }
            } else {
                // hay bloques libres
                un_solo_bloque_ocupado = ((ram[inicio_ES + act_ocup] & 0xFFFF) == act_ocup);
                // guardo el proximo ocupado del bloque a liberar
                proximo_ocupado = ram[inicio_ES + act_ocup] & 0xFFFF;
                // recorro lista de libres buscando al anterior
                do {
                    ant_lib = act_lib;
                    act_lib = ram[inicio_ES + act_lib] & 0xFFFF;
                } while ((act_lib != (registro[4] >> 16)) && (act_lib > act_ocup || ((ram[inicio_ES + act_lib] & 0xFFFF) < act_ocup)));

                // act_ocup->sig = act_lib->sig;
                ram[inicio_ES + act_ocup] = (ram[inicio_ES + act_ocup] & 0xFFFF000) | (ram[inicio_ES + act_lib] & 0xFFFF);
                // act_lib->sig = act_ocup;
                ram[inicio_ES + act_lib]  = (ram[inicio_ES + act_lib] & 0xFFFF0000) | (act_ocup);
                
                // ant_ocup->sig = act_coup->sig;
                if (act_ocup != ant_ocup) {
                    ram[inicio_ES + ant_ocup] = (ram[inicio_ES + ant_ocup] & 0xFFFF0000) | (proximo_ocupado);
                }
                
                // si el HPL apunta al que libero -> lo paso al siguiente
                if ((registro[4] & 0xFFFF) == act_ocup) {
                    if (un_solo_bloque_ocupado)
                        registro[4] = (registro[4] & 0xFFFF0000) | 0xFFFF;
                    else
                        registro[4] = (registro[4] & 0xFFFF0000) | ant_ocup;
                }        
                // al finalizar compacto el ES
                compactar(ram, registro);
            }
        }
    }
}

int dir_mem_abs_indirecto(int valorOp, int *registro, int *segfault) {
    int code_seg;
    int base;
    int offset;

    if ((valorOp & 0xF) == 0) {
        // pertenece al DS
        base = registro[0] & 0xFFFF;
        offset = valorOp >> 4;
        code_seg = 0;
    } else {
        // me quedo con la parte alta del registro -> segmento al cual quiero acceder
        code_seg = registro[valorOp & 0xF] >> 16;
        // printf("%d\n", code_seg);
        // busco donde empieza ese segmento
        base = registro[code_seg] & 0xFFFF;
        // printf("%d\n", base);
        // calculo es offset = parte baja del registro + offset instruccion
        offset = (registro[valorOp & 0xF] & 0xFFFF) + (valorOp >> 4);
        // printf("%d\n", offset);
    }
    // printf("codeop = %d, offset = %d, base = %d\n", code_seg, offset, base);
    if (offset > (registro[code_seg] >> 16))
        *segfault = 1;
    return base + offset;
}

// un operando
void SYS(int *a, int *b, memoria_t *mem) {
    switch (*a) {
        case  1: sys_read(mem->ram, mem->registro); break;
        case  2: sys_write(mem->ram, mem->registro); break;
        case  3: str_read(mem->ram, mem->registro); break;
        case  4: str_write(mem->ram, mem->registro); break;
        case  5: if (mem->registro[4] == -1) init_heap(mem->ram, mem->registro);
                 es_new(mem->ram, mem->registro); 
                 break;
        case  6: es_free(mem->ram, mem->registro); break;
        case  7: system("cls"); break;
        case 15: sys_breakpoint(mem->ram, mem->registro, mem->flags, &(mem->step)); break;
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
    mem->registro[9] = (mem->registro[9] & 0xFFFF0000) | (*a & 0xFFFF);
}

void LDH(int *a, int *b, memoria_t *mem) {
    mem->registro[9] = (mem->registro[9] & 0x0000FFFF) | ((*a & 0xFFFF) << 16);
}

void RND (int *a, int *b, memoria_t *mem) {
    srand(time(NULL));
    mem->registro[9] = rand() % (*a+1);
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

/* ----------------------    Funciones de Pila   --------------------- */

void PUSH(int *a, int *b, memoria_t *mem) {
    // si SPL es cero --> pila llena (verifico antes de restar el registro)
    if ((mem->registro[6] & 0x0000FFFF) == 0) {
        // TODO -> terminar proceso --> como lo hacemos? es void
        mem->stack_overflow = 1;
    } else {
        // SP = SP - 1
        mem->registro[6] -= 1;
        // Guardo el dato que viene en *a en la posicion de memoria relativa al SS
        // verdadera direccion = SSL + SPL
        mem->ram[(mem->registro[1] & 0x0000FFFF) + (mem->registro[6] & 0x0000FFFF)] = *a;
        // no cambia el registro cc no?
    }
}

void POP(int *a, int *b, memoria_t *mem) {
    // Si SPL + SSL >= SSH --> pila vacia --> stack-underflow
    if ((mem->registro[6] & 0x0000FFFF) /*+ (mem->registro[1] & 0x0000FFFF))*/ >= ((mem->registro[1] & 0xFFFF0000)>>16)) {
        mem->stack_underflow = 1;
    } else {
        // guardamos en *a el dato guardado en la direccion de memoria SSL + SPL
        *a = mem->ram[(mem->registro[1] & 0x0000FFFF) + (mem->registro[6] & 0x0000FFFF)];
        // SP = SP + 1
        mem->registro[6] += 1;
    }
}

void CALL(int *a, int *b, memoria_t *mem) {
    // guardo en la pila la direccion de retorno (utilizo el IP como proxima direccion)
    PUSH(&(mem->registro[5]), NULL, mem);
    // salto a la posicion indicada en *a
    JMP(a, NULL, mem);
}

// cero operandos
// func(NULL, NULL, &mem)
void RET(int *a, int *b, memoria_t *mem) {
    // obtengo valor del tope de la pila y lo guardo en paux
    int paux;
    POP(&paux, NULL, mem);
    // salto a esa direccion
    JMP(&paux, NULL, mem);
}

/* ----------------------------   Funciones de Strings    ------------------------------ */

void SLEN(int *a, int *b, memoria_t *mem) {
    int longitud = 0;

    while (*b != 0) {
        longitud++;
        b++;
    }
    *a = longitud;
}


void SMOV(int *a, int *b, memoria_t *mem) {
    // while (*a++ = *b++) ;
    while (*b != 0) {
        *a = *b;
        b++;
        a++;
    }
    *a = *b;
}

void SCMP(int *a, int *b, memoria_t *mem) {
    int finstr = 0;

    while (*a == *b && !finstr) {
        finstr = *a == 0;
        a++;
        b++;
    }
    modificar_CC(finstr ? 0 : *a - *b, mem->registro);
}
