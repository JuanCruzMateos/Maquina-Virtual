#include <stdlib.h>
#include <stdio.h>

// OBS: fseek(file, sizeof(int) * 15, SEEK_SET);
// offset es en bytes :: offset − This is the number of bytes to offset from whence.

int printable(int x) {
    return 32 <= x && x <= 126;
}

void p(FILE *file) {
    int i;
    fread(&i, sizeof(int), 1, file);
    printf("%08X\n", i);
}

int main(void) {
    FILE *file;
    char *fname = "../ASM/test.bin";
    int num, i = 0;

    file = fopen(fname, "rb");
    if (file == NULL)
        return -1;
    while (fread(&num, sizeof(int), 1, file) == 1) {
        printf("[%04d] %08X\n", i, num);
        if (printable(num)) {
            printf("%c\n", num);
        }
        i++;
    }
    // fseek(file, sizeof(int) * 15, SEEK_SET);
    // p(file);
    printf("%ld", sizeof(int));
    return 0;
}