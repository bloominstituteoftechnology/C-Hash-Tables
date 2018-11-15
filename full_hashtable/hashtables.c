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
  pair->key = key;
  pair->value = value;
  pair->next = NULL;

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) free(pair);
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
  ht-> capacity = capacity; 
  ht-> storage = calloc(capacity, sizeof(LinkedPair *)); 

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
  
  int hashIndex = hash(key, ht->capacity); 
  LinkedPair *linked_node = ht->storage[hashIndex]; 
  if(linked_node == NULL){
    printf("linked node created in head Index: %d \n", hashIndex); 
    linked_node = create_pair(key, value); 
    ht->storage[hashIndex] = linked_node; 
  }else{
    int count = 1; 
    while(linked_node != NULL){
      if(linked_node->next != NULL){
        count += 1; 
        linked_node = linked_node-> next;
      }else{
        break; 
      }
    }
    printf("Count %d \n", count); 
    printf("linked node created in tail Index: %d \n", hashIndex); 
    linked_node-> next = create_pair(key, value); 
  }
}
  // if the storage at the hash index is NULL, then the pair can go into the linked list, resembling both the head and tail which points to NULL
  // if the storage at the hash index is not NULL, then need to loop through each item, check if the next is NULL
  // if next is not NULL then move to the next linked list pair
  // when next is NULL, set the linked pair to the next linked list value. 

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
  int hashIndex = hash(key, ht-> capacity); 
  LinkedPair *linked_node = ht->storage[hashIndex]; 
  LinkedPair *previous = NULL; 
  if(linked_node == NULL){
    printf("No values found here. No key match to remove"); 
  }else{
    while(linked_node-> next != NULL){
      if(linked_node-> key != key){
        previous = linked_node; 
        linked_node = linked_node-> next; 
      }else{
        previous-> next = linked_node-> next; 
        break; 
      }
    }
    printf("Removing %s \n", linked_node->key); 
    destroy_pair(linked_node); 
  }
  // First, use hash function with key to find index at which key would be located
  // While the linked list pair is not NULL, loop through each item and do a string compare for each one
  // Save previous linked list item
  // if key is found set the next pair to the previous pair -> next 
  // then destroy the pair 
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 ****/
char *hash_table_retrieve(HashTable *ht, char *key)
{
  int hashIndex = hash(key, ht-> capacity); 
  LinkedPair *linked_node = ht-> storage[hashIndex]; 
  if(linked_node == NULL){
    return NULL; 
  }else{
    while(linked_node != NULL){
      if(linked_node-> key != key){
        linked_node = linked_node-> next; 
      }else{
        return linked_node-> value; 
      }
    }
  }
  return NULL;
}

// Implementation similar to the delete, should return key 
// First, use hash function with key to find index at which key would be located
// While the linked list pair is not NULL, loop through each item and do a string compare for each one
// If key is found, then return. Otherwise return NULL



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

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");
  hash_table_insert(ht, "line_4", "This should get interesting!\n"); 
  hash_table_insert(ht, "line_5", "Testing to make sure linked list is properly working!\n"); 

  hash_table_remove(ht, "line_4"); 

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
