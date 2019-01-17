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
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(LinkedPair *pair)
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
 ****/
HashTable *create_hash_table(int capacity)
{
  HashTable *ht;
  ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair));

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
  unsigned int hash_index = hash(key, ht->capacity);
  LinkedPair *linked_pair = create_pair(key, value);
  // if  a LinkedPair is already stored at this index
  if (ht->storage[hash_index] != NULL){
    // if this LinkedPair has the same key, overwrite the value only
    // no need to modify key or the next pair. key and current's pair LinkedPair may remain as they are.
    if (strcmp(ht->storage[hash_index]->key, key) == 0){
      printf("Warning: overwriting previous Pair with current Pair");
      ht->storage[hash_index]->value = value;
    }
    // if keys are not the same, check if there's a linked pair
    else if (ht->storage[hash_index]->next == NULL) {
      // there is no Linked Pair, so link it
      ht->storage[hash_index]->next = linked_pair; //???
    }
    // there is a Linked Pair. find the next unlinked pair, and assign it there
    else {
      LinkedPair *current_pair = ht->storage[hash_index]->next;
      while (current_pair->next != NULL){
        current_pair = current_pair->next;
      } //when current_pair's next value is finally NULL, assign the linked_pair to it
      current_pair->next = linked_pair;
    }

  }
  //the current index is emtpy. assign the linked_pair
  else {
    ht->storage[hash_index] = linked_pair;
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
  unsigned int hash_index = hash(key, ht->capacity);

  LinkedPair *current_pair = ht->storage[hash_index];

  if (current_pair == NULL){
    printf("nothing to remove\n");
  } else {
    //while keys do not match and there's still a next pair
    while (strcmp(current_pair->key, key) != 0 && current_pair->next != NULL){
      current_pair = current_pair->next;
    }
    if (strcmp(current_pair->key, key) == 0) {
      destroy_pair(current_pair);
      current_pair = NULL;
    } else {
    printf("nothing to remove\n");
    }
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
  unsigned int hash_index = hash(key, ht->capacity);
  LinkedPair *current_pair = ht->storage[hash_index];
  while (current_pair != NULL) {
    if (strcmp(current_pair->key, key) == 0){
      return current_pair->value;
    } else {
      current_pair = current_pair->next;
    }
  }

  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  for (int i=0; i<ht->capacity; i++){
    LinkedPair *current_pair = ht->storage[i];
    LinkedPair *next_pair = ht->storage[i]->next;
    while (current_pair->next != NULL){
      next_pair = current_pair->next;
      destroy_pair(current_pair);
      current_pair = next_pair;
    }
    destroy_pair(current_pair);
  }
}

/****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht;

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
