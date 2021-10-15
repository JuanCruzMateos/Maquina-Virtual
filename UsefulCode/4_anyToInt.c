#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int anytoint(char *s, char **out);


int main(int argc, char *argv[]) {
    char *out = NULL;
    int i, aux = 0;

    for (i = 1; i < argc; i++) {
        aux = anytoint(argv[i], &out);
        printf("[%d]: %s => %d |%s|\n", i, argv[i], aux, out);
    }

    aux = 0x80000000;
    printf("%x %d", aux, aux);
    return 0;
}

int anytoint(char *s, char **out) {
    char *BASES = {"**$*****@*#*****%"};
    int base = 10;
    char *bp = strchr(BASES, *s);

    if (bp != NULL) {
        base = bp - BASES;
        s += 1;
    }
    return strtol(s, out, base);
}