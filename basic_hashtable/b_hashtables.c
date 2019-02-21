#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
 Basic hash table key/value pair
****/
typedef struct Pair {
    char *key;
    char *value;
} Pair;

/****
 Basic hash table
****/
typedef struct BasicHashTable {
    int capacity;
    Pair **storage;
} BasicHashTable;

/****
 Create a key/value pair to be stored in the hash table.
****/
Pair *create_pair(char *key, char *value)
{
    Pair *pair = malloc(sizeof(Pair *));
    pair->key = strdup(key);
    pair->value = strdup(value);

    return pair;
}

/****
 Use this function to safely destroy a hashtable pair.
****/
void destroy_pair(Pair *pair)
{
    if (pair != NULL) {
        free(pair->key);
        free(pair->value);
        free(pair);
    }
}

/****
 djb2 hash function

Do not modify this!
****/
unsigned int hash(char *str, int max)
{
    unsigned long hash = 5381;
    int c;
    unsigned char * u_str = (unsigned char *)str;

    while ((c = *u_str++)) {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % max;
}


/****
 Fill this in.

All values in storage should be initialized to NULL
(hint: look up `calloc`)
****/
BasicHashTable *create_hash_table(int capacity)
{
    // allocate space for the single struct
    BasicHashTable *ht = malloc(sizeof(BasicHashTable));
    // define the structs capacity
    ht->capacity = capacity;
    // allocate enough space for the storage of Pair*'s
    ht->storage = calloc(capacity, sizeof(Pair *));
    return ht;
}

/****
 Fill this in.

If you are overwriting a value with a different key, print a warning.

Don't forget to free any malloc'ed memory!
****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
    // hash the key
    unsigned int i = hash(key, ht->capacity);
    // see if there's already a Pair* at hashed index
    if (ht->storage[i])
    {
        printf("Overwrote previous Pair '%s: %s'\n", ht->storage[i]->key, ht->storage[i]->value);
        // destroy the previous pair
        destroy_pair(ht->storage[i]);
    }
    // create a Pair* from the key and value and asign it to hashed index
    ht->storage[i] = create_pair(key, value);
}

/****
 Fill this in.

Don't forget to free any malloc'ed memory!
****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
    // hash the given key to an index
    unsigned int i = hash(key, ht->capacity);
    // check if storage holds a pair at that index
    if (!ht->storage[i])
    {
        fprintf(stderr, "Key not found\n");
        return;
    }
    // free the pair
    destroy_pair(ht->storage[i]);
    // null the storage at the hashed index
    ht->storage[i] = NULL;
}

/****
 Fill this in.

Should return NULL if the key is not found.
****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
    // hash the key to an index
    unsigned int i = hash(key, ht->capacity);
    // check if storage is empty
    if (!ht->storage[i])
    {
        fprintf(stderr, "Key not found\n");
        return NULL;
    }
    // return value of Pair* in storage at hashed index
    return  ht->storage[i]->value;
}

/****
 Fill this in.

Don't forget to free any malloc'ed memory!
****/
void destroy_hash_table(BasicHashTable *ht)
{
    // loop through all of storage
    for (int i = 0; i < ht->capacity; i++)
    {
        // check if a Pair* is present at the index
        if (ht->storage[i])
        {
            // destroy the Pair*
            destroy_pair(ht->storage[i]);
        }
    }
    // free storage
    free(ht->storage);
    // free struct
    free(ht);
}


#ifndef TESTING
int main(void)
{
    struct BasicHashTable *ht = create_hash_table(16);

    hash_table_insert(ht, "line", "Here today...\n");

    printf("%s", hash_table_retrieve(ht, "line"));

    hash_table_remove(ht, "line");

    if (hash_table_retrieve(ht, "line") == NULL) {
        printf("...gone tomorrow. (success)\n");
    } else {
        fprintf(stderr, "ERROR: STILL HERE\n");
    }

    destroy_hash_table(ht);

    return 0;
}
#endif
