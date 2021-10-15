#include <stdio.h>
#include <stdlib.h>

void printCell(int h);

int main(int argc, char const *argv[]) {
    int b;

    printCell(0x0012AF00);
    printf("\n");
    printCell(-123456);
    printf("\n");
    b = 0xFFFF;
    // b = b << 16;
    // b = b >> 16;
    printf("%d %08x\n", b, b);
    return 0;
}


void printCell(int h) {
    printf("%02x %02x %02x %02x", (h>>24)&0xFF, (h>>16)&0xFF, (h>>8)&0xFF, (h>>0)&0xFF);
}