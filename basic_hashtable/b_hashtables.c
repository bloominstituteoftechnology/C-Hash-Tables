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
  Pair **storage; //storage serves as array so it is a pointer to pointer
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
  //initiate hash table
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  //set hash table capacity
  ht->capacity = capacity;
  //allocate memory for storage
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
  //hash key to get index
  unsigned int index = hash(key, ht->capacity);
  //create a pair using key and value
  Pair *new_pair = create_pair(key, value);
  //store current pair
  Pair *pair = ht->storage[index];
  //check current index for null. if not null destroy what is there.
  if (ht->storage[index] != NULL)
  {
    //conditional statement comparing key and pair. strcmp returns 0. 0 if they are equal.
    if (strcmp(key, pair->key) != 0)
    {
      printf("Warning: overwriting variable");
      //collisions will go here
    }
    //delete existing pair
    destroy_pair(pair);
  }
  //update current position
  ht->storage[index] = new_pair;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  //get index using hash function
  unsigned int index = hash(key, ht->capacity);
  //create a pair
  Pair *pair = ht->storage[index];
  //check for null: do nothing if null. delete if there is something at that index.
  if (ht->storage[index] == NULL)
  {
    fprintf(stderr, "key does not exist");
    return;
  }
  destroy_pair(pair);
  ht->storage[index] = NULL;
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  //retrive value
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{

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


https://github.com/jamesroutley/write-a-hash-table

