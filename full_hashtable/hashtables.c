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
  HashTable *ht = calloc(sizeof(HashTable), capacity);
  ht->capacity = capacity;
  ht->storage = calloc(sizeof(LinkedPair), capacity);

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
  printf("\n\nLet's insert: %s : %s", key, value);
  int hash_key = hash(key, ht->capacity);
  if (ht->storage[hash_key] == NULL)
  {
    printf("No pair at that index\n");
    LinkedPair *pair = create_pair(key, value);
    ht->storage[hash_key] = pair;
  }
  else if (ht->storage[hash_key] != NULL)
  {
    printf("There is a pair at that index\n");
    LinkedPair *curr_pair = ht->storage[hash_key];

    //Changes to do while loop because i was stuck in an infinite loop
    do
    {
      //printf("%s ", curr_pair->value);

      //check if keys are the same
      if (strcmp(curr_pair->key, key) == 0)
      {
        free(curr_pair->value);
        curr_pair->value = strdup(value);
      }
      if (curr_pair->next == NULL)
      {
        printf("added new pair to the list\n");

        LinkedPair *new_pair = create_pair(key, value);
        printf("%s -> %s\n", curr_pair->key, new_pair->key);
        curr_pair->next = new_pair;
      }

      if (curr_pair->next != NULL)
      {
        curr_pair = curr_pair->next;
      }
    } while (curr_pair->next != NULL);
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
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
char *hash_table_retrieve(HashTable *ht, char *key)
{
  int hash_key = hash(key, ht->capacity);
  if (ht->storage[hash_key] == NULL)
  {
    return NULL;
  }
  if (ht->storage[hash_key])
  {
    if (ht->storage[hash_key]->next == NULL)
    {
      if (strcmp(ht->storage[hash_key]->key, key) == 0)
      {
        return ht->storage[hash_key]->value;
      }
      return ht->storage[hash_key]->value;
    }
    else
    {
      LinkedPair *curr_pair = ht->storage[hash_key];
      printf("next value in the list: %s\n", curr_pair->next->key);
      do
      {
        printf("\n\n\n-----looking for %s in {%s}\n", key, curr_pair->key);
        printf("%s\n", curr_pair->key);

        if (strcmp(curr_pair->key, key) == 0)
        {
          return curr_pair->value;
        }
        curr_pair = curr_pair->next;

      } while (curr_pair);
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

  // int old_capacity = ht->capacity;
  // ht = hash_table_resize(ht);
  // int new_capacity = ht->capacity;

  // printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  // destroy_hash_table(ht);

  return 0;
}
#endif
