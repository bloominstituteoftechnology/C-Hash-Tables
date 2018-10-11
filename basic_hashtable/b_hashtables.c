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
  pair->key = key;
  pair->value = value;

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL) free(pair);
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
  BasicHashTable *ht;
  //ht = NULL;
  ht = malloc(sizeof(BasicHashTable));

  if (capacity < 1)
  {
    return NULL;
  }

  if ((ht = calloc(0, sizeof(BasicHashTable))) == NULL)
  {
    return NULL;
  }

  // if ((ht->storage = malloc((sizeof(Pair) * capacity))) == NULL)
  // {
  //   return NULL;
  // }

   ht->storage = (Pair**) calloc(capacity, sizeof(Pair*)); // 16 Null

  for (int i=0; i < capacity; i++)
  {
    ht->storage[i] = NULL;
  }

  ht->capacity = capacity;

  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  unsigned int hash_index = hash(key, ht->capacity);

  if (ht->storage[hash_index] != NULL)
  {
    printf("WARNING: Index will be overwritten!\n");
    destroy_pair(ht->storage[hash_index]);
    //free(ht->storage[hash_index]);
  }

  ht->storage[hash_index] = create_pair(key, value);
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  unsigned int hash_index = hash(key, ht->capacity);

  if (ht->storage[hash_index] != NULL)
  {
    destroy_pair(ht->storage[hash_index]);
    ht->storage[hash_index] = NULL;
    free(ht->storage[hash_index]);
  } else
  {
    printf("Invalid. Value does not exist\n");
  }
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  unsigned int hash_index = hash(key, ht->capacity);

  if (ht->storage[hash_index] != NULL)
  {
    // if (strcmp(ht->storage[hash_index]->key, key) == 1)
    // {
    //   return ht->storage[hash_index]->value;
    // }
    // else
    // {
    //   return NULL;
    // }
    return ht->storage[hash_index]->value;
  }
  
  else
  {
    printf("Invalid: Index not found \n");
    return NULL;
  }
  
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  for (int i =0; i< ht->capacity; i++)
  {
    if (ht->storage[i] != 0)
    {
      destroy_pair(ht->storage[i]);
    } else
    {
      destroy_pair(ht->storage[0]);
    }
  }

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
