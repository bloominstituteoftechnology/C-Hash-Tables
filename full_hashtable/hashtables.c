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
  int index = hash(key, ht->capacity); // hash the key to get the index

  if(ht->storage[index] != NULL){ // if there is something in the index already ...
    LinkedPair *current_pair = ht->storage[index]; // store that pair in a variable while we make room
    while(current_pair != NULL){ // so long as our current_pair has a value, do some logic checks:
      if(strcmp(current_pair->key, key) == 0){ // if the keys for the new pair and current pair are the same,
        free(current_pair->value); // free the value...
        current_pair->value = strdup(value); // ... and replace it with the new value
        break;
      } else if(current_pair->next == NULL){ // if there is no next value in the LinkedList, insert the given pair there
        current_pair->next = create_pair(key, value);
        break;
      } else {
        current_pair = current_pair->next; // if neither of the above conditions are met, move to the next pair
      }
    }
  } else {
    ht->storage[index] = create_pair(key, value); // if there's nothing at the given index, store the new pair there
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
  int index = hash(key, ht->capacity); // get our index from the hash function

  if(ht->storage[index] != NULL){ // if there is a value at that index...
    LinkedPair *current_pair = ht->storage[index]; // store that pair in a variable
    LinkedPair *previous_pair = NULL; // and initialize a pointer for its previous pair
    while(current_pair != NULL){ // iterate over the linked list, starting from the current pair
      if(strcmp(current_pair->key, key) == 0){ // if the keys match, do this:
        if(previous_pair == NULL){ // if no pair precedes it...
          ht->storage[index] = current_pair->next; // ...assign the next pair to the current index
        } else { // if a previous value exists...
          previous_pair->next = current_pair->next; // ... assign the current_next value to the previous_next value
        } /** ^ This effectively removes the pair from the LinkedList ^ **/
        destroy_pair(current_pair); // once it's removed from the list, free its memory and end the loop
        break;
      } else { // if they keys don't match, iterate over the list until they do
      previous_pair = current_pair;
      current_pair = current_pair->next;
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
  int index = hash(key, ht->capacity); // get our index from the hash function

  if(ht->storage[index] != NULL){ // if there is a pair at that index, store it in current_pair
    LinkedPair *current_pair = ht->storage[index];
    while(current_pair != NULL){ // iterate to check if the keys are matching
      if(strcmp(current_pair->key, key) == 0){ // if they do...
        return current_pair->value; //... return that pair
      } else {
        current_pair = current_pair->next; // otherwise, move to the next pair in the LinkedList
      }
    }
  }

  return NULL; // return NULL if no key match is found
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  for(int i = 0; i < ht->capacity; i++){
    if(ht->storage[i] != NULL){
      LinkedPair *next_pair = ht->storage[i]->next;
      while(next_pair != NULL){
        free(ht->storage[i]);
        ht->storage[i] = next_pair;
        next_pair = ht->storage[i]->next;
      }
    }
  }
  free(ht->storage);
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
  for(int i = 0; i < ht->capacity; i++){
    LinkedPair *current_pair = ht->storage[i];
    if(current_pair != NULL){
      while(current_pair != NULL){
        hash_table_insert(new_ht, current_pair->key, current_pair->value);
        current_pair = current_pair->next;
      }
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
