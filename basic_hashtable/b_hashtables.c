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
  // Initiate a hash table
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  // Set the hash table's capacity to the passed in capacity
  ht->capacity = capacity;
  // Allocate memory for the storage
  ht->storage = calloc(capacity, sizeof(Pair));

  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  // unsigned int hash(char *str, int max) {}
  unsigned int index = hash(key, ht->capacity);
  
  // If there is stuff at this index, overwrite it
  if (ht->storage[index]) {
    fprintf(stderr, "Warning: overwriting previous value");
    free(ht->storage[index]);
  }
  
  // Otherwise, add it to the hash table
  ht->storage[index] = create_pair(key, value);
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  // Use given key to get the index
  unsigned int index = hash(key, ht->capacity);
  // Use the index to get the full Pair
  Pair *pair = ht->storage[index];
  
  // Check if the pair is null; if yes, throw error and return, otherwise destroy the pair
  if (pair == NULL) {
//    perror("Error removing pair: ");
    fprintf(stderr, "Error removing pair\n");
    return;
  }
  ht->storage[index] = NULL;
  destroy_pair(pair);
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  // Use given key to get the index
  unsigned int index = hash(key, ht->capacity);
  
  // Can't get the value directly?
//  // Use the index to get the value
//  char *value = ht->storage[index]->value;
//  
//  // Check if the value is null; if yes, return null, otherwise, return the value
//  if (value == NULL) {
//    return NULL;
//  }
//  return value;
  
  // This works...
  Pair *pair = ht->storage[index];
  
  if (pair == NULL) {
    return NULL;
  }
  return pair->value;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  // Loop the hash table up to the capacity and free all elements
  for (int index = 0; index < ht->capacity; index++) {
    free(ht->storage[index]);
  }
  
  // Also free the storage and hash table
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
