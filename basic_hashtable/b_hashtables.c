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
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = malloc( sizeof(BasicHashTable) );
  ht->capacity = capacity; // --> Set capacity to given integer
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
  // 1. Get hash && create a pair between key and value passed in so we can put it in the ht later
  Pair *new_pair = create_pair(key, value);
  int hash_key = hash(key, ht->capacity);

  // 2. Check if we overwrite a value that has a different key
  if (ht->storage[hash_key] != NULL) {
    printf("Value Exists Already: Value already exists in a different key (overwriting value)...");
    destroy_pair(ht->storage[hash_key]);
  }

  // 3. Insert new Pair into our specified index
  ht->storage[hash_key] = new_pair;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  // 1. Get hash
  int hash_key = hash(key, ht->capacity);

  // 2. Check if the key exists, if so we delete it (get rid of key/value pair that is at current key, and then set to NULL)
  if (ht->storage[hash_key] != NULL) {
    destroy_pair(ht->storage[hash_key]);
    ht->storage[hash_key] = NULL;
  } else {
    printf("There is nothing at specified key");
    exit(1);
  }  
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  // 1. Get hash
  int hash_key = hash(key, ht->capacity);

  // 2. Base Case: If key is not found return NULL || Else: We return the value at that storage index
  if (ht->storage[hash_key]) {
    return ht->storage[hash_key]->value;
  }
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  // 1. Grab all pairs in storage && destroy the pair
  for (int i = 0; i < ht->capacity; i++) {
    destroy_pair(ht->storage[i]);    
  }
  
  // 2. We free the storage after all the pairs are set to NULL -> free the hash table
  free(ht->storage);
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
