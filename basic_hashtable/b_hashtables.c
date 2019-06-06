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
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  ht->capacity = capacity;
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
  // find new hashed index from key and capacity
  unsigned int new_index = hash(key, ht->capacity);
  // create new pair
  Pair *pair = create_pair(key, value);
  // determine if key/pair already exists
  Pair *stored_pair = ht->storage[new_index];
  if (ht->storage[new_index] != NULL)
  {
    if (strcmp(ht->storage[new_index]->key, key) == 0)
    {
      printf("Destroy key: %s\n", ht->storage[new_index]->key);
      destroy_pair(stored_pair);
    }
    else
    {
      printf("Keys do not match.\n");
    }
  }
  else
  {
    printf("Key is doesn't exist: NULL.\n");
  }
  // place new pair in hashed index of hash table
  ht->storage[new_index] = pair;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  // find hashed index
  int index = hash(key, ht->capacity);
  // destroy pair via hashed index
  if (ht->storage[index] != NULL && strcmp(ht->storage[index]->key, key) == 0)
  {
    // remove pair
    destroy_pair(ht->storage[index]);
    // set storage index to null
    ht->storage[index] = NULL;
  }
  else
  {
    // print error
    printf(stderr, "Unable to remove entry with key: %s\n", key);
  }
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  // find hashed index value from key
  int index = hash(key, ht->capacity);
  // if key value / indexed storage value is null return null else return value
  if (ht->storage[index] != NULL && strcmp(ht->storage[index]->key, key) == 0)
  {
    return ht->storage[index]->value;
  }
  printf(stderr, "Unable to remove entry with key: %s\n", key);
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  // loop through storage and destrpy pairings and then free storage per index
  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {

      destroy_pair(ht->storage[i]);
    }

    // free(ht->storage[i]); not necessary already handled by destroy_pair();
  }
  // free overall storage
  free(ht->storage);
  // free hash table structure
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
