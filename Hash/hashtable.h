#include <stdlib.h>

// Puntero a funcion
typedef void (*funct_ptr)(int *, int *, int *);

// Nodo de la tabla de hash
typedef struct nodo_hash {
    int clave;
    funct_ptr fptr;
    struct nodo_hash *sig;
} nodo_hash;

// Tabla de hash: implementado como hash abierto para resolver las colisiones
typedef struct {
    unsigned int tam;
    nodo_hash **datos;
} hash_table_t;


// Crear tabla de hash
hash_table_t *hash_crear();

// Guarda un par clave valor
// si key ya estaba en las key, se actualiza el par key-val con el nuevo valor de fptr
void hash_guardar(hash_table_t *ht, int key, funct_ptr fptr);

// Obtener valor de una determinada clave
// @return valor || NULL si no exite la clave en el diccionario
funct_ptr hash_obtener(hash_table_t *ht, int key);
