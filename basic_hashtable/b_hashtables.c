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

  //create a key value pair using create_pair
  Pair *pair = create_pair(key, value);
  //use the hash function to create a result
  unsigned int result = hash(key, ht->capacity); 
  //check if result is in storage and throw an error
  Pair *stored_pair = ht->storage[result];

  if (ht->storage[result] != NULL) {
    if(strcmp(key,stored_pair->key) != 0) {
    printf("You are overwriting an existing value: %s, %s with %s, %s\n",
      stored_pair->key, stored_pair->value, key, value);
  }
  else {
    destroy_pair(stored_pair);
  }
  }
  ht->storage[result] = pair;
}


/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  unsigned int result = hash(key, ht->capacity);
  if (ht->storage[result] == NULL){
    printf("That value does not exist");
  } else {
    destroy_pair(ht->storage[result]);
    ht->storage[result] = NULL;
  }
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  unsigned int result = hash(key, ht->capacity);

  if(ht->storage[result] == NULL){
  printf(" result not found");
  return NULL;
  }
    return ht->storage[result]->value;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  for(int i = 0; i<ht->capacity; i++){
    if (ht->storage[i] != NULL) {
      destroy_pair(ht->storage[1]);
    }
  }
  free(ht->storage);
  free(ht);
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);
  printf("=== Hash table created\n");

  hash_table_insert(ht, "line", "Here today...\n");
  printf("=== Insertion Successful\n");

  printf("%s", hash_table_retrieve(ht, "line"));
  printf("=== Retrieval Successful\n");

  hash_table_remove(ht, "line");
  printf("=== Removal Successful");

  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);
  printf("=== Destruction successful\n");

  return 0;
}
#endif
