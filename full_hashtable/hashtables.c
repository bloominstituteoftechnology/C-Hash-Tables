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
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *));
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
  // find new hashed index from key and capacity
  unsigned int new_index = hash(key, ht->capacity);
  printf("new_index: %d\n", new_index);
  // create new pair
  LinkedPair *pair = create_pair(key, value);
  // determine if key/pair already exists
  LinkedPair *stored_pair = ht->storage[new_index];

  if (ht->storage[new_index] != NULL)
  {
    LinkedPair *pt1 = stored_pair;
    LinkedPair *ptr_lag1;
    int index2 = 0;
    while (pt1->next != NULL)
    {
      if (strcmp(pt1->key, key) == 0)
      {
        printf("Destroy key: %s %s\n", pt1->key, pt1->value);
        pair->next = pt1->next;
        destroy_pair(pt1);
        if (index2 != 0)
        {
          ptr_lag1->next = pair;
        }
        return;
      }
      index2++;
      ptr_lag1 = pt1;
      pt1 = pt1->next;
    }
    if (strcmp(pt1->key, key) == 0)
    {
      printf("Destroy key 2nd: %s %s\n", pt1->key, pt1->value);
      // printf("pt1->next: %s\n", pt1->next);
      // if (pt1->next == NULL)
      // {
      //   printf("nulllllllllll");
      //   pair->next == NULL;
      // }
      // else
      // {
      //   pair->next = pt1->next;
      // }
      if (index2 == 0)
      {
        pt1 = pair;
        return;
      }
      
      destroy_pair(pt1);
      printf("destroyed");
      ptr_lag1->next = pair;
      printf("ptr_lag1->next");
      return;
    }
    else
    {
      printf("Keys do not match: %s.\n", key);
      pt1->next = pair;
    }
  }
  else
  {
    printf("Key is doesn't exist: %s.\n", key);
    // place new pair in hashed index of hash table
    ht->storage[new_index] = pair;
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
  // find hashed index
  int index = hash(key, ht->capacity);
  // destroy pair via hashed index
  if (ht->storage[index] != NULL)
  {
    LinkedPair *ptr = ht->storage[index];
    LinkedPair *ptr_lag;
    int index = 0;
    while (ptr->next != NULL)
    {
      if (strcmp(ptr->key, key) == 0)
      {
        if (ptr_lag != 0)
        {
          ptr_lag->next = ptr->next;
          // remove pair
          destroy_pair(ptr);
        }
        else
        {
          ht->storage[index] = ptr->next;
          destroy_pair(ptr);
        }
      }
      index++;
      ptr_lag = ptr;
      ptr = ptr->next;
    }
    if (strcmp(ptr->key, key) == 0)
    {
      // remove pair
      destroy_pair(ptr);
      // set storage index to null
      ptr_lag->next = NULL;
    }
    else
    {
      // print error
      printf(stderr, "Unable to remove entry with key: %s\n", key);
    }
  }
  else
  {
    // print error
    printf(stderr, "Unable to remove entry with key: %s\n", key);
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
  // find hashed index value from key
  int index = hash(key, ht->capacity);
  // if key value / indexed storage value is null return null else return value
  if (ht->storage[index] != NULL)
  {
    LinkedPair *ptr2 = ht->storage[index];
    while (ptr2->next != NULL)
    {
      if (strcmp(ptr2->key, key) == 0)
      {
        return ptr2->value;
      }
      ptr2 = ptr2->next;
    }
    if (strcmp(ptr2->key, key) == 0)
    {
      return ptr2->value;
    }
  }
  printf(stderr, "Unable to retrieve entry with key: %s\n", key);
  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  // loop through storage and destrpy pairings and then free storage per index

  for (int i = 0; i < ht->capacity; i++)
  {
    if (ht->storage[i] != NULL)
    {
      LinkedPair *ptr3 = ht->storage[i];
      LinkedPair *ptr3_lag;
      while (ptr3->next != NULL)
      {
        ptr3_lag = ptr3;
        ptr3 = ptr3->next;
        destroy_pair(ptr3_lag);
      }
      destroy_pair(ptr3);
    }
  }
  // free overall storage
  free(ht->storage);
  // free hash table structure
  free(ht);
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  int new_capacity = ht->capacity * 2;
  HashTable *new_ht = create_hash_table(new_capacity);
  new_ht->storage = ht->storage;
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

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
