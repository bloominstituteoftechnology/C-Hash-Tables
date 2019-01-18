#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;


typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;
  return pair;
}


void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}


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


HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *));

  return ht;
}

void hash_table_insert(HashTable *ht, char *key, char *value)
{
  //get the index in the array from the hashed key 
  unsigned int index = hash(key, ht->capacity);

  //check if the index location in the array is occupied
  LinkedPair *current_pair = ht->storage[index];
  LinkedPair *last_pair;

  //if the location has a struct we start making a linked list
        //location is free       //are we at the end of the inked list?
  while(current_pair != NULL && strcmp(current_pair->key, key) != 0){
    last_pair = current_pair;
    current_pair = last_pair->next;
  }

  if (current_pair != NULL){
    current_pair->value = value;
  } else {
    LinkedPair *new_pair = create_pair(key, value);
    new_pair->next = ht->storage[index];
    ht->storage[index] = new_pair;
  }
}


void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);

  LinkedPair *current_pair = ht->storage[index];
  LinkedPair *previous_pair = NULL;

  while (current_pair != NULL && strcmp(current_pair->key, key) != 0) {
    previous_pair = current_pair; 
    current_pair = current_pair->next;
  }

  if (current_pair == NULL) {
    printf("Unable to remove entry with key: %s\n", key);
  } else {
    if (previous_pair == NULL) {
      ht->storage[index] = current_pair->next;
    } else {
      previous_pair->next = current_pair->next;
    }
    current_pair = NULL;
  }
}

char *hash_table_retrieve(HashTable *ht, char *key)
{
  int hashKey=hash(key, ht->capacity);
  LinkedPair *pair = ht->storage[hashKey];

  while(pair){
    printf("%s", pair->key);
    if(!strcmp(pair->key, key)){
      return pair->value;
    }
    pair = pair->next;
  }
  return NULL;
}

void destroy_hash_table(HashTable *ht)
{
  for (int i = 0; i< ht->capacity; i++){
    if(ht->storage[i] != NULL) {
      destroy_pair(ht->storage[i]);
    }
  }
  free(ht->storage);
  free(ht);
}

HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = malloc(sizeof(HashTable));
  new_ht->capacity = ht->capacity *2;
  new_ht->storage = calloc(new_ht->capacity, sizeof(LinkedPair *));

  //get dem values up
  for (int j = 0; j < ht->capacity; j++){
    if(ht->storage[j] != NULL){
      LinkedPair *current_pair = ht->storage[j];
      //LinkedPair *new_pair = create_pair(current_pair, value);
      // LinkedPair *create_pair(char *key, char *value)
      if (current_pair->next == NULL){
        hash_table_insert(new_ht, current_pair->key, current_pair->value);
      } else {
        while (current_pair != NULL){
          hash_table_insert(new_ht, current_pair->key, current_pair->value);
          current_pair = current_pair->next;
        }
      }
    }
  }

  //destroy them all
  for (int i = 0; i < ht->capacity; i++){
    if (ht->storage[i] != NULL){
      LinkedPair *current = ht->storage[i];
      LinkedPair *prev = NULL;
      if (current->next == NULL){
        destroy_pair(current);
      } else {
        while (current != NULL){
          prev = current; 
          current = current->next;
          destroy_pair(prev);
        }
      }
    }
  }
  return new_ht;
}

#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

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

    hash_table_remove(ht, "key-6");
    printf("\n%s", hash_table_retrieve(ht, "key-4"));

  
  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
