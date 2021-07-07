#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Pair {
  char *key;
  char *value;
} Pair;

typedef struct BasicHashTable {
  int capacity;
  Pair **storage;
} BasicHashTable;

Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

void destroy_pair(Pair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
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
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(Pair));
  return ht;
}

void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  unsigned int index = hash(key, ht->capacity);
  if (ht->storage[index])
  {
    printf("WARNING: EXISTING VALUE OVERWRITTEN!\n");
    destroy_pair(ht->storage[index]);
  }
  ht->storage[index] = create_pair(key, value);
}

void hash_table_remove(BasicHashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  if (ht->storage[index])
  {
    destroy_pair(ht->storage[index]);
    ht->storage[index] = NULL;
  }
}

char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  if (ht->storage[index])
  {
    return ht->storage[index]->value;
  }
  return NULL;
}

void destroy_hash_table(BasicHashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++) {
    if (ht->storage[i]) {
      destroy_pair(ht->storage[i]);
    }
  }
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
