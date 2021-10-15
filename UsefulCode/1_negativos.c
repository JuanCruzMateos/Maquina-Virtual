#include <stdio.h>
#include <stdlib.h>

int main() {
    int i;

    printf("%10s %10s %10s %10s %10s %10s\n", "Decimal", "Hexa", "N. decimal", "N. Hexa", "comp1", "comp2");
    for (i = 0; i < 20; i++) {
        printf("%10d %10x %10d %10x %10x %10x\n", i, i, i*(-1), i*(-1), ~i, ~i+1);
    }
    return 0;
}