#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Hash table key/value pair with linked list pointer
 ****/
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/****
  Hash table with linked pairs
 ****/
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/****
  Create a key/value linked pair to be stored in the hash table.
 ****/
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = key;
  pair->value = value;
  pair->next = NULL;

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(LinkedPair *pair)
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
 ****/
HashTable *create_hash_table(int capacity)
{
  //very similar to the basic hash table set up.
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *));

  return ht;
}

/****
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 ****/
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  //create and hash, storage that accesses the has, and
  //create a pair. All of these are pointers.

  unsigned int index = hash(key, ht->capacity);
  LinkedPair *stored = ht->storage[index];
  LinkedPair *new_pair = create_pair(key, value);
  //check to see is there is anything in the hash
  if(stored == NULL){
    //if nothing exists set the hash to new_pair.
    ht->storage[index] = new_pair;
    //set up a else where a linked list is created.
  }else{
    //if the hash does have items then they will need a
    // collision preventor. LinkedPair enables the items 
    //with similar index's to be linked together.
    LinkedPair *linked = ht->storage[index];
    //run a while loop that will link the items together
    while(linked != NULL){
      linked = linked->next;
    }
    linked->next = new_pair;
    }
  }


/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  LinkedPair *stored = ht->storage[index];

  if(stored != NULL){
    destroy_pair(stored);
    stored = NULL;
  }
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  LinkedPair *current = NULL;
  char *value = NULL;

  if(ht->storage[index] == NULL){
    return value;
  }else{
    while(strcmp(current->key, key) != 0 && current->next != NULL){
      current = current->next;
    }
    value =current->value;
  }

  return value;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  for(int i = 0; i < ht->capacity; i++){
    destroy_pair(ht->storage[i]);
  }
  free(ht->storage);
  free(ht);
}

/****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
HashTable *hash_table_resize(HashTable *ht)
{
  //
  HashTable *new_ht = create_hash_table(ht->capacity * 2);
  //run a loop that goes through the capacity.
  for(int i = 0; i < ht->capacity; i++){
  //the new_ht takes in the previous storage.
    new_ht->storage[i] = ht->storage[i];
  }
  //destroy the previous hash cause it its capacity is old.
  destroy_hash_table(ht);
  //return the new and resized hash table.
  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
