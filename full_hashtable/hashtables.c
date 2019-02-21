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
  int count;
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
    // free(pair->key);
    // free(pair->value);
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
  ht->count = 0;
  ht->storage = calloc(capacity, sizeof(LinkedPair*));
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
  unsigned int hash_index = hash(key, ht->capacity);
  LinkedPair *current_pair = ht->storage[hash_index];
  
  // Find the last pair, or the first pair with the given key, on the linked list that storage[hash_index] points to
  while (current_pair != NULL && strcmp(current_pair->key, key) != 0){
    current_pair = current_pair->next;
  }
  // if found an existing pair with the same key, overwrite the value
  if (current_pair != NULL){
    current_pair-> value = value;
  }
   //if no existing pair with the given key found, create a new pair with the given key and value, then insert the new pair to the front of the linked list
  else {     
    LinkedPair *new_pair = create_pair(key, value);
    new_pair->next = ht->storage[hash_index];
    ht->storage[hash_index] = new_pair;
  }
  ht->count++;
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int hash_index = hash(key, ht->capacity);
  LinkedPair *current_pair = ht->storage[hash_index];

  //Case 1: storage[hash_index] points to null -> no pair found
  if (!current_pair){
    printf("Case 1: No pair with the given key can be found.\n");
  }
  else if (current_pair && !current_pair->next){
    // Case 2: if storage[hash_index] points to a single pair, and the single pair does not contain the key given -> no pair found
    if (strcmp(current_pair->key, key) != 0){
      printf("Case 2: No pair with the given key can be found. \n");
    }
    //Case 3: if storage[hash_index] points to a single pair, and the single pair contains the key given -> destroy the pair and point storage[hash_index] to NULL;
    else{
      destroy_pair(current_pair);
      ht->storage[hash_index] = NULL;
      ht->count--;
      printf("Case 3: Pair with given key found; pair removed.\n");
    }
  }
  else{
    LinkedPair *previous_pair;
    // Find the last pair, or the first pair with the given key, on the linked list that storage[hash_index] points to
    int iteration = 0;
    while (current_pair && (strcmp(current_pair->key, key) != 0)){

      previous_pair = current_pair;
      current_pair = current_pair->next;
      iteration++;
    } 
    //Case 4: if current_pair passed end of list -> no pair found
    if (!current_pair){
      printf("Case 4: no pair is found with the given key.\n");
    }
    // Case 5: if found an existing pair with the same key, set next of previous pair to NULL then destroy current pair
    else {   
      if (iteration == 0){
        ht->storage[hash_index] = current_pair->next;
        // printf("%s\n", ht->storage[hash_index]->key);
      }
      else{
        previous_pair->next = current_pair->next;  
        // printf("previous next key: %s, current next key: %s\n", previous_pair->next->key, current_pair->next->key);      
      }
      printf("current key: %s, current value: %s\n", current_pair->key, current_pair->value);
      destroy_pair(current_pair);
      ht->count--;
      printf("Case 5: Pair with given key found; pair removed.\n");
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
  unsigned int hash_index = hash(key, ht->capacity);
  LinkedPair *current_pair = ht->storage[hash_index];

  while (current_pair && strcmp(current_pair->key, key) != 0){
    current_pair = current_pair->next;
  }

  //if storage[hash_index] points to NULL, return NULL
  if (!current_pair){
    return NULL;
  }
  
  // if found an existing pair with the same key, overwrite the value
  // if (strcmp(current_pair->key, key) == 0)  {      
  return current_pair->value;
  // }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  //iterate through the hash array, and for each hash_index, 
  for (int i=0; i<ht->capacity; i++){
    LinkedPair *current_pair = ht->storage[i];
    // LinkedPair *next_pair = current_pair->next;
    // if (current_pair)
    while (current_pair != NULL){
      destroy_pair(current_pair);
      current_pair = current_pair->next;
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
  //allocate memory for a new hash table and its components
  HashTable *new_ht = create_hash_table(ht->capacity*2);
  //loop through entire storage array, if storage[i] is pointing to a pair (i.e. not pointing to NULL), copy each pair in linked list to new hashtable

  for (int i=0; i<ht->capacity; i++){
      //declare new pair variables for linked list traversal
      if (ht->storage[i]){
        // printf("value: %s, capacity: %d\n", ht->storage[i]->value, new_ht->capacity);
        // printf("%d", hash(ht->storage[i]->key, new_ht->capacity));
        unsigned int new_index = hash(ht->storage[i]->key, new_ht->capacity);
        new_ht->storage[new_index] = ht->storage[i];   
      }
  }
  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  // hash_table_remove(ht, "line_0");
  
  // printf("%s", hash_table_retrieve(ht, "line_1"));

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  hash_table_remove(ht, "line_3");

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
