#include "hashtable.h"
#define HASHSIZE 101

// Funcion de hash -> podria ser mejor
unsigned int hash(int key) {
    return key % HASHSIZE;
}


hash_table_t *hash_crear() {
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


void hash_guardar(hash_table_t *ht, int key, funct_ptr fptr) {
    // si key ya estaba pisar con nuevo valor
    unsigned int posicion = hash(key);
    nodo_hash *nodo;

    nodo = ht->datos[posicion];
    while (nodo != NULL && nodo->clave != key)
        nodo = nodo->sig;
    if (nodo != NULL)
        nodo->fptr = fptr;
    else {
        nodo = (nodo_hash *) malloc(sizeof(nodo_hash));
        nodo->clave = key;
        nodo->fptr = fptr;
        nodo->sig = ht->datos[posicion];
        ht->datos[posicion] = nodo;
        ht->tam += 1;
    }
}


funct_ptr hash_obtener(hash_table_t *ht, int key) {
    unsigned int posicion = hash(key);

    // if (ht == NULL)
    nodo_hash *entrada = ht->datos[posicion];
    while (entrada != NULL && entrada->clave != key) {
        entrada = entrada->sig;
    }
    return entrada == NULL ? NULL : entrada->fptr;
}
