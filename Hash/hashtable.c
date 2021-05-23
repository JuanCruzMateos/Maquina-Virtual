#include "hashtable.h"
#define HASHSIZE 101

// Funcion de hash -> podria ser mejor
unsigned int hash(int key) {
    return key % HASHSIZE;
}

// crea la tabla de hash
hash_table_t *hash_new() {
    unsigned int i;
    hash_table_t *ht;

    ht = (hash_table_t *) malloc(sizeof(hash_table_t));
    if (ht == NULL)
       return NULL;
    ht->tam = 0;
    ht->datos = (nodo_hash **) malloc(sizeof(nodo_hash *) * HASHSIZE);
    if (ht->datos == NULL)
       return NULL;
    for (i = 0; i < HASHSIZE; i++)
        ht->datos[i] = NULL;
    return ht;
}

// guarda una par clave-valor
// pisa antiguo valor
void hash_put(hash_table_t *ht, int key, void *val) {
    unsigned int posicion = hash(key);
    nodo_hash *nodo;

    nodo = ht->datos[posicion];
    while (nodo != NULL && nodo->clave != key)
        nodo = nodo->sig;
    if (nodo != NULL)
        nodo->val = val;
    else {
        nodo = (nodo_hash *) malloc(sizeof(nodo_hash));
        nodo->clave = key;
        nodo->val = val;
        nodo->sig = ht->datos[posicion];
        ht->datos[posicion] = nodo;
        ht->tam += 1;
    }
}

// devuelve el valor asociado a una clave
void *hash_get(hash_table_t *ht, int key) {
    unsigned int posicion = hash(key);
    nodo_hash *entrada = ht->datos[posicion];

    while (entrada != NULL && entrada->clave != key) {
        entrada = entrada->sig;
    }
    return entrada == NULL ? NULL : entrada->val;
}

// destruye la tabla de hash liberando memoria
void hash_delete(hash_table_t *ht) {
    nodo_hash *nodo, *elim;
    int i;

    for (i = 0; i < HASHSIZE; i++) {
        while (ht->datos[i] != NULL) {
            elim = ht->datos[i];
            ht->datos[i] = ht->datos[i]->sig;
            free(elim);
        }
    }
    free(ht->datos);
    free(ht);
}
