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
  HashTable *ht;

  ht = malloc(sizeof(HashTable));
  ht->storage = calloc(capacity,sizeof(LinkedPair*));
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
  int index = hash(key,ht->capacity-1);
  
  if (ht->storage[index] != NULL){
    LinkedPair *current = ht->storage[index];
    //loop through LinkedList one by one
    while(current->next != NULL){

      if(strcmp(current->key, key) == 0){
        // insert value with existing key
        printf("%s was replaced with %s at key %s\n",ht->storage[index]->value,value,key );
        current->value = value;
        return;
      }
      //current key does not match insert key
      current = current->next; 
    }
    //we've reached end of linkedlist
    //insert value with new key at end of linked list
    LinkedPair *new_element = create_pair(key,value);
    current->next = new_element;
  } else {
    LinkedPair *new_element = create_pair(key,value);
    ht->storage[index] = new_element;
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
  int index = hash(key,ht->capacity-1);
  if (ht->storage[index] == NULL){
    //There is no key in hash table
    fprintf(stderr, "Key not found: no value with key %s\n", key);
    return;
  } 
  //array index is occuped so loop through linkedlist to find match
  
  LinkedPair *previous = NULL;
  LinkedPair *current = ht->storage[index];

  while(current != NULL){
    if(strcmp(current->key,key) == 0){
      //match found
      if(previous != NULL){
        //element is not head of linkedlist so fix links
        previous->next = current->next;
      }
      destroy_pair(current);
      return;
    }
    previous = current;
    current = current->next;
  }
  fprintf(stderr, "Key not found: no value with key %s\n",key);
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
char *hash_table_retrieve(HashTable *ht, char *key)
{

  int index = hash(key,ht->capacity-1);
  if (ht->storage[index] == NULL){
    //There is no key in hash table
    fprintf(stderr, "Key not found: no value with key %s\n", key);
    return NULL;
  } 
  //hash value found so loop to find match
  
  LinkedPair *current = ht->storage[index];

  while(current != NULL){
    if(strcmp(current->key,key) == 0){
      //match found
      return current->value;
    }
    current = current->next;
  }
  fprintf(stderr, "Key not found: no value with key %s\n",key);
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{

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

  hash_table_insert(ht, "line_1", "Tiny hash table");
  hash_table_insert(ht, "line_2", "Filled beyond capacity");
  hash_table_insert(ht, "line_3", "Sneaky");
  hash_table_insert(ht, "23", "Sneaky");
  hash_table_insert(ht, "line_4", "Linked list saves the day!");
  hash_table_remove(ht,"23");
  hash_table_remove(ht,"line_3");

  LinkedPair *current = ht->storage[0];
  while(current != NULL){
    printf("k:%s v:%s\n", current->key,current->value);
    current = current->next;
  }
  printf("%s\n", hash_table_retrieve(ht, "line_1"));
  printf("%s\n", hash_table_retrieve(ht, "line_2"));
  printf("%s\n", hash_table_retrieve(ht, "line_4"));

  // int old_capacity = ht->capacity;
  // ht = hash_table_resize(ht);
  // int new_capacity = ht->capacity;

  // printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  // destroy_hash_table(ht);

  return 0;
}
#endif
