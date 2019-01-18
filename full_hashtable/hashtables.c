
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
void hash_table_insert(HashTable *ht, char *key, char *value) //create hash, see if something is at key, if not insert, if occ add new pair as *next
{
  unsigned int hashedkey = hash(key, ht->capacity);
  LinkedPair *newnode = create_pair(key, value);
  if (ht->storage[hashedkey] == NULL) //empty index, insert value
  {
    ht->storage[hashedkey] = newnode;
    printf("Inserted into empty index [%d]\n", hashedkey);
  }
  else if (strcmp(key, ht->storage[hashedkey]->key) == 0) //if you reach node with same key as passed in key just change value
  {
    ht->storage[hashedkey]->value = strdup(value);
    printf("Just overwrote a value of a node with same key at index [%d]\n", hashedkey);
  }
  else if (ht->storage[hashedkey]->next == NULL) //If the end is reached add node as next to existing last node
  {
    ht->storage[hashedkey]->next = newnode;
    printf("just added a node as a linked list [%d]\n ", hashedkey);
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
  unsigned int hashedkey = hash(key, ht->capacity);

  LinkedPair *currentpair = ht->storage[hashedkey];
  LinkedPair *oldpair;

  while (currentpair != NULL)
  {
    if (strcmp(key, currentpair->key) == 0 && currentpair->next == NULL)
    {
      free(currentpair->key);
      free(currentpair->value);
      free(currentpair);
    }
    else if (strcmp(key, currentpair->key) == 0 && currentpair->next != NULL)
    {
      oldpair->next = currentpair->next;
      free(currentpair->key);
      free(currentpair->value);
      free(currentpair);
    }
    oldpair = currentpair;
    currentpair = oldpair->next;
  }
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
// char *hash_table_retrieve(HashTable *ht, char *key)
// {
//   unsigned int hashedkey = hash(key, ht->capacity);
//   while (ht->storage[hashedkey] != NULL)
//   {
//     if (strcmp(key, ht->storage[hashedkey]->key) == 0)
//     {

//       return ht->storage[hashedkey]->value;
//     }
//     else if (ht->storage[hashedkey]->next != NULL)
//     {
//       ht->storage[hashedkey] = ht->storage[hashedkey]->next;
//     } ////traverse linked lists to check if key matches
//   }
//   return NULL;
// }

char *hash_table_retrieve(HashTable *ht, char *key)
{
    unsigned int index = hash(key, ht->capacity);

    LinkedPair *current_pair = ht->storage[index];

    while (current_pair != NULL)
    {
        if (strcmp(current_pair->key, key) == 0)
        {
            return current_pair->value;
        }
        current_pair = current_pair->next;
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
  HashTable *new_ht = create_hash_table(2 * ht->capacity);

  LinkedPair *current_pair;
  for (int i = 0 ; i < ht->capacity ; i++) {
    current_pair = ht->storage[i];
    while (current_pair != NULL) {
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
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("Retrieved: %s", hash_table_retrieve(ht, "line_1"));
  printf("Retrieved: %s", hash_table_retrieve(ht, "line_2"));
  printf("Retrieved: %s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("Should be tiny hash table = %s \n", hash_table_retrieve(ht, "line_1"));
  printf("Filled beyond capacity = %s \n", hash_table_retrieve(ht, "line_2"));
  printf("Linked lists save lives = %s \n", hash_table_retrieve(ht, "line_3"));
  printf("Resizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif