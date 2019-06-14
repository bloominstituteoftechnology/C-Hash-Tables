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
  HashTable *ht;
  ht = malloc(sizeof(HashTable));
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
int index = hash(key, ht->capacity);
if (ht->storage[index] != NULL) {
  LinkedPair *pair = ht->storage[index];
  LinkedPair *last_pair = pair;
  while (pair->next != NULL) {
    if (strcmp(pair->key, key) == 0) {
      pair->value = value;
      printf("Value for key: %s been overwritten\n", pair->key);
      break;
    } else {
      last_pair = pair;
      pair = pair->next;
    }
  }
    if (strcmp(pair->key, key) == 0) {
    pair->value = value;
    printf("Value for key: %s been overwritten\n", pair->key);
  } else {
  LinkedPair *new_pair = create_pair(key, value);
  last_pair->next = new_pair;
  printf("New Pair was added to the end\n");
  }
} else {
LinkedPair *new_pair = create_pair(key, value);
ht->storage[index] = new_pair;
printf("New Pair was added to the table\n");
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
  int index = hash(key, ht->capacity);

  if (ht->storage[index] != NULL) {
    LinkedPair *current = ht->storage[index];
    // if the key is the first one
    if (strcmp(current->key, key) == 0) {
      ht->storage[index] = current->next; 
      destroy_pair(current);
    } else {
      // otherwise, the key is one of the other elements
      while (current->next != NULL) {
      // printf("Current key: %s, value: %s\n", current->key, current->value);
      if (strcmp(current->next->key, key) == 0) {
        LinkedPair *next = current->next;
        current->next = next->next;
        destroy_pair(next);
        break;
      }
        current = current->next;
      }

    }
  }
}



  // int index = hash(key, ht->capacity);
  // LinkedPair *current_pair = ht->storage[index];
  // LinkedPair *previous_pair = NULL;

  // while (current_pair != NULL && strcmp(current_pair->key, key) != 0)
  // {
  //   previous_pair = current_pair;
  //   current_pair = current_pair->next;
  // }

  // if (current_pair == NULL)
  // {

  //   fprintf(stderr, "Unable to remove entry with key: %s\n", key);
  // }
  // else
  // {

  //   if (previous_pair == NULL)
  //   { // Removing the first element in the Linked List
  //     ht->storage[index] = current_pair->next;
  //   }
  //   else
  //   {
  //     previous_pair->next = current_pair->next;
  //   }

  //   destroy_pair(current_pair);
  // }
  // }
    
// 



/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
	  int index = hash(key, ht->capacity);
	  if (ht->storage[index] != NULL) {
	    LinkedPair *pair = ht->storage[index];
	    while (pair->next != NULL) {
	      if (strcmp(pair->key, key) == 0) {
	        return pair->value;
	      }
	      pair = pair->next;
	    }
	    if (strcmp(pair->key, key) == 0) {
	      return pair->value;
	    }
	  }
	  printf("Unable to retrieve entry with key: %s\n", key);
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
      LinkedPair *temp_pair;
      while (pair->next != NULL) {
        temp_pair = pair;
        pair = pair->next;
        destroy_pair(temp_pair);
      }
      destroy_pair(pair);
    }
  }
  free(ht->storage);
  free(ht);
  printf("Hash table has been deleted\n");
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
  new_ht = create_hash_table(2 * ht->capacity);
  LinkedPair *current_pair;
  for (int i = 0; i < ht->capacity; i++) {
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
  // struct HashTable *ht = create_hash_table(2);

  // hash_table_insert(ht, "line_1", "Tiny hash table\n");
  // hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  // hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  // printf("%s", hash_table_retrieve(ht, "line_1"));
  // printf("%s", hash_table_retrieve(ht, "line_2"));
  // printf("%s", hash_table_retrieve(ht, "line_3"));

  // int old_capacity = ht->capacity;
  // ht = hash_table_resize(ht);
  // int new_capacity = ht->capacity;

  // printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);


 struct HashTable *ht = create_hash_table(8);

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

    hash_table_remove(ht, "key-9");
    hash_table_remove(ht, "key-8");
    hash_table_remove(ht, "key-7");
    hash_table_remove(ht, "key-6");
    hash_table_remove(ht, "key-5");
    hash_table_remove(ht, "key-4");
    hash_table_remove(ht, "key-3");
    hash_table_remove(ht, "key-2");
    hash_table_remove(ht, "key-1");
    hash_table_remove(ht, "key-0"); 

  destroy_hash_table(ht);

  return 0;
}
#endif
