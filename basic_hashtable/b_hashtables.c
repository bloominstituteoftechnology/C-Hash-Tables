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
  
  
  ht->storage = calloc(capacity, sizeof(Pair));
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
  int i = hash(key, ht->capacity-1); // created the key
  Pair * np = create_pair(key, value); // created pair to be inserted

  if ( ht->storage[i] == NULL ) {
    printf("Succesful!");
    ht->storage[i] = np;
  } else {
    free(ht->storage[i]);
    fprintf(stderr, "Warning, you've overwritten a value");
    ht->storage[i] = np;
  }

}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  int i = hash(key, ht->capacity-1); // created the key
  printf("%s %s \n", ht->storage[i]->key, key );
  if ( ht->storage[i]->key == key) {
    destroy_pair(ht->storage[i]);
    ht->storage[i] = NULL;
  } else {
    printf("That key is invalid");
  }

}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  int i = hash(key, ht->capacity-1);
  if (ht->storage[i] == NULL) {
    printf("Index is empty");
    return NULL;
  } else {
    return ht->storage[i]->value;
  }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  for ( int i = 0; i < ht->capacity; i++ ) {
    if ( ht->storage[i] != NULL ) {
      destroy_pair(ht->storage[i]);
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
