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
// void hash_table_insert(HashTable *ht, char *key, char *value)
// {
//   LinkedPair * newLinkedPair = create_pair(key, value);
//   int hashKey = hash(key, ht->capacity);
//   LinkedPair *current =  ht->storage[hashKey];
//   while(current != NULL){
//     if(strcmp(current->key, key) == 0 ){
//       current = newLinkedPair;
//       break;
//     } else if (current->next == NULL){
//       current->next = newLinkedPair;
//       break;
//     } else {
//       current = current->next;
//     }
//   }
// }
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
  // if(ht->storage[hashKey] == NULL){
  //   printf("This spot in memory does not exist");
  // } else {
    // if(ht->storage[hashKey]->key == key){
    //   if(ht->storage[hashKey]->next == NULL){
    //     ht->storage[hashKey] = NULL;
    //   } else {
    //     LinkedPair *parent = ht->storage[hashKey];
    //     while(strcmp(parent->next->key, key) != 0  && parent->next != NULL){
    //         parent = parent->next;
    //     }
    //     if(parent->next == NULL){
    //       printf("the key was not found.");
    //     } else {
    //       LinkedPair *toBeDeleted = parent->next;
    //       if(toBeDeleted->next != NULL){
    //         parent->next = parent->next->next;
    //       } else {
    //         parent->next = NULL;
    //       }
    //       destroy_pair(toBeDeleted);
    //     }
    //   }
    // }

     // pointers to first pair at index and a trailing pointer
     LinkedPair * closerToEnd = ht->storage[hashKey];//is start 
     LinkedPair * closerToBeg = NULL;//but not the beginning 
     while (closerToEnd != NULL && strcmp(closerToEnd->key, key) != 0){
        closerToBeg = closerToEnd;
        closerToEnd = closerToEnd->next;
     }
     if(closerToEnd == NULL){
        printf("does not exist");
        return;
     }
     if(closerToBeg = NULL){
       ht->storage[hashKey] = closerToEnd->next;
     } else {//is match 
        closerToBeg->next = closerToEnd->next;//deleting 'closer to the end'
     }
    // loop thru pair looking for a matching key or null
    // check if pair is null, if it is, no matching key was found
    // check if trail is null, if it is, then change index to next
    // set trail next to equal pair next


  // }
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

  while(pair != NULL){
    if (strcmp(pair->key, key) == 0){
      return pair->value;
    } else {
      pair = pair->next;
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
  for(int i = 0; i < ht->capacity; i++){
    destroy_pair(ht->storage[i]);
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
  int newCap = ht->capacity;
  newCap = newCap * 2;
  HashTable *new_ht = create_hash_table(newCap);
  for(int i = 0; i < ht->capacity; i++){
    LinkedPair * current = ht->storage[i];
    if(ht->storage[i] != NULL){
      current = ht->storage[i];
      do {
        hash_table_insert(new_ht, current->key, current->value);
        current = current->next;
        printf("insert %d\n", i);
      } while(current != NULL);
    }
  }
  destroy_hash_table(ht);
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
  printf("retrieve 1\n");
  printf("%s", hash_table_retrieve(ht, "line_2"));
  printf("retrieve 2\n");
  printf("%s", hash_table_retrieve(ht, "line_3"));
  printf("retrieve 3\n");

  printf("%i", ht->capacity);
  int old_capacity = ht->capacity;
  printf("complete old capacity");
  ht = hash_table_resize(ht);
  printf("complete resize");
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);
  printf("destroyed hash table");
  return 0;
}
#endif
