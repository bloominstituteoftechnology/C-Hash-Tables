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
  HashTable *ht;
  ht = malloc(sizeof(HashTable));
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
  // Get hash value
  int hashvalue = hash(key, ht->capacity);
  // Prepare to search through linked list by creating pointers to current and previous pairs
  LinkedPair *currentpair = ht->storage[hashvalue];
  LinkedPair *previouspair = NULL;

  // Search linked list as long as current pair is not NULL
  while (currentpair != NULL)
  {
    // if two keys are the same then overwrite value
    if (strcmp(currentpair->key, key) == 0)
    {
      printf("Overwriting Value\n");
      currentpair->value = value;
      break;
    }
    // If key not found change current
    else
    {
      previouspair = currentpair;
      currentpair = currentpair->next;
    }
  }
  // If current pair is Null and not found in linked list
  // If while loop breaks with current pair defined this will not run
  if (currentpair == NULL)
  {
    // Create Linked pair
    LinkedPair *newpair = create_pair(key, value);
    // If there is a previous pair set it's next to new pair
    if (previouspair != NULL)
    {
      previouspair->next = newpair;
    }
    // else set new pair to hash value
    else
    {
      ht->storage[hashvalue] = newpair;
    }
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
  // Get hash value
  int hashvalue = hash(key, ht->capacity);
  // Prepare to search through linked list by setting current pair and previous pair.
  LinkedPair *currentpair = ht->storage[hashvalue];
  LinkedPair *previouspair = NULL;

  // Search for match
  while (currentpair != NULL && strcmp(currentpair->key, key) != 0)
  {
    previouspair = currentpair;
    currentpair = currentpair->next;
  }

  // if previous pair exists
  if (previouspair)
  {
    // set previous next as current next
    previouspair->next = currentpair->next;
  }
  else
  {
    // if no previous pair
    // set first as current next
    ht->storage[hashvalue] = currentpair->next;
  }
  currentpair = NULL;
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
char *hash_table_retrieve(HashTable *ht, char *key)
{
  // Get hash value
  int hashvalue = hash(key, ht->capacity);
  // Prepare to search through linked list by setting up current pair
  LinkedPair *currentpair = ht->storage[hashvalue];
  // while currentpair is not NULL
  while (currentpair)
  {
    // if current pair key is equal to key searching for return current pair value
    if (strcmp(currentpair->key, key) == 0)
    {
      return currentpair->value;
    }
    // if not keys not equal go to next pair in linked list
    currentpair = currentpair->next;
  }
  // If current node equal to null then element not found
  printf("Element not found\n");
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
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
  printf("\n=== Begin Test ===\n");
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  // int old_capacity = ht->capacity;
  // ht = hash_table_resize(ht);
  // int new_capacity = ht->capacity;

  // printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  // destroy_hash_table(ht);
  printf("=== End Test ===\n\n");
  return 0;
}
#endif
