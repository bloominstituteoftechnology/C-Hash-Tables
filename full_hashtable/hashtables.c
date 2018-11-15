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
  ht->capacity = capacity; 
  ht->storage = calloc(capacity, sizeof(LinkedPair));

  return ht;
}

/****
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in the existing LinkedPair list.
 ****/
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  LinkedPair * newLinkedPair = malloc(sizeof(LinkedPair));
  newLinkedPair = create_pair(key, value);
  int hashKey = hash(key, ht->capacity);
  if(ht->storage[hashKey] == NULL){
    ht->storage[hashKey] = newLinkedPair;
  } else {
    LinkedPair *current =  ht->storage[hashKey];
    while(current->next != NULL){
      current = current->next;
    }
    current->next = newLinkedPair;
    // newLinkedPair->next = ht->storage[hashKey];
    // ht->storage[hashKey] = newLinkedPair;
  }
}

int string_length(char s[])
{
    int count = 0;
    for (int i = 0; ; i++){
        if(s[i] == '\0'){
            return count;
        }
        count += 1;
    };
}

int string_compare(char *m, char *n)
{

    //loop through and compare the nth index of each until there is a difference.  
    //if the value of n is closer to a than m return 1 // m=aaaa
    //if the value of m is closer to a than n return -1 // n=aaaab
    //if the value of each is the same throughout return 0
    int lenM = string_length(m);
    int lenN = string_length(n);

    int biggerLen;

    if(lenM > lenN){
        biggerLen = lenM;
    } else {
        biggerLen = lenN;
    }
    for ( int i = 0; i < biggerLen ; i++){
        if(m[i] != n[i]){
            return m[i] - n[i];
        };
    }
    return 0;
}

/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
  int hashKey = hash(key, ht->capacity);
  if(ht->storage[hashKey] == NULL){
    printf("This spot in memory does not exist");
  } else {
    if(ht->storage[hashKey]->key == key){
      if(ht->storage[hashKey]->next == NULL){
        ht->storage[hashKey] = NULL;
      } else {
        LinkedPair *parent = ht->storage[hashKey];
        while(string_compare(parent->next->key, key) != 0  && parent->next != NULL){
            parent = parent->next;
        }
        if(parent->next == NULL){
          printf("the key was not found.");
        } else {
          LinkedPair *toBeDeleted = parent->next;
          if(toBeDeleted->next != NULL){
            parent->next = parent->next->next;
          } else {
            parent->next = NULL;
          }
          destroy_pair(toBeDeleted);
        }
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
  int hashKey = hash(key, ht->capacity);
  LinkedPair *pair = ht->storage[hashKey];
  while(string_compare(pair->key, key) != 0 && pair != NULL){
    pair = pair->next;
    printf("");
  }

  if(pair->next == NULL){
    return NULL;
  } else {
    return pair->value;
  }
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
  printf("first insert\n");
  hash_table_insert(ht, "line_2", "Filled beyond capacity\n");
  printf("second insert\n");
  hash_table_insert(ht, "line_3", "Linked list saves the day!\n");
  printf("third insert\n");

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
