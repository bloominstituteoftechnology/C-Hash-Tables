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
  ht->storage = calloc(capacity, sizeof(LinkedPair) * capacity);

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
  // create linked pair
  LinkedPair *new_pair = create_pair(key, value);
  // hash its index
  int index = hash(key, ht->capacity);

  // if there is a linked pair at that index
  if (ht->storage[index] != NULL)
  {
    LinkedPair *current_node = ht->storage[index];
    // Check if key exists
    while (1)
    {
      // IF KEYS ARE EQUAL
      if (strcmp(current_node->key, key) == 0)
      {
        free(current_node->value);
        current_node->value = strdup(value);
        break;
      }
      // MOVE TO NEXT NODE
      if (current_node->next != NULL)
      {
        current_node = current_node->next;
      }
      // BREAK OUT OF LOOP
      else
      {
        break;
      }
    }
    // ADD PAIR TO NEXT OPEN NODE
    current_node->next = new_pair;
  }
  // IF THERE IS NO LINKED LIST
  else
  {
    // ADD NODE
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
  int index = hash(key, ht->capacity);
  // DO SOME STUFF
  if (ht->storage[index] != NULL)
  {
    LinkedPair *current_node = ht->storage[index];
    while (1)
    {
      if (strcmp(current_node->key, key) == 0)
      {
        destroy_pair(current_node);
      }
      if (current_node->next != NULL)
      {
        current_node = current_node->next;
      }
      else
      {
        break;
      }
    }
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
  int index = hash(key, ht->capacity);

  if (ht->storage[index] != NULL)
  {
    LinkedPair *current_node = ht->storage[index];

    while (current_node != NULL)
    {
      if (strcmp(current_node->key, key) == 0)
      {
        return ht->storage[index]->value;
      }
      current_node = current_node->next;
    }
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
    LinkedPair *current_node = ht->storage[i];
    // IF THERE IS AT LEAST ONE NODE
    if (current_node != NULL)
    {
      // LOOP THROUGH LINKED LIST
      while (1)
      {
        // IF NODE IN LINKED LIST
        if (current_node->next != NULL)
        {
          destroy_pair(current_node);
          current_node = current_node->next;
        }
        // LAST ITEM IN LINKED LIST - DELETE
        else
        {
          destroy_pair(current_node);
          break;
        }
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
  HashTable *new_ht = calloc(ht->capacity * 2, ht->capacity * 2 * sizeof(HashTable));
  new_ht->capacity = ht->capacity * 2;
  new_ht->storage = calloc(new_ht->capacity, sizeof(LinkedPair) * new_ht->capacity);

  for (int i = 0; i < ht->capacity; i++)
  {
    new_ht->storage[i] = ht->storage[i];
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

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
