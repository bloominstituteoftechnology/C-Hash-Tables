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
  // Allocate memory for the BasicHashTable struct
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));

  // Set initial value for capacity
  ht->capacity = capacity;

  // Allocate memory for storage
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
  // Create a hashed key
  unsigned int hashed_key = hash(key, ht->capacity);

  // If the space is occupied, free it and print a warning
  if (ht->storage[hashed_key] != NULL)
  {
    free(ht->storage[hashed_key]);
    printf("Previous value is being overwritten.\n");
  }

  // Create a new key/value pair and insert it into that space
  ht->storage[hashed_key] = create_pair(key, value);
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  // create a hashed key
  unsigned int hashed_key = hash(key, ht->capacity);

  // if that space is not empty
  if (ht->storage[hashed_key] != NULL)
  {
    // and if the keys match up
    if (strcmp(ht->storage[hashed_key]->key, key) == 0)
    {
      // free up the memory held in that space, set to NULL, and return
      free(ht->storage[hashed_key]);
      ht->storage[hashed_key] = NULL;
      return;
    }
  }

  // otherwise, print a key not found error and exit
  fprintf(stderr, "Key '%s' not found.\n", key);
  exit(1);
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  // create a hashed key
  unsigned int hashed_key = hash(key, ht->capacity);

  // if the space is not empty
  if (ht->storage[hashed_key] != NULL)
  {
    // and if the keys match
    if (strcmp(ht->storage[hashed_key]->key, key) == 0)
    {
      // return the value at that space
      return ht->storage[hashed_key]->value;
    }
  }

  // otherwise return NULL as the key will not have been found
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  // free up any and all key/value pairs in storage
  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      free(ht->storage[i]);
      ht->storage[i] = NULL;
    }
  }

  // free up storage
  if (ht->storage != NULL) free(ht->storage);

  // free up the hash table
  if (ht != NULL) free(ht);
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
