// Punteros a funcion
#include <stdio.h>
#include <stdlib.h>

void mov(int *a, int *b);
void add(int *a, int *b);
void sub(int *a, int *b);

// llamada:
// func <cod operacion> <operando a> <operando b>
int main(int argc, char *argv[]) {
    int inst[] = {0, 0, 0};   // {operacion, operando a, operando b}
    char *ops[] = {"mov", "add", "sub"};
    void (*func[])(int *, int *) = {mov, add, sub};
    int i;

    for (i = 1; i < argc; i++) {
        inst[i - 1] = atoi(argv[i]);
    }

    printf("a=%d b=%d\n", inst[1], inst[2]);
    printf("%s a, b\n", ops[inst[0]]);

    (*func[inst[0]])(&inst[1], &inst[2]);

    printf("a=%d b=%d\n", inst[1], inst[1]);
    return 0;
}


void mov(int *a, int *b) {
    *a = *b;
}

void add(int *a, int *b) {
    *a += *b;
}

void sub(int *a, int *b) {
    *a -= *b;
}
