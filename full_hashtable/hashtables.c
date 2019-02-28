#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/****
  Hash table key/value pair with linked list pointer
 ****/
typedef struct LinkedPair
{
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/****
  Hash table with linked pairs
 ****/
typedef struct HashTable
{
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
  if (pair != NULL)
  {
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
  unsigned char *u_str = (unsigned char *)str;

  while ((c = *u_str++))
  {
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
  HashTable *ht = malloc(sizeof(HashTable)); // allocate memory for a hash table;

  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *)); // allocate the memory storage for a struct LinkedPair pointer;

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
  unsigned int target_index = hash(key, ht->capacity); // creates an array index for node to be inserted into
  LinkedPair *new_pair = create_pair(key, value);      // creates a new node for insertion

  if (ht->storage[target_index] == 0)
  { // if the current index is empty, insert new_pair
    ht->storage[target_index] = new_pair;
  }
  else
  {
    while (ht->storage[target_index] != 0)
    { // if the current index is not empty, 3 things can happens
      // 1. check if the keys are the same, then overwrite existing value with the new value
      if (strcmp(ht->storage[target_index]->key, key) == 0)
      { // == or != compares base addresses, strcmp to compare values
        ht->storage[target_index]->value = value;
        break;
      }
      // 2. check if keys are different AND there's an empty "next" slot, insert new pair there
      else if (strcmp(ht->storage[target_index]->key, key) != 0 && ht->storage[target_index]->next == NULL)
      {
        ht->storage[target_index]->next = new_pair;
        break;
      }
      // 3. If neither of the terminating conditionals activates, must mean that it's a new linked list node to be attached
      // continue with while loop until there are matching keys or there's an empty "next" slot
      ht->storage[target_index] = ht->storage[target_index]->next;
    }
  }

  /****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
  void hash_table_remove(HashTable * ht, char *key)
  {
  }

  /****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
  char *hash_table_retrieve(HashTable * ht, char *key)
  {
    return NULL;
  }

  /****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
  void destroy_hash_table(HashTable * ht)
  {
  }

  /****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
  HashTable *hash_table_resize(HashTable * ht)
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
