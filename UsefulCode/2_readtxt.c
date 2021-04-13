#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                             ->   INCORRECTO   <- 
*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    FILE *archivo;

    char linea[256];

    archivo = fopen("test.txt", "rt");
    if (archivo == NULL)
        exit(1);
    else {
        while (feof(archivo) == 0) {
            fgets(linea, 256, archivo);
            printf("%10d | %s", strlen(linea), linea);
        }
    }
    close(archivo);
    return 0;
}

/*////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                                             ->   CORRECTO   <-  ver libro c Ritchie
*/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main() {
    FILE *archivo;
    char linea[245];
    char *fname = "test.txt";

    if (archivo = fopen(fname, "rt") == NULL)
        return -1;
    while (fgets(linea, 256, archivo) != NULL) {
        printf("%10d | %s", strlen(linea), linea);
    }
    close(archivo);
    return 0;
}