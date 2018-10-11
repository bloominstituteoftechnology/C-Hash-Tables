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
  // don't use because of danger of changing the content of the original pointer
  // pair->key = key;
  pair->key = strdup(key);
  // don't use because of danger of changing the content of the original pointer
  // pair->value = value;
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
    // because strdup, have to free key & value
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
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  // set the capacity
  ht->capacity = capacity;
  // initialize storage locations to 0
  // sizeof(Pair*) because Pair** is an array of pointers(addresses)
  // and we want the size of each "bucket", i.e. the address there
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
  unsigned int index = hash(key, ht->capacity);

  // it is OK to overwrite if the same key
  if (ht->storage[index] && (strcmp(key, ht->storage[index]->key) != 0))
  {
    printf("Overwriting the old value of %s\n", ht->storage[index]->value);
    destroy_pair(ht->storage[index]);
  }

  ht->storage[index] = create_pair(key, value);
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  if (ht->storage[index])
  {
    destroy_pair(ht->storage[index]);
    ht->storage[index] = 0;
  }
  else
  {
    printf("\nERROR! no value\n");
  }
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  // we get a seg fault if don't check this first
  if (ht->storage[index] != 0)
  {
    if (strcmp(ht->storage[index]->key, key) == 0)
    {
      return ht->storage[index]->value;
    }
  }
  return NULL;
}
/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++)
  {
    // destroy_pair checks for NULLs, so don't have to here
    destroy_pair(ht->storage[i]);
  }

  if (ht->storage != NULL)
    free(ht->storage);

  if (ht != NULL)
    free(ht);
}

#ifndef TESTING
int main(void)
{
  BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");
  // uncomment below to test duplicate inserts
  // hash_table_insert(ht, "line", "Here today...\n");
  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");
  // uncomment below to test duplicate removes
  // hash_table_remove(ht, "line");

  // should return null - uncomment below to test
  // printf("%s", hash_table_retrieve(ht, "line"));

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
