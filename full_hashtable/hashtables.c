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
  // allocate memory for the new instance of the HashTable struct
  HashTable *ht = malloc(sizeof(HashTable));

  // set initial value for capacity
  ht->capacity = capacity;

  // allocate memory for storage
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
  // create an index by hashing the key
  unsigned int index = hash(key, ht->capacity);

  // if there is nothing in that space
  if (ht->storage[index] == NULL)
  {
    // create a key/value pair in that space
    ht->storage[index] = create_pair(key, value);
  }
  // else, if there is something occupying that space
  else
  {
    // and if the keys match
    if (strcmp(ht->storage[index]->key, key) == 0)
    {
      // and if the values match
      if (strcmp(ht->storage[index]->value, value) == 0) {
        // print an error stating that the key/value pair already exists and exit
        fprintf(stderr, "Key '%s' and value '%s' already exist.\n", key, value);
        exit(1);
      }
      // else if the values do not match
      else
      {
        // print a warning stating that the key will be overwritten
        printf("Previous key is being overwritten.\n");

        // create a pointer to the next key/value pair
        LinkedPair *prev_next = ht->storage[index]->next;

        // then free the occupied space
        destroy_pair(ht->storage[index]);

        // create a new key/value pair and insert it into that space
        ht->storage[index] = create_pair(key, value);

        // assign its next pointer the value of the previously next key/value pair
        ht->storage[index]->next = prev_next;
      }
    }
    // else if the keys do not match
    else
    {
      // create pointers to the current and next pair
      LinkedPair *curr_pair = ht->storage[index];
      LinkedPair *next_pair = curr_pair->next;

      // cycle through the pairs until your next pair is a NULL pointer
      while(next_pair != NULL)
      {
        curr_pair = next_pair;
        next_pair = curr_pair->next;
      }

      // create a new key/value pair and insert it as the next pair
      curr_pair->next = create_pair(key, value);
    }
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
  // create an index by hashing the key
  unsigned int index = hash(key, ht->capacity);

  // if there exists a LinkedPair pointer in that space
  if (ht->storage[index] != NULL)
  {
    // and if the keys match
    if (strcmp(ht->storage[index]->key, key) == 0)
    {
      // create a pointer to the next key/value pair
      LinkedPair *prev_next = ht->storage[index]->next;

      // then free the occupied space
      destroy_pair(ht->storage[index]);

      // assign in its place the value of the previously next key/value pair
      ht->storage[index] = prev_next;
    }
    // else if the keys do not match
    else
    {
      // create pointers to the previous and current pair
      LinkedPair *prev_pair = ht->storage[index];
      LinkedPair *curr_pair = ht->storage[index]->next;

      // cycle through the pairs until your curr pair is a NULL pointer
      // or the keys match
      while(curr_pair != NULL && strcmp(curr_pair->key, key) != 0)
      {
        prev_pair = curr_pair;
        curr_pair = prev_pair->next;
      }

      // if current pair is not null, a matching key was found
      if (curr_pair != NULL)
      {
        // assign the previous pair's next to point to current pair's next
        // and destroy the current pair
        prev_pair->next = curr_pair->next;
        destroy_pair(curr_pair);
      }
      // else key does not exist
      // print an error message stating such and exit
      else
      {
        fprintf(stderr, "Key '%s' does not exist.\n", key);
        exit(1);
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

  destroy_hash_table(ht);

  return 0;
}
#endif
