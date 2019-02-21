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
  HashTable *ht = malloc(sizeof(HashTable));
  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *));
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
  if(key == NULL || value == NULL){
    printf("Either the key or value is NULL");
  }
  int hashed = hash(key, ht->capacity);//creates an index value

    LinkedPair *p = create_pair(key, value); //creates a new pair
    if(ht->storage[hashed] == NULL){ //if index is already empty just place in there
      ht->storage[hashed] = p;
    }else{
      LinkedPair *head = ht->storage[hashed];
      int b_continue = 0;
      while(!b_continue){//use arrows notation not dot
        if(head->next == NULL){
          head->next = p;
          b_continue = 1;
        }
        head = head->next;
      }
    }
  printf("key = %s", ht->storage[hashed]->key);
  printf("\nvalue=%s\n", ht->storage[hashed]->value);
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
  int hashed = hash(key, ht->capacity);
  if(ht->storage[hashed] == NULL){
    printf("The value with that key is null");
  }else{
    if(ht->storage[hashed]->next == NULL){
      destroy_pair(ht->storage[hashed]); //destroys pair / free's all the malloc'd memory
      ht->storage[hashed] = NULL; //resets value back to NULL
    }else {// if there is more than one value in the list
      LinkedPair *head = ht->storage[hashed];
      while (head->next != NULL) {
        if (strcmp(head->next->key, key) == 0) {//compares the two keys to see if they match
          head->next = head->next->next;
          destroy_pair(head->next);
          return;
        }
        head = head->next;
      }
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
  int hashed = hash(key, ht->capacity);
  if(ht->storage[hashed] == NULL){
    return NULL;
  }else{
    if(ht->storage[hashed]->key == key){
      return ht->storage[hashed]->value;
    }else{
      LinkedPair *head = ht->storage[hashed];
      while(head->next != NULL){
        if(head->key == key){
          return head->value;
        }
        head = head->next;
      }
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

  printf("%s", hash_table_retrieve(ht, "line_1"));
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("%s", hash_table_retrieve(ht, "line_3"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  // destroy_hash_table(ht);

  return 0;
}
#endif
