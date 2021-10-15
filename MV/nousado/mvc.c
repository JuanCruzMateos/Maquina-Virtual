//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Traductor Maquina Virtual                                                           //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define LEN 256


char *toUpper(char *str);


int main(int argc, char const *argv[]) {

    FILE *asmfile;
    const char *fname;
    const char *out;
    char line[LEN];
    int linenumber = 0;
    int flag = 0;

    if (argc < 5) {
        fname = argv[1];
        out = argv[2];
        // printf("asm = %s\n", fname);
        // printf("out = %s\n", out);
        if (argc == 4 && strcmp(argv[3], "-o") == 0) {
            flag = 1;
        }
        asmfile = fopen(fname, "rt");
        if (asmfile == NULL)
            return -1;
        else {
            while (fgets(line, LEN, asmfile) != NULL) {
                if (strcmp(line, " \n") != 0) {
                    linenumber += 1;
                    if (flag)
                        printf("[%04d] %s", linenumber, line);
                }
            }
            fclose(asmfile);
        }
    }
    return 0;
}


char *toUpper(char *str) {
    static char upper[LEN];
    int i = 0;

    memset(upper, 0, LEN);
    while (*str != '\0') {
        upper[i++] = *str & ~(' ');
        str += 1;
    }
    return upper;
}
