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
    Double pointer indirection
    Pair*  holds a key-value pair (a pointer to a key-value pair)
    Pair** is an array of key-value pairs
    Storage (aka a bookshelf) holds a key-value pair or Pair*
 ****/
typedef struct BasicHashTable {
  int capacity;
  Pair **storage;     // Double pointer indirection 
} BasicHashTable;

/*** 
Deconstructed BasicHashTable

BasicHashTable = {
  capacity: 5;
  storage: *[
        * Pair = {
          *key: 'key';
          *value: 'value';
        },
        * Pair = {
          *key: 'key';
          *value: 'value';
        },
        * Pair = {
          *key: 'key';
          *value: 'value';
        },
  ]

}
***/

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
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
  void *calloc(size_t nitems, size_t size)
    - nitems − This is the number of elements to be allocated.
    - size − This is the size of elements.
  a = (int*)calloc(n, sizeof(int));
  Difference between malloc and calloc:
   malloc does not set the memory to zero 
   calloc sets allocated memory to zero.

 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = calloc(capacity, sizeof(BasicHashTable));
  ht->capacity = capacity;
  ht->storage = (Pair**)calloc(capacity, sizeof(Pair));

  // for loop using malloc: 
    // len = capacity 
    // ht->storage[i] = (Pair**)malloc(capacity * sizeof(Pair));

  return ht;
}


/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  // for(int i = 0; i < ht->capacity; i++) {
  //   if (key == ht->storage->key) {
  //     fprintf(stderr, "Warning: %d has already been used \n", key);
  //   }
  // }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{

}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  //   for(int i = 0; i < ht->capacity; i++) {
  //     destroy_pair(ht->storage[i]);
  //     free(ht->storage[i]);
  //   }
  //   if (ht != NULL) {
  //     free(ht->storage);
  //     free(ht);
  // }
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
