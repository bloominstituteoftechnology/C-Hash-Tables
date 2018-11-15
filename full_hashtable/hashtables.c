#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//https://repl.it/@codejoncode/Full-Hash-Table

/*Structures  Begin*/
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
/*Structures End*/

/*Prototypes Begin*/
LinkedPair *create_pair(char *key, char *value);
void destroy_pair(LinkedPair *pair); 
unsigned int hash(char *str, int max);
HashTable *create_hash_table(int capacity);
void hash_table_insert(HashTable *ht, char *key, char *value);
void hash_table_remove(HashTable *ht, char *key);
char *hash_table_retrieve(HashTable *ht, char *key);
void destroy_hash_table(HashTable *ht);
HashTable *hash_table_resize(HashTable *ht);

/*Prototypes End*/

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
  ht->capacity = capacity; 

  ht->storage = calloc(ht->capacity, sizeof(LinkedPair*));

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
    //get a hash 
    unsigned int hash_index = hash(key, ht->capacity);
    //create a LinkedPair 
    LinkedPair *new_pair = create_pair(key, value);

    //check if something exists at the hash 
    if (ht->storage[hash_index] == NULL){
      //if nothing exists  set the hash to new pair 
      ht->storage[hash_index] = new_pair;
    } else {
      //if something exists  take the pair.next and have equal to new pair 
      // ht->storage[hash_index]->next = pair; 
      //^Not as simple as that. What if there is  already a next? and then a next after that. 
      LinkedPair *linked_pair = ht->storage[hash_index];
      //create a loop to check for a null value  if null set the new pair else go to the next 
      while(linked_pair->next != NULL){
        linked_pair = linked_pair->next; 
      }
      linked_pair->next = new_pair; 

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
  //get hash 
  unsigned int  hash_index = hash(key, ht->capacity);
  //create parent  if not null 
  LinkedPair *parent = NULL; 

  //Check if the index point of the storarage is null 
  if(ht->storage[hash_index] == NULL){
    printf("ERROR: The item is not in the hash table");
  }else {
    parent = ht->storage[hash_index]; 
  }

  //loop through checking for the next node's key, 
  //if it matches we have the item we plan to delete 
  while(strcmp(parent->next->key, key) != 0 && parent->next != NULL){
      parent = parent->next; 
  }

  if(parent->next != NULL){
    LinkedPair *deleting = parent->next; 
    LinkedPair *next_next = NULL; 
    if(deleting->next != NULL){
      next_next = deleting->next; 
    }

    parent->next = next_next; 
    destroy_pair(deleting);
    
  } else {
    printf("ERROR  ITEM NOT FOUND"); 
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
  // get hash 
  unsigned int hash_index = hash(key, ht->capacity);

  //check if null else start loop 

  LinkedPair *current = NULL;
  char *value = NULL; 
  if(ht->storage[hash_index] == NULL){
    // printf("ERROR: key does not exsit");
    return value; 
  } else {
    while(strcmp(current->key, key) != 0 && current != NULL){
      current = current->next; 
    }
    value = current->value; 
  }

  return value; 
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  for(int i = 0; i< ht->capacity; i++){
    if(ht->storage[i] != NULL){
      destroy_pair(ht->storage[i]); 
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
  HashTable *new_ht = create_hash_table(ht->capacity *2); 
  
  for(int i = 0; i<ht->capacity; i++){

    LinkedPair *current = NULL; 
    if(ht->storage[i] != NULL){
      current = ht->storage[i];
      do {
        hash_table_insert(new_ht, current->key, current->value);
        current = current->next;
      }
      while(current != NULL); 
    }
  }

  //destroy after we are done
  destroy_hash_table(ht); 


  return new_ht;
}