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
  HashTable *ht;
  ht = (HashTable *)malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = (LinkedPair**) calloc(capacity, sizeof(LinkedPair*));

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
  unsigned int hashed_index = hash(key, ht->capacity);

  if(ht->storage[hashed_index])                             // If a LinkdePair already exists for that hashIndex
  {
    LinkedPair *currentPair = ht->storage[hashed_index];

    while(currentPair){                                     // Loop throught the linked list

      if(strcmp(currentPair->key, key) == 0){               // If we've found the key

        currentPair->value = value;
        currentPair = NULL;

      }
      else if(currentPair->next){                           // If theres a next value in the linked list

        currentPair = currentPair->next;                    // proceed to the next value

      }
      else{ 

        currentPair->next = create_pair(key,value);

      }
    }
    
  }else{
    ht->storage[hashed_index] = create_pair(key, value);
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
  unsigned int hashed_index = hash(key, ht->capacity);

  LinkedPair *currentPair = ht->storage[hashed_index];
  LinkedPair *last_pair = currentPair;

  while (currentPair != NULL && strcmp(currentPair->key, key) != 0) {

    last_pair = currentPair;
    currentPair = last_pair->next;

  }
  if (currentPair != NULL) {                          // If we've found the remove key

    if(currentPair == last_pair){                      // If we're deleting the first item in the LL

      ht->storage[hashed_index] = currentPair->next;  // Reassign the bucket to be the next item in the LL (or NULL if no next item)

    }

    last_pair->next = currentPair->next;              // Else we assign the previous LL element's next to be the element after the deleted element (or NULL if no next element)
    destroy_pair(currentPair);


  } else {                                            // currentPair at the end of list, no key match

    printf("No matching key, can't delete\n");

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
  unsigned int hashed_index = hash(key, ht->capacity);

  LinkedPair *currentPair = ht->storage[hashed_index];

  while(currentPair && strcmp(currentPair->key, key) != 0){

    currentPair = currentPair->next;

  }

  if(currentPair != NULL){

    return currentPair->value;

  }else{

    return NULL;

  }
  
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  for(int i = 0; i < ht->capacity; i++)
  { 
    if(ht->storage[i]){

      LinkedPair *currentPair = ht->storage[i];
      LinkedPair *previousPair;

      while(currentPair){

        previousPair = currentPair;
        currentPair = currentPair->next;
        destroy_pair(previousPair);

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
  HashTable *new_ht;

  new_ht = (HashTable *)malloc(sizeof(HashTable));
  new_ht->capacity = ht->capacity * 2;
  
  // Doesn't initialize all values to Null, causing errors with destroy_hash_table
  // new_ht->storage = (LinkedPair**) realloc(ht->storage, sizeof(LinkedPair*) * new_ht->capacity);

  new_ht->storage = (LinkedPair**) calloc(new_ht->capacity, sizeof(LinkedPair*));

  
  for(int i = 0; i < ht->capacity; i++)
  {
    new_ht->storage[i] = ht->storage[i];
    ht->storage[i] = NULL;
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

  printf("%s\n", hash_table_retrieve(ht, "line_1"));
  printf("%s\n", hash_table_retrieve(ht, "line_2"));
  printf("%s\n", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);
  
  hash_table_insert(ht, "line_4", "line 4\n");
  hash_table_insert(ht, "line_5", "line 5\n");
  hash_table_insert(ht, "line_6", "line6\n");

  printf("%s\n", hash_table_retrieve(ht, "line_4"));
  printf("%s\n", hash_table_retrieve(ht, "line_5"));
  printf("%s\n", hash_table_retrieve(ht, "line_6"));

  destroy_hash_table(ht);

  return 0;
}
#endif
