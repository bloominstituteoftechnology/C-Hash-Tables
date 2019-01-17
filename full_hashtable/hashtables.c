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
  printf("destroy pair working so far\n");
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
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair));

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
  LinkedPair *linked_pair = create_pair(key, value);
      printf("TOP linked pair key, value, next: %s %3s %3s\n", linked_pair->key, linked_pair->value, linked_pair->next);
  // if  a LinkedPair is already stored at this index
  if (ht->storage[hash_index] != NULL){
    // if this LinkedPair has the same key, overwrite the value only
    // no need to modify key or the next pair. key and current's pair LinkedPair may remain as they are.
    if (strcmp(ht->storage[hash_index]->key, key) == 0){
      printf("\nWarning: overwriting previous Pair with current Pair\n");
      printf("PREVIOUS key, value, next: %s %3s %3s\n", ht->storage[hash_index]->key, ht->storage[hash_index]->value, ht->storage[hash_index]->next);
      ht->storage[hash_index]->value = value;
      printf("OVERWRITTEN linked pair key, value, next: %s %3s %3s\n", linked_pair->key, linked_pair->value, linked_pair->next);
    }
    // if keys are not the same, check if there's a linked pair
    else if (ht->storage[hash_index]->next == NULL) {
      // there is no Linked Pair, so link it
      printf("there is no Linked Pair, so link it\n\n");
      ht->storage[hash_index]->next = linked_pair; //???
    }
    // there is a Linked Pair. find the next unlinked pair, and assign it there
    else {
      LinkedPair *current_pair = ht->storage[hash_index];
      while(current_pair->next != NULL){
        if (strcmp(current_pair->next->key, key)==0){
          printf("got it, overwrite linked pair with same key\n");
          current_pair->next = linked_pair;
          return;
        } else { current_pair = current_pair->next; }
      }
        current_pair->next = linked_pair;




      
      // printf("I will never run \n\n");
      // LinkedPair *current_pair = ht->storage[hash_index]->next;
      // printf("FIRST ELSE  current pair key, value, next: %s %3s %3s\n\n", current_pair->key, current_pair->value, current_pair->next);
      // while (current_pair->next != NULL){
      //   printf("running");
      //   current_pair = current_pair->next;
      // } //when current_pair's next value is finally NULL, assign the linked_pair to it
      // current_pair = linked_pair;
    }

  }
  //the current index is emtpy. assign the linked_pair
  else {
    printf("Standard printing\n");
    ht->storage[hash_index] = linked_pair;
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
  unsigned int hash_index = hash(key, ht->capacity);

  LinkedPair *current_pair = ht->storage[hash_index];

  if (current_pair == NULL){
    printf("nothing to remove\n");
  } else {
    //while keys do not match and there's still a next pair
    while (strcmp(current_pair->key, key) != 0 && current_pair->next != NULL){
      current_pair = current_pair->next;
    }
    if (strcmp(current_pair->key, key) == 0) {
      destroy_pair(current_pair);
      current_pair = NULL;
    } else {
    printf("nothing to remove\n");
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
  while (current_pair != NULL) {
    if (strcmp(current_pair->key, key) == 0){
      return current_pair->value;
    } else {
      current_pair = current_pair->next;
    }
  }

  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  printf("destroying hash table\n");
  printf("ht capacity: %s", ht->capacity);
  for (int i=0; i<ht->capacity; i++){
    printf("round: %d", i);
    printf("%s\n", ht->storage[i]->value);
    LinkedPair *current_pair = ht->storage[i];
    LinkedPair *next_pair;
    printf("haseeing\n");
    while (current_pair->next != NULL){
      next_pair = current_pair->next;
      destroy_pair(current_pair);
      free(current_pair->next);
      current_pair = next_pair;
    }
     printf("here\n");
    destroy_pair(current_pair);
  }
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
  new_ht = malloc(sizeof(HashTable));
  new_ht->capacity = ht->capacity*2;
  new_ht->storage = calloc(new_ht->capacity, sizeof(LinkedPair));

  for (int i=0; i<ht->capacity; i++){
    printf("resizing round: %d\n", i);
    LinkedPair *current_pair = ht->storage[i];
    LinkedPair *next_pair;
    while(current_pair->next != NULL){
      printf("\n while loop \n");
      next_pair = current_pair->next;
      hash_table_insert(new_ht, current_pair->key, current_pair->value);
      printf("aqui");
      printf("%s", hash_table_retrieve(new_ht, current_pair->key));
      // printf(hash_table_retrieve(new_ht, current_pair->key);
      printf("now here?\n");
      // free(current_pair->next);

      destroy_pair(current_pair);
      current_pair = next_pair;
    }
    printf("\n NO while loop \n");
    hash_table_insert(new_ht, current_pair->key, current_pair->value);
    destroy_pair(current_pair);
    free(current_pair->next);

  }

  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "line_1", "Tiny hash table\n");
  // hash_table_insert(ht, "line_1", "HUGE hash table\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));
  // // printf("%s", ht->storage[1]->next->value);
  // printf("really ignroiong me like this");

  int old_capacity = ht->capacity;
  printf("\n\nhere\n");
  ht = hash_table_resize(ht);
  printf("\nyo\n\n");
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);
  printf("\n ht->capacity: %d\n", ht->capacity);
  destroy_hash_table(ht);

  return 0;
}
#endif
