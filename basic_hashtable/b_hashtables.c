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
  This Creates a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key); // when we strdup you need to free()
  pair->value = strdup(value); // when we strdup you need to free()
  return pair;
}

/****
  Use this function to safely destroy a hash table pair.
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
  BasicHashTable *ht;

  ht = malloc(sizeof(BasicHashTable));

  ht->capacity = capacity;

  ht->storage = calloc( capacity, sizeof(Pair*));

  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  // make an index
int index = hash(key, ht->capacity);

  // this is the warning if we overwrite.
  if (ht->storage[index] != NULL) {
    printf("warning: overwriting\n");
  // this is to successfully overwrite
    destroy_pair(ht->storage[index]);
  // once its destroyed this is what is going to be overwritten
    ht->storage[index] =  create_pair(key, value);
  
  } else {
    // if there is nothing in its place then this is going to be the
    // new code
    ht->storage[index] = create_pair(key, value);
  }
}

//  printf("printing the key: %s/n", key);

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/

// Find and remove the desired element
// Go to the next bucket
// If the bucket is empty, quit
// If the bucket is full, delete the element in that bucket
// and re-add it to the hash table using the normal means. 
// The item must be removed before re-adding,
// because it is likely that the item could be added back 
// into its original spot.
// Repeat steps
// remove a key from the array
void hash_table_remove(BasicHashTable *ht, char *key)
{

}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/

// destroy entire hashtable 
void destroy_hash_table(BasicHashTable *ht)
{
  int i = 0;
// loop through the array to get all the keys
for (i = 0; i < ht->capacity; i++) {
  // call the destroy each key it passes through
  destroy_pair(ht->storage[i]);
}
// deallocate it from memory so it can be used for other puprposes. 
free(ht->storage);
free(ht);
}
// do i need to call \0? 

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
