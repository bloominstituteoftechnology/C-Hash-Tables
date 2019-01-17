#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Hash table key/value pair with linked list pointer
 ****/
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;  // Remember to use this when building linked lists for overlapping hashed_keys
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
// This is the same as for the basic table but with Linked Pairs this time
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair)); // ??? Should this use sizeof(LinkedPair) or sizeof(LinkedPair *) ???
  return ht;
}

/****
  Fill this in.
  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.
  *Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.*
 ****/
// Think of the hash table as an array of linked lists.
// Hashed keys correspond to indexes of the hash table.
// When multiple Pairs have keys that hash to the same number (they are equal modulo ht->capacity),
// they form a linked list at the same index. hashed_key == index

void hash_table_insert(HashTable *ht, char *key, char *value)
{
  unsigned int hashed_key = hash(key, ht->capacity);
  // LinkedPair *new_pair = create_pair(key, value);

  // if (ht->storage[hashed_key] == NULL) {         // If there are no Pairs in the ht with the same hashed_key
  //   ht->storage[hashed_key] = new_pair;          // insert the new_pair at that index
  //   return;
  // }

  LinkedPair *current_pair = ht->storage[hashed_key];      // make a pointer to the first pair at the index for this hashed_key
  while (current_pair != NULL) {                           // Step through the linked list until landing on a NULL slot.
    if (strcmp(key, current_pair->key) == 0) {             // If the key of the current pair is equal to the key of the pair to be inserted,
      current_pair->value = value;                         // then overwrite the value of the existing pair with the new_pair value
      return;
    }
    // if (current_pair->next == NULL) {
    //   current_pair->next = new_pair;
    //   return;
    // }
    current_pair = current_pair->next;
  }
  LinkedPair *new_pair = create_pair(key, value);
  ht->storage[hashed_key] = new_pair;                      // if no matching keys were found, insert the new_pair at the first NULL index
  return;
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
// I'm applying basically the same process used for insert
char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int hashed_key = hash(key, ht->capacity);
  LinkedPair *current_pair = ht->storage[hashed_key];

  // if (current_pair == NULL) {
  //   return NULL;
  // }
  
  while (current_pair != NULL) {
    if (strcmp(current_pair->key, key) == 0){
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
void destroy_hash_table(HashTable *ht)
{
  int i = 0;
  while (i <= ht->capacity) {   
    ht->storage[i] = NULL;
    free(ht->storage[i]);
    i++;
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
  HashTable *new_ht;

  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);
  int capacity = ht->capacity;
  printf("Hash table created with capacity == %d\n", capacity);

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
