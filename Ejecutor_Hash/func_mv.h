#include <stdio.h>
#include <stdlib.h>


#define CANT_REG 16
#define CANT_RAM 4096


typedef struct {
    int ram[CANT_RAM];
    int registro[CANT_REG];
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
void print_binary(memoria_t memoria);
operacion decodificar_operacion(int instruccion_hex);

// dos operandos
void MOV (int *a, int *b, int *c);
void ADD (int *a, int *b, int *c);
void SUB (int *a, int *b, int *c);
void SWAP(int *a, int *b, int *c);
void MUL (int *a, int *b, int *c);
void DIV (int *a, int *b, int *c);
void CMP (int *a, int *b, int *c);
void SHL (int *a, int *b, int *c);
void SHR (int *a, int *b, int *c);
void AND (int *a, int *b, int *c);
void OR  (int *a, int *b, int *c);
void XOR (int *a, int *b, int *c);

// un operando
void SYS (int *a, int *b, int *c);
void JMP (int *a, int *b, int *c);
void JZ  (int *a, int *b, int *c);
void JP  (int *a, int *b, int *c);
void JN  (int *a, int *b, int *c);
void JNZ (int *a, int *b, int *c);
void JNP (int *a, int *b, int *c);
void JNN (int *a, int *b, int *c);
void LDL (int *a, int *b, int *c);
void LDH (int *a, int *b, int *c);
void RND (int *a, int *b, int *c);
void NOT (int *a, int *b, int *c);

// cero operandos
void STOP(int *a, int *b, int *c);
