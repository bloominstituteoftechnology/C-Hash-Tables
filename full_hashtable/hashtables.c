
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/minunit.h"

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
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(ht->capacity, sizeof(LinkedPair *));
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

    while (current_pair != NULL && strcmp(current_pair->key, key) != 0)
    {
        last_pair = current_pair;
        current_pair = last_pair->next;
    }

    if (current_pair != NULL)
    {
        current_pair->value = strdup(value);
    }
    else
    {
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
    LinkedPair *previous_pair = NULL;

    while (current_pair != NULL && strcmp(current_pair->key, key) != 0)
    {
        previous_pair = current_pair;
        current_pair = current_pair->next;
    }

    if (current_pair == NULL)
    {

        fprintf(stderr, "Unable to remove entry with key: %s\n", key);
    }
    else
    {

        if (previous_pair == NULL)
        { // Removing the first element in the Linked List
            ht->storage[index] = current_pair->next;
        }
        else
        {
            previous_pair->next = current_pair->next;
        }

        destroy_pair(current_pair);
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
  unsigned int index = hash(key, ht->capacity); //get has

  LinkedPair *current_pair = ht->storage[index]; //use hash to set current pair

  while (current_pair != NULL) //loop as long as current pair exists
  {
    if (strcmp(current_pair->key, key) == 0) // if key is the same at the index change value;
    {
      return current_pair->value; //
    }
    current_pair = current_pair->next; //
  }
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht) //How to kill capacity->next?
{
  if (ht != NULL)
  {
    for (int i = 0; i < ht->capacity; i++)
    {
      if (ht->storage[i] != NULL)
      {
        destroy_pair(ht->storage[i]);
      }
    }
    free(ht->storage);
    free(ht);
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
  HashTable *new_ht = create_hash_table(2 * ht->capacity); //make ht with double capacity

  LinkedPair *current_pair;
  for (int i = 0; i < ht->capacity; i++)
  { //loop through every index
    current_pair = ht->storage[i];
    while (current_pair != NULL)
    {
      hash_table_insert(new_ht, current_pair->key, current_pair->value);
      current_pair = current_pair->next;
    }
  }
  destroy_hash_table(ht);

  return new_ht;
}

#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(8);

  char *return_value = hash_table_retrieve(ht, "key-0");
  mu_assert(return_value == NULL, "Initialized value is not NULL");

  hash_table_insert(ht, "key-0", "val-0");
  hash_table_insert(ht, "key-1", "val-1");
  hash_table_insert(ht, "key-2", "val-2");
  hash_table_insert(ht, "key-3", "val-3");
  hash_table_insert(ht, "key-4", "val-4");
  hash_table_insert(ht, "key-5", "val-5");
  hash_table_insert(ht, "key-6", "val-6");
  hash_table_insert(ht, "key-7", "val-7");
  hash_table_insert(ht, "key-8", "val-8");
  hash_table_insert(ht, "key-9", "val-9");

  return_value = hash_table_retrieve(ht, "key-0");
  mu_assert(strcmp(return_value, "val-0") == 0, "Value is not stored correctly");
  return_value = hash_table_retrieve(ht, "key-1");
  mu_assert(strcmp(return_value, "val-1") == 0, "Value is not stored correctly");
  return_value = hash_table_retrieve(ht, "key-2");
  mu_assert(strcmp(return_value, "val-2") == 0, "Value is not stored correctly");
  return_value = hash_table_retrieve(ht, "key-3");
  mu_assert(strcmp(return_value, "val-3") == 0, "Value is not stored correctly");
  return_value = hash_table_retrieve(ht, "key-4");
  mu_assert(strcmp(return_value, "val-4") == 0, "Value is not stored correctly");
  return_value = hash_table_retrieve(ht, "key-5");
  mu_assert(strcmp(return_value, "val-5") == 0, "Value is not stored correctly");
  return_value = hash_table_retrieve(ht, "key-6");
  mu_assert(strcmp(return_value, "val-6") == 0, "Value is not stored correctly");
  return_value = hash_table_retrieve(ht, "key-7");
  mu_assert(strcmp(return_value, "val-7") == 0, "Value is not stored correctly");
  return_value = hash_table_retrieve(ht, "key-8");
  mu_assert(strcmp(return_value, "val-8") == 0, "Value is not stored correctly");
  return_value = hash_table_retrieve(ht, "key-9");
  mu_assert(strcmp(return_value, "val-9") == 0, "Value is not stored correctly");

  hash_table_insert(ht, "key-0", "new-val-0");
  hash_table_insert(ht, "key-1", "new-val-1");
  hash_table_insert(ht, "key-2", "new-val-2");
  hash_table_insert(ht, "key-3", "new-val-3");
  hash_table_insert(ht, "key-4", "new-val-4");
  hash_table_insert(ht, "key-5", "new-val-5");
  hash_table_insert(ht, "key-6", "new-val-6");
  hash_table_insert(ht, "key-7", "new-val-7");
  hash_table_insert(ht, "key-8", "new-val-8");
  hash_table_insert(ht, "key-9", "new-val-9");

  return_value = hash_table_retrieve(ht, "key-0");
  mu_assert(strcmp(return_value, "new-val-0") == 0, "Value is not overwritten correctly");
  return_value = hash_table_retrieve(ht, "key-1");
  mu_assert(strcmp(return_value, "new-val-1") == 0, "Value is not overwritten correctly");
  return_value = hash_table_retrieve(ht, "key-2");
  mu_assert(strcmp(return_value, "new-val-2") == 0, "Value is not overwritten correctly");
  return_value = hash_table_retrieve(ht, "key-3");
  mu_assert(strcmp(return_value, "new-val-3") == 0, "Value is not overwritten correctly");
  return_value = hash_table_retrieve(ht, "key-4");
  mu_assert(strcmp(return_value, "new-val-4") == 0, "Value is not overwritten correctly");
  return_value = hash_table_retrieve(ht, "key-5");
  mu_assert(strcmp(return_value, "new-val-5") == 0, "Value is not overwritten correctly");
  return_value = hash_table_retrieve(ht, "key-6");
  mu_assert(strcmp(return_value, "new-val-6") == 0, "Value is not overwritten correctly");
  return_value = hash_table_retrieve(ht, "key-7");
  mu_assert(strcmp(return_value, "new-val-7") == 0, "Value is not overwritten correctly");
  return_value = hash_table_retrieve(ht, "key-8");
  mu_assert(strcmp(return_value, "new-val-8") == 0, "8Value is not overwritten correctly");
  return_value = hash_table_retrieve(ht, "key-9");
  mu_assert(strcmp(return_value, "new-val-9") == 0, "9Value is not overwritten correctly");

  return 0;
}
#endif