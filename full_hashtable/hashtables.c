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
  if (destination) {
    while (destination) {
      if (strcmp(destination->key, key) == 0) {
        LinkedPair *next = destination->next;
        printf("rewrote old value of key: %s\n", destination->key);
        destination->key = pair->key;
        destination->value = pair->value;
        destination->next = next;
        printf("new value of key: %s is value: %s\n", destination->key, destination->value);
        break;
      } else {
        if (destination->next) {
          printf("cycled through\n");
          destination = destination->next;
        } else {
          printf("assigned value to a next\n");
          destination->next = pair;
          printf("key: %s, value: %s\n", destination->next->key, destination->next->value);
          break;
        }
      }
    }
  } else {
    printf("no value for key; assigned in place\n");
    ht->storage[hashed_key] = pair;
    // printf("key: %s, value: %s\n", destination->key, destination->value);
    printf("key: %s, value: %s\n", ht->storage[hashed_key]->key, ht->storage[hashed_key]->value);
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
  printf(">>>starting to destroy with key: %s\n", key);
  unsigned int hashed_key = hash(key, ht->capacity);
  LinkedPair *destination = ht->storage[hashed_key];
  LinkedPair *previous = NULL;

  while (strcmp(destination->key, key) != 0) {
    printf("inside while loop\n");
    if (destination->next) {
      printf("key mismatch; traversing linked list\n");
      printf(">> setting previous\n");
      previous = destination;
      destination = destination->next;
    } else {
      printf("about to break\n");
      break;
    }
  }

  if (strcmp(destination->key, key) == 0) {
    printf("match found, checking for next\n");
    LinkedPair *next = NULL;

    if (destination->next) {
      // printf("<< setting previous\n");
      // previous = destination;
      printf(">> setting next in reserve\n");
      next = destination->next;
    }

    printf("destroying pair: key %s value %s\n", destination->key, destination->value);
    destroy_pair(destination);
    destination = NULL;


    if (next) {
      if (previous) {
        previous->next = next;
      } else {
        next = ht->storage[hashed_key];
      }
    }
    // 
    // destination = NULL;
    // if (next) {
    //   printf("bringing next in from reserve\n");
    //   destination = next;
    // }
    // else {
    //   printf("setting destination to NULL\n");
    //   destination = NULL;
    // }
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
  if (ht->storage[hashed_key]) {
    result = ht->storage[hashed_key];
    printf("first result >key: %s, >value: %s\n", result->key, result->value);
  }
  if (result) {
    while (strcmp(result->key, key) != 0) {
      if (result->next) {
        printf("cycled through nexts\n");
        result = result->next;
      } else {
        break;
      }
    }
    if (strcmp(result->key, key) == 0) {
      printf("found matching key: %s\n", result->key);
      return result->value;
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
  for (int i = 0; i < ht->capacity; i++) {
    if (ht->storage[i]) {
      LinkedPair *pair = ht->storage[i];
      while (pair->next) {
        LinkedPair *next = pair->next;
        printf("about to destroy pair key: %s\n", pair->key);
        destroy_pair(pair);
        printf("cycling to next\n");
        pair = next;
      }
      printf("about to destroy pair key: %s\n", pair->key);
      destroy_pair(pair);
    } else  {
      printf("nothing inside\n");
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
  HashTable *new_ht = create_hash_table(ht->capacity * 2);

  for (int i = 0; i < ht->capacity; i++) {
    if (ht->storage[i]) {
      LinkedPair *pair = ht->storage[i];
      while (pair->next) {
        LinkedPair *next = pair->next;
        printf("about to reassign pair key: %s\n", pair->key);
        hash_table_insert(new_ht, pair->key, pair->value);
        printf("cycling to next\n");
        pair = next;
      }
      printf("about to reassign pair key: %s\n", pair->key);
      hash_table_insert(new_ht, pair->key, pair->value);
    } else  {
      printf("nothing inside\n");
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
// printf("storage[0]->value: %s\n", ht->storage[0]->value);
  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
