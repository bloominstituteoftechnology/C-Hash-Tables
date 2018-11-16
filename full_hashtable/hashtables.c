#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//https://repl.it/@codejoncode/Full-Hash-Table

//Structures  Begin
// Hash table key/value pair with linked list pointer

typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;


// Hash table with linked pairs

typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;
// Structures End

// Prototypes Begin
LinkedPair *create_pair(char *key, char *value);
void destroy_pair(LinkedPair *pair); 
unsigned int hash(char *str, int max);
HashTable *create_hash_table(int capacity);
void hash_table_insert(HashTable *ht, char *key, char *value);
void hash_table_remove(HashTable *ht, char *key);
char *hash_table_retrieve(HashTable *ht, char *key);
void destroy_hash_table(HashTable *ht);
HashTable *hash_table_resize(HashTable *ht);

// Prototypes End

#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  hash_table_insert(ht, "key-0", "new-val-0");
  hash_table_insert(ht, "key-1", "new-val-1");
  hash_table_insert(ht, "key-2", "new-val-2");
  hash_table_insert(ht, "key-3", "new-val-3");
  hash_table_insert(ht, "key-4", "new-val-4");
  hash_table_insert(ht, "key-5", "new-val-5");
  hash_table_insert(ht, "key-6", "new-val-6");
  hash_table_insert(ht, "key-7", "new-val-7");
  hash_table_insert(ht, "key-8", "new-val-8");
  hash_table_insert(ht, "key-9", "new-val-9");

  printf("%s\n", hash_table_retrieve(ht, "key-0"));
  printf("%s\n", hash_table_retrieve(ht, "key-1"));
  printf("%s\n", hash_table_retrieve(ht, "key-2"));
  printf("%s\n", hash_table_retrieve(ht, "key-3"));
  printf("%s\n", hash_table_retrieve(ht, "key-4"));
  printf("%s\n", hash_table_retrieve(ht, "key-5"));
  printf("%s\n", hash_table_retrieve(ht, "key-6"));
  printf("%s\n", hash_table_retrieve(ht, "key-7"));
  printf("%s\n", hash_table_retrieve(ht, "key-8"));
  printf("%s\n", hash_table_retrieve(ht, "key-9"));

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


  // Create a key/value linked pair to be stored in the hash table.
 
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = key;
  pair->value = value;
  pair->next = NULL;

  return pair;
}


// Use this function to safely destroy a hashtable pair.

void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) free(pair);
}


  // djb2 hash function

  // Do not modify this!

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


  // Fill this in.

  // All values in storage should be initialized to NULL

HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable)); 
  ht->capacity = capacity; 

  ht->storage = calloc(ht->capacity, sizeof(LinkedPair*));

  return ht;
}


  // Fill this in.

  // Inserting values to the same index with different keys should be
  // added to the corresponding LinkedPair list.

  // Inserting values to the same index with existing keys can overwrite
  // the value in th existing LinkedPair list.
 
void hash_table_insert(HashTable *ht, char *key, char *value)
{

    LinkedPair * newLinkedPair = create_pair(key, value);
    int hashKey = hash(key, ht->capacity);
    LinkedPair *current =  ht->storage[hashKey];
    if(current == NULL){
      ht->storage[hashKey] = newLinkedPair;
    } else {
      while(current != NULL){
        if(strcmp(current->key, key) ==0 ){
          current->value = value;
          break;
        } else if (current->next == NULL){
          current->next = newLinkedPair;
          break;
        } else {
          current = current->next;
        }
      }
      // newLinkedPair->next = ht->storage[hashKey];
      // ht->storage[hashKey] = newLinkedPair;
    }

}

  // Fill this in.

  // Should search the entire list of LinkedPairs for existing
  // keys and remove matching LinkedPairs safely.

  // Don't forget to free any malloc'ed memory!
 
void hash_table_remove(HashTable *ht, char *key)
{
    int hashedKey = hash(key, ht->capacity);
    LinkedPair *pair = ht->storage[hashedKey];
    LinkedPair *trail = NULL;
    while(pair != NULL && strcmp(pair->key, key) != 0){
      trail = pair;
      pair = pair->next;
    }
    if(pair == NULL){
      printf("%s is not a key", key);
      return;
    }
    if(trail == NULL){
      ht->storage[hashedKey] = pair->next;
    }else{
      trail->next = pair->next;
    }

}

  // Fill this in.

  // Should search the entire list of LinkedPairs for existing
  // keys.

  // Return NULL if the key is not found.
 
char *hash_table_retrieve(HashTable *ht, char *key)
{
  // get hash 
  unsigned int hash_index = hash(key, ht->capacity);

  //check if null else start loop 

  LinkedPair *current = NULL;
  if(ht->storage[hash_index] == NULL){
    printf("ERROR: key does not exsit");
    return NULL; 
  } else {
    current = ht->storage[hash_index]; 
    
    while(current != NULL){
      if(strcmp(current->key, key) == 0){
          printf("281\n"); 
          return current->value; 
      }
      printf("284\n");
      current = current->next; 
    }
 
  }

  return NULL; 
}

  // Fill this in.

  // Don't forget to free any malloc'ed memory!

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


  // Fill this in.

  // Should create a new hash table with double the capacity
  // of the original and copy all elements into the new hash table.

  // Don't forget to free any malloc'ed memory!
 
HashTable *hash_table_resize(HashTable *ht)
{
  int new_capacity = 0; 
  for (int i = 0; i<ht->capacity; i++){
    new_capacity++;
  } 
  for (int i = 0; i<ht->capacity; i++){
    new_capacity++;
  } 

  HashTable *new_ht = create_hash_table(new_capacity); 
  
  for(int i = 0; i<ht->capacity; i++){

    LinkedPair *current = NULL; 
    if(ht->storage[i] != NULL){
      current = ht->storage[i];
      do {
        printf("Inside of here"); 
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