#ifndef HASH_TABLE
#define HASH_TABLE

#include <stdlib.h>

// Nodo de la tabla de hash
typedef struct nodo_hash {
    int clave;
    void *val;
    struct nodo_hash *sig;
} nodo_hash;

// Tabla de hash: implementado como hash abierto para resolver las colisiones
typedef struct {
    unsigned int tam;
    nodo_hash **datos;
} hash_table_t;

// Crear tabla de hash
hash_table_t *hash_new();

// Guarda un par clave valor
// si key ya estaba en las key, se actualiza el par key-val con el nuevo valor de fptr
void hash_put(hash_table_t *ht, int key, void *val);

// Obtener valor de una determinada clave
// pre: tabla de hash creada (!= NULL)
// @return valor || NULL si no exite la clave en el diccionario
void *hash_get(hash_table_t *ht, int key);

// pre: tabla creada
// post: libera memoria
void hash_delete(hash_table_t *ht);

#endif // HASH_TABLE
