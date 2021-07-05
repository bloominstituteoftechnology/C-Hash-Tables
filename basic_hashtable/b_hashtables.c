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
  
  BasicHashTable *ht = NULL;
  ht = malloc(sizeof(BasicHashTable));
  ht->storage = calloc(capacity, sizeof(Pair));
  ht->capacity = capacity;
  printf("allocated storage >> %d", ht->storage);
  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  printf("cap >>>>> %d", ht->capacity);
  int hashed = hash(key, ht->capacity);
  while (ht->storage[hashed] != NULL){
    printf("index >> %d", hashed);
    hashed = (hashed + 1) % ht->capacity;
  }
  if (ht->storage[hashed] != NULL) {
    printf("The value of %s is being overwritten\n", key);
    destroy_pair(ht->storage[hashed]);
    ht->storage[hashed]->value = value;
  } else {
    Pair *nu_hash = malloc(sizeof(Pair));
    nu_hash = create_pair(key, value);
    ht->storage[hashed] = nu_hash;
  }
  return;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  int hashed = hash(key, ht->capacity);
  while (ht->storage[hashed] != NULL){
    hashed = (hashed + 1) & ht->capacity;
  }
  if(ht->storage[hashed]->key == key){
    destroy_pair(ht->storage[hashed]);
  }
  return;
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  int hashed = hash(key, ht->capacity);
  while (ht->storage[hashed] != NULL){
    hashed = (hashed + 1) & ht->capacity;
  }
  if(ht->storage[hashed]->key == key){
    return ht->storage[hashed]->value;
  }
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  for(int i=0; i<ht->capacity; i++){
    destroy_pair(ht->storage[i]);
  }
}


// #ifndef TESTING
int main(void)
{
  // printf("WHAT????????\n");
  struct BasicHashTable *ht = create_hash_table(16);
  printf("Capacity >>>>>> %d", ht->capacity);
  // hash_table_insert(ht, "line", "Here today...\n");

  // printf("%s", hash_table_retrieve(ht, "line"));

  // hash_table_remove(ht, "line");

  // if (hash_table_retrieve(ht, "line") == NULL) {
  //   printf("...gone tomorrow. (success)\n");
  // } else {
  //   fprintf(stderr, "ERROR: STILL HERE\n");
  // }

  // destroy_hash_table(ht);

  return 0;
}
// #endif
