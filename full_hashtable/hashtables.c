#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the
  next `LinkedPair` in the list of `LinkedPair` nodes.
 */
typedef struct LinkedPair
{
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable
{
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL)
  {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
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

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht;
  ht = (HashTable *)malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity * capacity, sizeof(LinkedPair *));
  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  int hashed_key = hash(key, ht->capacity);
  printf("hashed key %d \n", hashed_key);
  if (ht->storage[hashed_key])
  {
    printf("found a key duplicate \n");
    LinkedPair *pair = ht->storage[hashed_key];
    while (pair != NULL)
    {
      if (pair->next == NULL)
      {
        printf("Reached the end of the array creating a new linkedpair \n");
        LinkedPair *new_pair = create_pair(key, value);
        pair->next = new_pair;
        break;
      }
      else if (strcmp(pair->key, key) == 0)
      {
        printf("Found a duplicate updating values \n");
        pair->value = value;
        break;
      }
      printf("lets go to the next one");
      pair = pair->next;
    }
  }
  else
  {
    printf("No key found creating a new entry \n");
    LinkedPair *new_pair = create_pair(key, value);
    ht->storage[hashed_key] = new_pair;
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  int hashed_key = hash(key, ht->capacity);
  if (ht->storage[hashed_key])
  {
    LinkedPair *curr_pair = ht->storage[hashed_key];
    LinkedPair *prev_pair = NULL;
    while (curr_pair != NULL)
    {
      if (strcmp(curr_pair->key, key) == 0)
      {
        printf("Found our target \n");
        if (prev_pair != NULL)
        {
          printf("there was a previous pair update its values \n");
          prev_pair->next = curr_pair->next;
        }
        else
        {
          printf("No previous pairs");
          ht->storage[hashed_key] = curr_pair->next;
        }
        printf("Destroying pairs now with the value of %s\n", curr_pair->value);
        destroy_pair(curr_pair);
        break;
      }
      else if (curr_pair->next == NULL)
      {
        printf("No value found in the key provided\n");
        break;
      }
      else
      {
        prev_pair = curr_pair;
        curr_pair = curr_pair->next;
      }
    }
  }
  else
  {
    printf("No value found at key %s \n", key);
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  if (ht != NULL)
  {
    printf("hash table error! \n");
    return NULL;
  }
  int hashed_key = hash(key, ht->capacity);
  int found = 1;
  if (ht->storage[hashed_key])
  {
    LinkedPair *pair = ht->storage[hashed_key];
    while (found)
    {
      if (strcmp(pair->key, key) == 0)
      {
        found = 0;
        printf("Key %s has the value %s \n", pair->key, pair->value);
        return pair->value;
      }
      else if (pair->next == NULL)
      {
        printf("No value found! with the key %s", key);
        break;
      }
      pair = pair->next;
    }
  }
  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      LinkedPair *pair = ht->storage[i];
      while (pair != NULL)
      {
        LinkedPair *next_pair = pair->next;
        destroy_pair(pair);
        pair = next_pair;
      }
    }
  }
  free(ht->storage);
  free(ht);
  printf("destroyed the hash table \n");
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = create_hash_table(ht->capacity * 2);
  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i])
    {
      LinkedPair *pair = ht->storage[i];
      while (pair != NULL)
      {
        printf("%s this is the key \n", pair->key);
        pair = pair->next;
      }
    }
  }

  return new_ht;
}

#ifndef TESTING
int main(void)
{
  printf("Coolio");
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "hash table entry #1 table\n");
  hash_table_insert(ht, "line_2", "hash table entry #2 table\n");
  hash_table_insert(ht, "line_3", "hash table entry #3 table\n");
  printf("%s heyaa \n", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif