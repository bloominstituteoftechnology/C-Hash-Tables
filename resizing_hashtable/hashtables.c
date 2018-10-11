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
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair->next);
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
  unsigned int index = hash(key, ht->capacity);

  LinkedPair *current_pair = ht->storage[index];
  LinkedPair *last_pair;

  while (current_pair != NULL && strcmp(current_pair->key, key) != 0) {
    last_pair = current_pair;
    current_pair = last_pair->next;
  }

  if (current_pair != NULL) {
    current_pair->value = value;
  } else {
    LinkedPair *new_pair = create_pair(key, value);
    new_pair->next = ht->storage[index];
    ht->storage[index] = new_pair;
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

  LinkedPair *current_pair = ht->storage[index];
  LinkedPair *last_pair = current_pair;

  // current pair isn't null and our key is not yet matching the linkedpair that's found
  // this continues to set the previous pair as the current pair and then the current pair to the next pair ahead, traversing one at a time until NULL or until match is found
  while (current_pair != NULL && strcmp(current_pair->key, key) != 0) {
    last_pair = current_pair;
    current_pair = last_pair->next;
  } 

  // after breaking out of the while loop, it means that either it's NULL or that strcmp found a match
  if (current_pair != NULL) {
  // so while it's still not NULL, we check to see if the current pair is still set as the previous pair;
  // then we assign the index to the next index (or to NULL)
    if(current_pair == last_pair) {
      ht->storage[index] = current_pair->next;
    } 
    // then we check to see where the match occurs, and remove it safely by
    // setting the previous next to the current next
    // destroying the current
    // and then setting the index's value to NULL (like it's brand new)
    else if (strcmp(current_pair->key, key) == 0) {
      last_pair->next = current_pair->next;
      destroy_pair(current_pair);
      ht->storage[index] = NULL;
    }
    // otherwise, no match
    else {
        printf("No value found for that key. Not able to remove.");
        return NULL;
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
  unsigned int index = hash(key, ht->capacity);

  LinkedPair *current_pair = ht->storage[index];

  while(current_pair && strcmp(current_pair->key, key) != 0 ) {
    current_pair = current_pair->next;
  }

  if(current_pair != NULL && strcmp(current_pair->key, key) == 0) {
    return current_pair->value;
  }
  else {
    return NULL;
  }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++) {
    if(ht->storage[i]) {
      LinkedPair *current_pair = ht->storage[i];
      LinkedPair *last_pair;

      while(current_pair) {
        last_pair = current_pair;
        current_pair = current_pair->next;
        destroy_pair(last_pair);
      }
    } 
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
  HashTable *new_ht;
  
  new_ht = (HashTable *)malloc(sizeof(HashTable));
  new_ht->capacity = ht->capacity * 2;

  new_ht->storage = (LinkedPair**) calloc(new_ht->capacity, sizeof(LinkedPair*));

  for(int i = 0; i < ht->capacity; i++) {
    if(ht->storage[i]){
      LinkedPair *current_pair = ht->storage[i];
      hash_table_insert(new_ht, current_pair->key, current_pair->value);

      while(current_pair->next != NULL) {
        current_pair = current_pair->next;
        hash_table_insert(new_ht, current_pair->key, current_pair->value);
      }
      ht->storage[i] = NULL;
    }
  }

  destroy_hash_table(ht);

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
