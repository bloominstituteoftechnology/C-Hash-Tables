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
  // hash its index
  unsigned int index = hash(key, ht->capacity);

  // if there is a linked pair at that index
  LinkedPair *current_node = ht->storage[index];
  LinkedPair *prev_node;

  // LOOP AND SEE IF KEY EXISTS
  while (current_node != NULL && strcmp(current_node->key, key) != 0)
  {
    prev_node = current_node;
    current_node = prev_node->next;
  }

  // IF KEY EXISTS OVERWRITE THE VALUE
  if (current_node != NULL)
  {
    // free(current_node->value);
    current_node->value = value;
  }
  else
  {
    // create linked pair
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

  // GET INDEX
  unsigned int index = hash(key, ht->capacity);

  LinkedPair *current_node = ht->storage[index];
  LinkedPair *prev_node = NULL;

  // LOOP AND SEE IF KEY EXISTS
  while (current_node != NULL && strcmp(current_node->key, key) != 0)
  {
    prev_node = current_node;
    current_node = prev_node->next;
  }

  if (current_node != NULL)
  {
    // IF CURRENT NODE IS FIRST IN LINKED LIST
    if (prev_node == NULL)
    {
      ht->storage[index] = current_node->next;
    }
    else
    {
      prev_node->next = current_node->next;
    }
    // REMOVE IT
    destroy_pair(current_node);
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
  // hash to get index
  unsigned int index = hash(key, ht->capacity);
  // STORE IN TEMP VARIABLE INDEX
  LinkedPair *current_node = ht->storage[index];

  while (current_node != NULL && strcmp(current_node->key, key) != 0)
  {
    current_node = current_node->next;
  }

  if (current_node != NULL)
  {
    return current_node->value;
  }
  else
  {
    return NULL;
  }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{

  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      LinkedPair *current_node = ht->storage[i];
      LinkedPair *prev_node;

      while (current_node != NULL)
      {
        prev_node = current_node;
        current_node = current_node->next;
        destroy_pair(prev_node);
      }
    }
  }

  if (ht->storage != NULL)
  {
    free(ht->storage);
  }

  if (ht != NULL)
  {
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
  // CREATE NEW HASH TABLE
  HashTable *new_ht = create_hash_table(ht->capacity * 2);

  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      LinkedPair *current_node = ht->storage[i];
      while (current_node != NULL)
      {
        hash_table_insert(new_ht, current_node->key, current_node->value);
        current_node = current_node->next;
      }
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
  // hash_table_remove(ht, "line_3");
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
