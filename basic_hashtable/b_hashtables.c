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
   malloc 
    does not set the memory to zero 
    need for loop to set each spot to NULL
    1 parameter size of
   calloc 
    sets all slot of allocated memory to zero.
    2 parameters: number of slots & size

 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(Pair*));

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
  // unsigned int hash(char *str, int max);
  unsigned int hash_key = hash(key, ht->capacity);

  // obj
  if (ht->storage[hash_key]) {
      fprintf(stderr, "Warning: overwriting %s with a different key \n", key);
      destroy_pair(ht->storage[hash_key]);
  }
  ht->storage[hash_key] = create_pair(key, value);
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  unsigned int hash_key = hash(key, ht->capacity);
  destroy_pair(ht->storage[hash_key]);
  ht->storage[hash_key] = NULL;
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  // unsigned int hash(char *str, int max);
  unsigned int hash_key = hash(key, ht->capacity);

  if (!(ht->storage[hash_key])) {
    return NULL;
  }
  return ht->storage[hash_key]->value;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
    for(int i = 0; i < ht->capacity; i++) {
      if(ht->storage[i]) {
        destroy_pair(ht->storage[i]);
        free(ht->storage[i]);
      }
    }
    if (ht != NULL) {
      free(ht->storage);
      free(ht);
  }
}

/*****
 * Utility function to print an array.
 *****/
void ht_print(BasicHashTable *ht) {
  printf("[");
  for (int i = 0 ; i < ht->capacity ; i++) {
    printf("%p", ht->storage[i]);
    if (i != ht->capacity - 1) {
      printf(",");
    }
  }
  printf(", capacity: %d", ht->capacity);
  printf("]\n");
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);
  ht_print(ht);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("\n...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
