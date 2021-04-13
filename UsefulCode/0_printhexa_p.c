#include <stdio.h>
#include <stdlib.h>


char *sprintx(long x);


int main(int argc, char const *argv[]) {
    system("cls");
    char *c;

    c = sprintx(0x0012AD00);
    printf("%s \n", c);
    c = sprintx(70000);
    printf("%s \n", c);
    return 0;
}


char *sprintx(long x) {
    static char str[21];
    char aux[11];
    int i, j;

    sprintf(aux, "%08x", x);
    for (j = i = 0; i < 8; i += 2, j += 3) {
        str[j] = aux[i];
        str[j + 1] = aux[i + 1];
        str[j + 2] = ' ';
    }
    str[j] = '\0';
    return (str);
}