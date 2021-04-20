#include <stdio.h>
#include <stdlib.h>
#define CANT_REG 16
#define CANT_RAM 4096

// registros - declaro
extern int registro[CANT_REG];
// ram - declaro
extern int ram[CANT_RAM];

// arreglos de punteos a funciones -> instrucciones
extern void (*instruccion_dos_op[]) (int *, int *);
extern void (*instruccion_un_op[]) (int *);

typedef enum { DOS_OP_REG_IN, DOS_OP_REG_REG, DOS_OP_REG_DIR,
               DOS_OP_DIR_IN, DOS_OP_DIR_REG, DOS_OP_DIR_DIR,
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

void load_ram(FILE *arch, int *mem, int *DS);
void print_binary(int *ram, int DS);
operacion decodificar_operacion(int instruccion_hex);
void print_registros();

void MOV(int *a, int *b);
void ADD(int *a, int *b);
void SUB(int *a, int *b);
void SWAP(int *a, int *b);
void MUL(int *a, int *b);
void DIV(int *a, int *b);
void CMP(int *a, int *b);
void SHL(int *a, int *b);
void SHR(int *a, int *b);
void AND(int *a, int *b);
void OR(int *a, int *b);
void XOR(int *a, int *b);

void SYS(int *a);
void JMP(int *a);
void JZ(int *a);
void JP(int *a);
void JN(int *a);
void JNZ(int *a);
void JNP(int *a);
void JNN(int *a);
void LDL(int *a);
void LDH(int *a);
void RND(int *a);
void NOT(int *a);

void STOP();
