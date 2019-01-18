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
  LinkedPair *pair = create_pair(key, value);
  unsigned int hashed_key = hash(key, ht->capacity);
  LinkedPair *destination = ht->storage[hashed_key];
  if (destination) { // if the slot is full
    while (destination) { // traverse until the first empty slot
      if (strcmp(destination->key, key) == 0) { // unless the key has been used already
        destination->value = pair->value; // then rewrite the value
        break;
      } else {
        if (destination->next) { // keep traversing if possible
          destination = destination->next;
        } else { // otherwise add to the list
          destination->next = pair;
          break;
        }
      }
    }
  } else { // if the slot is empty to begin with, fill it!
    ht->storage[hashed_key] = pair;
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

  unsigned int hashed_key = hash(key, ht->capacity);
  LinkedPair *destination = ht->storage[hashed_key];

  // traverse through linked list while keeping track of latest node
  LinkedPair *previous = NULL;
  while (strcmp(destination->key, key) != 0) {
    if (destination->next) {
      previous = destination;
      destination = destination->next;
    } else {
      break;
    }
  }

  if (strcmp(destination->key, key) == 0) {

    // keep a pointer to next for reassignment below
    LinkedPair *next = NULL;
    if (destination->next) {
      next = destination->next;
    }

    // destroy the ostensible focus of the remove function
    destroy_pair(destination);
    destination->next = NULL;
    destination = NULL;

    // four-way assignment of logical predecessors and successors
    if (next) {
      if (previous) {
        previous->next = next;
      } else {
        next = ht->storage[hashed_key];
      }
    } else {
      if (previous) {
        previous->next = NULL;
      } else {
        ht->storage[hashed_key] = NULL;
      }
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
  unsigned int hashed_key = hash(key, ht->capacity);
  LinkedPair *result = NULL;
  if (ht->storage[hashed_key]) { // is there anything at the hash mark?
    result = ht->storage[hashed_key];
  }
  if (result) { // if so ...
    while (strcmp(result->key, key) != 0) { // ... does the key match?
      if (result->next) { // traverse the list until the key matches ...
        result = result->next;
      } else { // ... or we run out of nodes
        return NULL;
      }
    }
    return result->value; // the key matches, so return the value
  }
  return NULL; // if there was no result, return NULL
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++) { // traverse each element in the backing array
    if (ht->storage[i]) {
      LinkedPair *pair = ht->storage[i];
      while (pair->next) { // traverse the list, destroying as we go
        LinkedPair *next = pair->next;
        destroy_pair(pair);
        pair->next = NULL;
        pair = next;
      }
      destroy_pair(pair); // destroy the last pair as well
      pair->next = NULL;
    }
  }
  free(ht->storage); // free everything else too
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
  HashTable *new_ht = create_hash_table(ht->capacity * 2); // new table twice as big

  for (int i = 0; i < ht->capacity; i++) { // reinsert everything
    if (ht->storage[i]) {
      LinkedPair *pair = ht->storage[i];
      while (pair->next) { // traverse the list
        LinkedPair *next = pair->next;
        hash_table_insert(new_ht, pair->key, pair->value);
        pair = next;
      }
      hash_table_insert(new_ht, pair->key, pair->value); // don't forget the final node
    }
  }

  destroy_hash_table(ht); // destroy the old table

  return new_ht; // and return the new
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
// printf("storage[0]->value: %s\n", ht->storage[0]->value);
  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
