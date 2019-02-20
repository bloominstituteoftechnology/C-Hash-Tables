#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****
  Basic hash table key/value pair
 ****/
typedef struct Pair
{
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable
{
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
  if (pair != NULL)
  {
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
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++))
  {
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
  // Create a new hash table named ht
  BasicHashTable *ht;
  // Malloc the size of hash table
  ht = malloc(sizeof(BasicHashTable));
  // set capacity to passed in capacity
  ht->capacity = capacity;
  // use calloc to set storage bites and set all to 0
  ht->storage = calloc(capacity, sizeof(Pair));
  // return the new Hash table
  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  // Create a pair by sending the key and value
  Pair *pair = create_pair(key, value);
  // create a key by hashing
  unsigned int hashed_key = hash(key, ht->capacity);
  // set the storage key and value
  ht->storage[hashed_key] = pair;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  // create a key by hashing
  unsigned int hashed_key = hash(key, ht->capacity);
  // free up the alloc memory
  free(ht->storage[hashed_key]);
  // set the value to null
  ht->storage[hashed_key] = NULL;
}

/****
  Fill this in.
  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  // create a key by hasing
  unsigned int hashed_key = hash(key, ht->capacity);
  // Map that pair into a result
  Pair *result = ht->storage[hashed_key];
  // if result, return the value
  if (result)
  {
    return result->value;
  }
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  // loops through the storage and uses destroy pair
  for (int i = 0; i < ht->capacity; i++)
  {
    // destroys each Pair struct at index
    destroy_pair(ht->storage[i]);
  }
  // frees alloc memory of storage
  free(ht->storage);
  // frees alloc memory of Hash Table
  free(ht);
}

#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL)
  {
    printf("...gone tomorrow. (success)\n");
  }
  else
  {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
