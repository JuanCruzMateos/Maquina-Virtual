#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANT_RAM 8192
#define CANT_REG 16

// Puntero a funcion
typedef void (*funct_ptr)(int *, int *, void *);

typedef struct {
    int b, c, d;
} flags_t;

typedef struct {
    int ram[CANT_RAM];
    int registro[CANT_REG];
    flags_t flags;
} memoria_t;

typedef enum { DOS_OP_REG_IN, DOS_OP_REG_REG, DOS_OP_REG_DIR,
               DOS_OP_DIR_IN, DOS_OP_DIR_REG, DOS_OP_DIR_DIR,
               DOS_OP_IN_IN , DOS_OP_IN_REG , DOS_OP_IN_DIR ,
               UN_OP_IN     , UN_OP_REG     , UN_OP_DIR     ,
               CERO_OP
} estados;

typedef struct {
    estados estado;
    int codigo_op;
    int tipo_a;
    int valor_a;
    int tipo_b;
    int valor_b;
} operacion;

void load_ram(FILE *arch, memoria_t *memoria);
void get_flags(char *argv[], int argc, memoria_t *memoria);
void print_binary(memoria_t memoria);
operacion decodificar_operacion(int instruccion_hex);
void disassembler(int *ram, int *registro);
void inicializar_registros(int *reg);

// dos operandos
void MOV (int *a, int *b, memoria_t *mem);
void ADD (int *a, int *b, memoria_t *mem);
void SUB (int *a, int *b, memoria_t *mem);
void SWAP(int *a, int *b, memoria_t *mem);
void MUL (int *a, int *b, memoria_t *mem);
void DIV (int *a, int *b, memoria_t *mem);
void CMP (int *a, int *b, memoria_t *mem);
void SHL (int *a, int *b, memoria_t *mem);
void SHR (int *a, int *b, memoria_t *mem);
void AND (int *a, int *b, memoria_t *mem);
void OR  (int *a, int *b, memoria_t *mem);
void XOR (int *a, int *b, memoria_t *mem);

// un operando
void SYS (int *a, int *b, memoria_t *mem);
void JMP (int *a, int *b, memoria_t *mem);
void JZ  (int *a, int *b, memoria_t *mem);
void JP  (int *a, int *b, memoria_t *mem);
void JN  (int *a, int *b, memoria_t *mem);
void JNZ (int *a, int *b, memoria_t *mem);
void JNP (int *a, int *b, memoria_t *mem);
void JNN (int *a, int *b, memoria_t *mem);
void LDL (int *a, int *b, memoria_t *mem);
void LDH (int *a, int *b, memoria_t *mem);
void RND (int *a, int *b, memoria_t *mem);
void NOT (int *a, int *b, memoria_t *mem);

// cero operandos
void STOP(int *a, int *b, memoria_t *mem);
