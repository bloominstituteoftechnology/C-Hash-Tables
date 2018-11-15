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
  the value in th existing LinkedPair list.
 ****/
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  unsigned int hashvalue = hash(key,ht->capacity);

  if(ht->storage[hashvalue] == 0){
    ht->storage[hashvalue] = create_pair(key,value);
    return;
  };
  LinkedPair *compareval = ht->storage[hashvalue];
  if(strcmp(compareval->key, key) == 0){
    compareval->value = value;
  }
  else{
    LinkedPair *nextup =  compareval->next;
    LinkedPair *prev = compareval;
    while(nextup != NULL){
      if(strcmp(nextup->key, key) == 0){
        nextup->value = value;
        return;
      }
      prev = nextup;
      nextup = nextup->next;
    }
    prev->next = create_pair(key,value);
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
  unsigned int hashvalue = hash(key,ht->capacity);
  LinkedPair *compareval = ht->storage[hashvalue];

  if(compareval == 0){
    printf("This key doesn't excist");
  };
  if(strcmp(compareval->key, key) == 0){
    if(compareval->next != NULL){
      LinkedPair *temp = compareval->next;
      destroy_pair(compareval);
      ht->storage[hashvalue] = temp; 
      return;
    }
    else{
      destroy_pair(compareval);
      ht->storage[hashvalue] = NULL;
      return;
    }
  }
  else{
    LinkedPair *nextup =  compareval->next;
    LinkedPair *prev = compareval;
    while(nextup != NULL){
      if(strcmp(nextup->key, key) == 0){
        if(nextup->next==NULL){
          prev->next = NULL;
        }
        else{
          prev->next = nextup->next;
        }
        destroy_pair(nextup);
        return;
      }
      prev = nextup;
      nextup = nextup->next;
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
  unsigned int hashvalue = hash(key,ht->capacity);
    
  if(ht->storage[hashvalue] != NULL){
      

    LinkedPair *compareval = ht->storage[hashvalue];
    if(strcmp(compareval->key, key) == 0){
      return compareval->value;
    }
    LinkedPair *nextup =  compareval->next;
    while(nextup != NULL){
      if(strcmp(nextup->key, key) == 0){
        return nextup->value;
      }
      else{
        nextup = nextup->next;
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
   for(int i = 0; i < ht->capacity; i++)
  {
    if(ht->storage != 0){
      LinkedPair *nextup =  ht->storage[i]->next;
      while(nextup != NULL){
        LinkedPair *temp = nextup->next;
        destroy_pair(nextup); 
        nextup = temp;
      
    };
    destroy_pair(ht->storage[i]);
  };
  
}
if(ht != 0){
    free(ht);
  };
}

/****
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 ****/
HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht = create_hash_table(ht->capacity*2);
  new_ht->capacity=ht->capacity *2;

  for(int i = 0; i < ht->capacity; i++){
    if(ht->storage[i] != 0){
      hash_table_insert(new_ht,ht->storage[i]->key,ht->storage[i]->value);
      LinkedPair *nextup =  ht->storage[i]->next;
      while(nextup != NULL){
        hash_table_insert(new_ht,nextup->key,nextup->value);
        nextup=nextup->next;
      }
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
