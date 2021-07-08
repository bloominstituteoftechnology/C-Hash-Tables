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
      BasicHashTable *ht;
      ht = malloc(sizeof(BasicHashTable));
      ht->capacity = capacity;
      //void *calloc(size_t nunber_of_items, size_t size)
      ht->storage = calloc(capacity, sizeof(Pair *));
      printf("\nBasic hash table..\n");

      return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  printf("\nIN INSERT....\n");
      //first generate a HASH-KEY for given key using hash function which will be an index.
      int hash_key_index = hash(key, ht->capacity);
      //check for generated hash_key already value present?
      if(ht->storage[hash_key_index] != NULL) {
            //if yes print warning of overwriting.
            printf("\nOVERWRITING.........\n");
            //Don't forget to free any malloc'ed memory ..create_pair()
            destroy_pair(ht->storage[hash_key_index]);
      }
      //insert given key -- value pair
      ht->storage[hash_key_index] = create_pair(key, value);
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
      //generate hash_key for given key 
      int hash_key_index = hash(key, ht->capacity);
      //check for key-value
      if(ht->storage[hash_key_index] != NULL) {
            ht->storage[hash_key_index] = NULL;
            //Don't forget to free any malloc'ed memory.
            destroy_pair(ht->storage[hash_key_index]);
      } 
      else {
            //if key does not exist through an error.
            fprintf(stderr, "\nKEY DOES NOT EXIST...\n");
            return;
      } 

}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
      //generate hash_key for given key 
      int hash_key_index = hash(key, ht->capacity);
      //check for key present 
      if(ht->storage[hash_key_index]) {
            return ht->storage[hash_key_index]->value;
      }
      else {
            fprintf(stderr, "\nKEY DOES NOT EXIST...\n");
            return NULL;
      }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
      //iterate through capacity 
      //check  for key-value presence
          //destroy key-value pair first
      //free storage and hash-table created.
      for(int i = 0; i < ht->capacity; i++) {
            if(ht->storage[i] != NULL) {
                  destroy_pair(ht->storage[i]);
            }
      }

      free(ht->storage);
      free(ht);
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16); //Basic hash table..

  hash_table_insert(ht, "line", "Here today...\n");//IN INSERT

  printf("%s", hash_table_retrieve(ht, "line")); // Here today

  hash_table_remove(ht, "line"); //line :here today removed..

  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
