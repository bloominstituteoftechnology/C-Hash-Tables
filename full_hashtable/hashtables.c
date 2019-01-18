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

}

char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int index = hash(key, ht->capacity);
  if (ht->storage[index] == NULL){
    printf("No Value Present");
    return NULL;
  }

  LinkedPair *current_pair = ht->storage[index];

  while(current_pair->key != key){
    if (current_pair->next == NULL){
      break;
    }
    current_pair = current_pair->next;
  }

  return current_pair->value;
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
  new_ht->capacity = ht->capacity * 2;
  new_ht->storage = calloc(new_ht->capacity, sizeof(LinkedPair *));

  //looping through the old hash table and get my new keys
  for (int i = 0; i < ht->capacity; i++){
    //is the index spot empty?
    if (ht->storage[i] != NULL){
      //get my key new and put the old pair in the new hash table storage index
      
      LinkedPair *current = ht->storage[i];
      while(current->next != NULL){
        hash_table_insert(new_ht, current->key, current->value);
        if (current->next == NULL){
          break;
        }
        current = current->next;
      }
    } else {
      hash_table_insert(new_ht, ht->storage[i]->key, ht->storage[i]->value);
    }
  }

  

  // for (int j = 0; j < ht->capacity; j++){
  //   //is there a linked list?
  //   if (ht->storage[j]->next == NULL){
  //     destroy_pair(ht->storage[j]);
  //   } else {
  //     //since there is a linked list we need to destroy them all
  //     LinkedPair *next;
  //     LinkedPair *current = ht->storage[j];
  //     while(current->next != NULL){
  //       if (current->next == NULL){
  //         break;
  //       }
  //       next = current->next;
  //       destroy_pair(current->next);
  //       current = next;
  //     }
  //   }
  // }
  // destroy_hash_table(ht);
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
  // printf("%s", hash_table_retrieve(ht, "line_2"));
  // printf("%s", hash_table_retrieve(ht, "line_3"));

  // int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  // int new_capacity = ht->capacity;

  // printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  // destroy_hash_table(ht);

  return 0;
}
#endif
