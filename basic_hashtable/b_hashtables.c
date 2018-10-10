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

Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = key;
  pair->value = value;

  return pair;
}

void destroy_pair(Pair *pair)
{
  if (pair != NULL) free(pair);
}

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

BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = calloc(capacity, sizeof(BasicHashTable));
  ht->capacity = capacity;
  return ht;
}


void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  puts("line 59: creating hash table");
  int hashed_index = hash(key, ht->capacity);
  *ht[hashed_index].storage = create_pair(key,value);
    // ht->storage[hashed_index] = create_pair(key,value);
  puts("creating hash table");
  printf("%s\n", ht->storage[hashed_index]->value);
}

void hash_table_remove(BasicHashTable *ht, char *key)
{
  puts("removing table from hash table");
  int hashed_index = hash(key, ht->capacity);
  if( ht->storage[hashed_index] != NULL) free(ht->storage[hashed_index]);
}


char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  puts("retrieving value from hash table");
  int hashed_index = hash(key, ht->capacity);
  return ht->storage[hashed_index]->value;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  if(ht != NULL) free(ht);
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
