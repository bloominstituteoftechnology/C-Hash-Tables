#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable {
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
  if (pair != NULL) {
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
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
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
  HashTable *ht = malloc(sizeof(HashTable));

  // Allocate memory for key/value pair storage
  ht->storage = calloc(capacity, sizeof(LinkedPair *));
  ht->capacity = capacity;

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{

  // Hash the key
  unsigned int index = hash(key, ht->capacity);
  // Create a key/value pair
  LinkedPair *linkedPair = create_pair(key, value);

  LinkedPair *stored_linked_pair = ht->storage[index];

  // If there is something already there
  if (stored_linked_pair != NULL)
  {

    if (strcmp(key, stored_linked_pair->key) != 0)
    {
      stored_linked_pair->next = linkedPair;
    }
    if (strcmp(key, stored_linked_pair->key) == 0)
    {
      printf("WARNING: key already in LL, overriding value\n");
      stored_linked_pair->value = value;
    }
    return;
  }

  // Add the hashedkey to the appropriate spot in ht
  ht->storage[index] = linkedPair;



}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{

  unsigned int index = hash(key, ht->capacity);

  if (ht->storage[index] == NULL || (
      strcmp(ht->storage[index]->key, key) != 0))
  {
    printf("Unable to remove entry\n");
  }
  else
  {
    destroy_pair(ht->storage[index]);
    ht->storage[index] = NULL;
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
  // hash the provided key to get the index
  unsigned int index = hash(key, ht->capacity);

  // if there's nothing there return NULL
  if (ht->storage[index] == NULL)
  {
    printf("Unable to retrieve entry\n");


    return NULL;
  }

  // if the key's don't match search next LinkedPair if available

  int * currentelement = ht->storage[index];
  while (currentelement->next =! NULL) {

    if 
      ((strcmp(currentKey->key, key) != 0))   {
    printf("Found it!\n");
    return currentKey->value;
  }

      currentelement = currentelement->next;

  }

  // otherwise return the value at the given key
  return ht->storage[index]->value;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{

}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
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

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
