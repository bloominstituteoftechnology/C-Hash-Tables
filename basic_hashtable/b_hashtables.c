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
  // creating index
  unsigned int index = hash(key, ht->capacity);
  // creating pair
  Pair *pair = create_pair(key, value);
  // creating pointer to storage at hased index
  Pair *old_pair_at_index = ht->storage[index];

  if (old_pair_at_index != NULL) {
    // If you are overwriting a value with a different key, print a warning.
    if (strcmp(key, old_pair_at_index->key) != 0) {
      fprintf(stderr, "Overwritting data.");
    }
    // Don't forget to free any malloc'ed memory!
    destroy_pair(old_pair_at_index);
  }
  // inserting new pair
  ht->storage[index] = pair;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  // creating index
  unsigned int index = hash(key, ht->capacity);
  // find pair at that index
  Pair *pair_to_remove = ht->storage[index];
  // destroy pair
  destroy_pair(pair_to_remove);
  // set that storage index to NULL
  ht->storage[index] = NULL;
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  // creating index from key
  unsigned int index = hash(key, ht->capacity);
  // grabbing pair from storage
  Pair *pair_to_return = ht->storage[index];
  // Should return NULL if the key is not found.
  if (pair_to_return == NULL) {
    fprintf(stderr, "Key does not exist");
    return NULL;
  }
  // return the value from pair at that storage index
  return pair_to_return->value;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  // loop to free all of the pairs
  for (int i = 0; i < ht->capacity; i++) {
    destroy_pair(ht->storage[i]);
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
