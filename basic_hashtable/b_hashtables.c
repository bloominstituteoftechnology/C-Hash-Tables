#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//https://repl.it/@codejoncode/Hash-Table-Assignment

/*Structures begin*/
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
/*Structures end*/


/*Function prototypes begin*/
Pair *create_pair(char *key, char *value);
void destroy_pair(Pair *pair);
unsigned int hash(char *str, int max);
BasicHashTable *create_hash_table(int capacity);
void hash_table_insert(BasicHashTable *ht, char *key, char *value);
void hash_table_remove(BasicHashTable *ht, char *key);
char *hash_table_retrieve(BasicHashTable *ht, char *key);
void destroy_hash_table(BasicHashTable *ht);
/*Function prototypes end*/



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


/*Functions begin*/
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

  ht->storage = calloc(ht->capacity, sizeof(Pair*));
  //^this is the line that sets the storage to null spaces. 

  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  Pair *pair = create_pair(key, value); 
  //^ create the pair to add to the hash table 

  //Get a hash 
  unsigned int hash_index = hash(key, ht->capacity);
  //Check if the space is taken up already will be NULL due to the use of calloc when creating the hash table storage 
  if (ht->storage[hash_index] != NULL){
    printf("\nERROR potential of losing previous omitted data! Overwriting\n");
    if(strcmp(ht->storage[hash_index]->key, key) != 0  ){
      printf("\nERROR-The key you previously set doesn't match the new key being set. Overwriting \n");
    }
  }
  //Overwrite/ set hash to equal the pair. 
  ht->storage[hash_index] = pair; 
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  //Get the hash 
  unsigned int hash_index = hash(key, ht->capacity);
  //Get the pair 
  Pair *pair = ht->storage[hash_index];
  //Destroy the pair and free the memory 
  destroy_pair(pair);
  //Set the space to NULL 
  // pair = NULL; 
  //Not sure if the above would do it or the below is required. 
  ht->storage[hash_index] = NULL; 
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  unsigned int hash_index = hash(key, ht->capacity);
   
  if(ht->storage[hash_index] != NULL){
    return ht->storage[hash_index]->value; 
  }
  
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  //Loop through the storage
  for(int i = 0; i<ht->capacity; i++){
    //grab each pair 
    Pair *pair = ht->storage[i]; 
    //only if the pair has a value does it need to be destroyed. 
    if (pair != NULL)
      destroy_pair(pair);
  }

  free(ht->storage);
  free(ht);
}