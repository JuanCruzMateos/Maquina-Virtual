#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANT_RAM 8192
#define CANT_REG 16
#define RED      "\033[91m"
#define YELLOW   "\033[93m"
#define RESET    "\033[39m"

// Puntero a funcion
typedef void (*funct_ptr)(int *, int *, void *);

typedef struct {
    int b, c, d;
} flags_t;

typedef struct {
    int ram[CANT_RAM];
    int registro[CANT_REG];
    flags_t flags;
    int segfault;
} memoria_t;

typedef enum { DOS_OP_REG_INM, DOS_OP_REG_REG, DOS_OP_REG_DIR, DOS_OP_REG_IND,
               DOS_OP_DIR_INM, DOS_OP_DIR_REG, DOS_OP_DIR_DIR, DOS_OP_DIR_IND,
               DOS_OP_INM_INM, DOS_OP_INM_REG, DOS_OP_INM_DIR, DOS_OP_INM_IND,
               DOS_OP_IND_INM, DOS_OP_IND_REG, DOS_OP_IND_DIR, DOS_OP_IND_IND,
               UN_OP_INM     , UN_OP_REG     , UN_OP_DIR     , UN_OP_IND     ,
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

// @return 0 == OK, 1 == Formato no valido, 2 == Memoria insuficiente.
int read_header(FILE *arch, memoria_t *memoria);
void init_reg(int *reg);
void load_cs(FILE *arch, memoria_t *memoria);
// @return 0 == OK, 1 == flag invalido
int get_flags(char *argv[], int argc, memoria_t *memoria);
void print_binary(memoria_t memoria);
operacion decodificar_operacion(int instruccion_hex);
void disassembler(int *ram, int *registro);
int dir_mem_abs_indirecto(int valorOp, int *registro, int *segfault);

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
void SLEN(int *a, int *b, memoria_t *mem);
void SMOV(int *a, int *b, memoria_t *mem);
void SCMP(int *a, int *b, memoria_t *mem);

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
void PUSH(int *a, int *b, memoria_t *mem);
void POP (int *a, int *b, memoria_t *mem);
void CALL(int *a, int *b, memoria_t *mem);

// cero operandos
void STOP(int *a, int *b, memoria_t *mem);
void RET (int *a, int *b, memoria_t *mem);
