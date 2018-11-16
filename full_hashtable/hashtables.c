#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Hash table key/value pair with linked list pointer
 ****/
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/****
  Hash table with linked pairs
 ****/
typedef struct HashTable {
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
  if (pair != NULL) {
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
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
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
  ht->storage = calloc(capacity, sizeof(LinkedPair));
  ht->capacity = capacity;

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
  LinkedPair *storagelocation = ht->storage[hash(key, ht->capacity)];
  LinkedPair *pair = create_pair(key, value);
  if (storagelocation == NULL) {
    ht->storage[hash(key, ht->capacity)] = pair;
  } else if (strcmp(storagelocation->key,key) == 0) {
    LinkedPair *tempnext = storagelocation->next;
    destroy_pair(storagelocation);
    ht->storage[hash(key, ht->capacity)] = pair;
    ht->storage[hash(key, ht->capacity)]->next = tempnext;
  } else if ((storagelocation != NULL) && (strcmp(storagelocation->key, key) != 0)) {
    ht->storage[hash(key, ht->capacity)] = pair;
    pair->next = storagelocation;
  } else {
    printf("WARNING: YOU'RE IN NO-MAN'S LAND! What's happening here?\n");
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
  LinkedPair *storagelocation = ht->storage[hash(key, ht->capacity)];
  if (storagelocation == NULL) {
    printf("This key does not exist :/\n");
  } else if (strcmp(storagelocation->key, key) == 0) {
    LinkedPair *tempstoragelocation = storagelocation->next;
    destroy_pair(storagelocation);
    ht->storage[hash(key, ht->capacity)] = tempstoragelocation;
    printf("Pair was destroyed successfully!\n");
  } else {
    LinkedPair *current = storagelocation;
    while (strcmp(current->key, key) != 0) {
      LinkedPair *tempnext = current->next;
      if (strcmp(current->next->key, key) == 0) {
        current->next = tempnext->next;
        destroy_pair(tempnext);
        printf("Pair was destroyed successfully!\n");
        break;
      } else if (current->next == NULL) {
        printf("WARNING: Could not find key/pair specified.\n");
        break;
      }
      current = tempnext;
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
  LinkedPair *storagelocation = ht->storage[hash(key, ht->capacity)];
  printf("%s\n", storagelocation->key);
  if (storagelocation == NULL) {
    printf("This key does not exist :/ 1\n");
  } else if (strcmp(storagelocation->key, key) == 0) {
    return storagelocation->value;
  }
  LinkedPair *current = storagelocation;
  while (strcmp(current->key, key) != 0) {
    LinkedPair *tempnext = current->next;
    if (current->next == NULL) {
      printf("WARNING: Could not find key/pair specified.\n");
      break;
    } else if (strcmp(tempnext->key, key) == 0) {
      return tempnext->value;
    }
    current = tempnext;
  }
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i < ht->capacity; i++) {
    if (ht->storage[i] != NULL) {
      LinkedPair *current = ht->storage[i];
      while (current->next != NULL) {
        LinkedPair *tempnext = current->next;
        free(current);
        current = tempnext;
      }
      free(current);
    }
  }
  free(ht);
}

/****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = create_hash_table(ht->capacity * 2);
  for (int i = 0; i<ht->capacity; i++) {
    LinkedPair *oldpair = ht->storage[i];
    LinkedPair *newpair = create_pair(oldpair->key, oldpair->value);
    newpair->next = oldpair->next;
    new_ht->storage[hash(newpair->key, new_ht->capacity)] = newpair;
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
