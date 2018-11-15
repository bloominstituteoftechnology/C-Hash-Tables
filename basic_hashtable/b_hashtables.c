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
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  ht->capacity = capacity;
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
  printf("hash_table_insert: key = '%s'\n", key);
  
  Pair *newPair = malloc(sizeof(Pair));
  newPair = create_pair(key, value);
  int hashKey = hash(key, ht->capacity);
  printf("hash_table_insert: hashKey = %d\n\n",hashKey);

  ht->storage[hashKey] = newPair;
  free(newPair);
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  printf("hash_table_remove: key = %s\n", key);
  // for(int i = 0; key[i] != '\0'; i++){
  //   hash[i] = key[i];
  // }
  int hashKey; 
  hashKey = hash(key, ht->capacity);
  printf("hash_table_remove: hashKey = %d\n\n", hashKey);
  ht->storage[hash(key, ht->capacity)] = NULL;

}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  // int i;
  int hashKey = hash(key, ht->capacity);
  printf("hash_table_retrieve: hashKey = %d\n", hashKey);
  // while (key[i] != '\0'){
  //   hash[i] = key[i];
  //   i++;
  // }
  if(ht->storage[hashKey] == NULL){
    printf("hash_table_retrieve: is NULL\n");
    return NULL;
  } 
  char *ans;
  // while (ht->storage[hashKey][i])
  ans = ht->storage[hashKey]->value;
  printf("hash_table_retrieve: ans = %s\n\n", ans);
  return ans;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  int i = 0;
  while (i < ht->capacity){
    destroy_pair(ht->storage[i]);
    i++;
  }
  free(ht->storage);
  free(ht);
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("-test-\n%s\n", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("-test-\n...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
