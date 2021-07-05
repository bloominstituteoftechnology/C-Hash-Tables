#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Basic hash table key/value pair
 ****/
typedef struct Pair {
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable {
  int capacity;
  int count;
  Pair **storage;
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
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
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht;
  ht = malloc(sizeof(struct BasicHashTable));
  ht->capacity = capacity;
  ht->count = 0;
  ht->storage = malloc(capacity*sizeof(struct Pair*));
  for(int i=0; i<capacity; i++)
  {
    ht->storage[i] = NULL;
  }
  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
int is_same(char *key, char *old_key)
{
  int i;
  for(i=0; key[i]!='\0'; i++)
  {
    if(old_key[i]=='\0')
    {
      return 0;
    }
    if(key[i]!=old_key[i])
    {
      return 0;
    }
  }
  if(old_key[i] != '\0')
  {
    return 0;
  }
  return 1;
}

void resize_table(BasicHashTable *ht)
{
  int new_capacity = ht->capacity*2;
  Pair **new_storage = malloc(new_capacity*sizeof(struct Pair*));
  for(int i=0; i<new_capacity; i++)
  {
    new_storage[i] = NULL;
  }
  for(int i=0; i<ht->capacity; i++)
  {
    if(ht->storage[i] != NULL)
    {
      new_storage[i] = ht->storage[i];
    }
  }
  free(ht->storage);
  ht->storage = new_storage;
  ht->capacity *= 2;
}

void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  int index = hash(key, ht->capacity);
  if((ht->count*10)/ht->capacity >= 7)
  {
    resize_table(ht);
  }
  if(ht->storage[index] != NULL)
  {
    if(!is_same(key, ht->storage[index]->key))
    {
      printf("Warning: writing over a different key.\n");
    }
    destroy_pair(ht->storage[index]);
  }
  ht->storage[index] = create_pair(key, value);
  ht->count++;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  int index = hash(key, ht->capacity);
  destroy_pair(ht->storage[index]);
  ht->storage[index] = NULL;
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  int index = hash(key, ht->capacity);
  if(ht->storage[index] != NULL)
  {
    return ht->storage[index]->value;
  }
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  for(int i=0; i<ht->capacity; i++)
  {
    destroy_pair(ht->storage[i]);
  }
  free(ht->storage);
  free(ht);
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
