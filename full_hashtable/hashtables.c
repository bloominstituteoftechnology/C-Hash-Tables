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
  Creates hashtable

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
  Inserts KV pair into Hashtable

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 ****/
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  unsigned int index = hash(key, ht->capacity);

  LinkedPair *new_pair = create_pair(key, value);

  LinkedPair *current_pair = ht->storage[index];
  
  if (current_pair == NULL){
    ht->storage[index] = new_pair;
  }else{

  while (current_pair != NULL){
  
      if (strcmp(current_pair->key, key) == 0) 
      {
        current_pair->value = value;
        break;
      }
      else if (current_pair->next == NULL){
        current_pair->next = new_pair;
        break;
      }
      current_pair = current_pair->next;
    }
  }
}

/****
  Removes KV from hashtable

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  LinkedPair *current_pair = ht->storage[index];
  
  if (current_pair == NULL){
    fprintf(stderr, "KEY NOT FOUND");
  }else{

  int i = 0;
  while (current_pair != NULL){

      if (strcmp(current_pair->key, key) == 0) 
      {
        // realign the LL
        if (ht->storage[i-1] != NULL){
          // previous pair should point to next -> next
          ht->storage[i-1]->next = current_pair->next;
        }
        destroy_pair(current_pair);
        break;
      }
      current_pair = current_pair->next;
      i++;
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
  unsigned int index = hash(key, ht->capacity);

  LinkedPair *current_pair = ht->storage[index];
  LinkedPair *last_pair = NULL;
  
  
  while (current_pair != NULL && strcmp(current_pair->key, key) != 0) {
    last_pair = current_pair;
    current_pair = last_pair->next;
  }
  // current pair is either null or matches the key

  if (current_pair == NULL){
    fprintf(stderr, "KEY %s NOT FOUND ", key);
    return NULL;
  } else {
    return current_pair->value;
  }


}

/****
  Destroys Hashtable

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  LinkedPair *current_pair;
  LinkedPair *pair_to_destroy;

  for (int i = 0 ; i < ht->capacity ; i++) {
    current_pair = ht->storage[i];
    while(current_pair != NULL) {
      pair_to_destroy = current_pair;
      
      current_pair = current_pair->next;
      if (pair_to_destroy != NULL){
        destroy_pair(pair_to_destroy);
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
  HashTable *new_ht = malloc(sizeof(HashTable));

  new_ht->capacity = ht->capacity * 2;
  new_ht->storage = calloc(new_ht->capacity, sizeof(LinkedPair *));

  for (int i = 0; i < ht->capacity; i++){
    if (ht->storage[i] != NULL){
      // may lose other LL pointers
      hash_table_insert(new_ht, ht->storage[i]->key, ht->storage[i]->value);
    }
    

  }
  destroy_hash_table(ht);
  return new_ht;
}


#ifndef TESTING
int main(void)
{
  // struct HashTable *ht = create_hash_table(2);

  // hash_table_insert(ht, "line_1", "Tiny hash table");
  // printf("%s\n", hash_table_retrieve(ht, "line_1"));
  // hash_table_remove(ht, "line_1");
  // printf("(REMOVED SUCCESS)%s\n", hash_table_retrieve(ht, "line_1"));

  // hash_table_insert(ht, "line_2", "HELLO FROM LINE 2!");
  // printf("%s\n", hash_table_retrieve(ht, "line_2"));
  // hash_table_insert(ht, "line_2", "GOODBYE FROM LINE 2!\n");
  // printf("%s", hash_table_retrieve(ht, "line_2"));

  // int old_capacity = ht->capacity;
  // ht = hash_table_resize(ht);
  // int new_capacity = ht->capacity;

  // printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  // destroy_hash_table(ht);

  return 0;
}
#endif
