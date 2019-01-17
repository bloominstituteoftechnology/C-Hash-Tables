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

// helper function to print out storage
void ht_print(HashTable *ht)
{
  printf("\nSTART printing storage:\n");
  LinkedPair *curr_pair;

  for (int i = 0; i < ht->capacity; i++){
    if (ht->storage[i] != NULL)
    {
      printf("At index %d:\n", i);
      curr_pair = ht->storage[i];

      while(curr_pair != NULL)
      {
        printf("Key: %s, Value: %s\n", curr_pair->key, curr_pair->value);
        curr_pair = curr_pair->next;
      }
    }
  }

  printf("END printing storage. Capacity was %d. Count was: %d\n", ht->capacity, ht->count);
}

/****
  Fill this in.

  All values in storage should be initialized to NULL
 ****/
HashTable *create_hash_table(int capacity)
{
  // allocate memory for the new instance of the HashTable struct
  HashTable *ht = malloc(sizeof(HashTable));

  // set initial value for capacity and count
  ht->capacity = capacity;
  ht->count = 0;

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

    // increment count
    ht->count++;
  }
  // else, if there is something occupying that space
  else
  {
    // and if the keys match
    if (strcmp(ht->storage[index]->key, key) == 0)
    {
      // print a warning stating that the key will be overwritten
      printf("Previous key is being overwritten.\n");

      // create a pointer to the next key/value pair
      LinkedPair *prev_next = ht->storage[index]->next;

      // then free the occupied space
      destroy_pair(ht->storage[index]);

      // create a new key/value pair and insert it into that space
      ht->storage[index] = create_pair(key, value);

      //increment count
      ht->count++;

      // assign its next pointer the value of the previously next key/value pair
      ht->storage[index]->next = prev_next;
    }
    // else if they keys do not match
    else
    {
      // create pointers to the previous and current pairs
      LinkedPair *prev_pair = ht->storage[index];
      LinkedPair *curr_pair = ht->storage[index]->next;

      // cycle through the pairs until your curr pair is a NULL pointer or the keys match
      while(curr_pair != NULL && strcmp(curr_pair->key, key) != 0)
      {
        prev_pair = curr_pair;
        curr_pair = prev_pair->next;
      }

      // if current pair is not NULL, a matching key was found
      if (curr_pair != NULL)
      {
        // print a warning stating that the key will be overwritten
        printf("Previous key is being overwritten.\n");

        // create a pointer to the current pair's next value
        LinkedPair *curr_pair_next = curr_pair->next;

        // then free the occupied space
        destroy_pair(curr_pair);

        // create a new key/value pair
        LinkedPair *new_pair = create_pair(key, value);

        // increment count
        ht->count++;

        // assign previous pair's next value to this new pair
        // and assign this new pair's next value to what was the current pair's next value
        prev_pair->next = new_pair;
        new_pair->next = curr_pair_next;
      }
      // else if the current pair was NULL, a matching key was not found
      else
      {
        // create a new key/value pair and insert that into the previous pair's next value
        prev_pair->next = create_pair(key, value);

        // increment count
        ht->count++;
      }
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

      // decrement count
      ht->count--;
    }
    // else if the keys do not match
    else
    {
      // create pointers to the previous and current pair
      LinkedPair *prev_pair = ht->storage[index];
      LinkedPair *curr_pair = ht->storage[index]->next;

      // cycle through the pairs until your curr pair is a NULL pointer or the keys match
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

        // decrement count
        ht->count--;
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
  // create an index by hashing the key
  unsigned int index = hash(key, ht->capacity);

  // if there exists a LinkedPair pointer in that space
  if (ht->storage[index] != NULL)
  {
      // create a pointer to the current pair
      LinkedPair *curr_pair = ht->storage[index];

      // cycle through the pairs until your curr pair is a NULL pointer or the keys match
      while(curr_pair != NULL && strcmp(curr_pair->key, key) != 0)
      {
        curr_pair = curr_pair->next;
      }

      // if current pair is not null, a matching key was found
      if (curr_pair != NULL)
      {
        // return the value associated with that key
        return curr_pair->value;
      }
  }
  // if no LinkedPair pointer or matching key was found, return NULL
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
  // loop through all the pointers in storage
  for (int i = 0; i < ht->capacity; i++)
  {
    // if there exists a LinkedPair pointer in that space
    if (ht->storage[i] != NULL)
    {
      // set curr_pair to that pointer
      LinkedPair *curr_pair = ht->storage[i];
      LinkedPair *next_pair;

      // loop through any and all LinkedPairs until reaching a NULL pointer
      while(curr_pair != NULL)
      {
        // for each loop,
        // point next_pair to curr_pair's next value
        next_pair = curr_pair->next;
        // destroy current pair
        destroy_pair(curr_pair);
        // make curr_pair now point to what was curr_pair's previous next pair
        curr_pair = next_pair;
      }
    }
  }

  // free up storage
  if (ht->storage != NULL)
  {
    free(ht->storage);
  }

  // free up the hash table
  if (ht != NULL)
  {
    free(ht);
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
  // new capacity will be twice previous capacity
  int new_capacity = ht->capacity * 2;

  // create a new ht
  HashTable *new_ht = create_hash_table(new_capacity);

  // loop through all the pointers in original ht storage
  for (int i = 0; i < ht->capacity; i++)
  {
    // if there exists a LinkedPair pointer in that space
    if (ht->storage[i] != NULL)
    {
      // set curr_pair to that pointer
      LinkedPair *curr_pair = ht->storage[i];

      // loop through any and all LinkedPairs until reaching a NULL pointer
      while(curr_pair != NULL)
      {
        // for each loop,
        // insert curr_pair's key/value into new ht
        hash_table_insert(new_ht, curr_pair->key, curr_pair->value);

        // make curr_pair now point to its next pair
        curr_pair = curr_pair->next;
      }
    }
  }

  // destroy the old ht
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

  ht_print(ht);

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  ht_print(ht);

  destroy_hash_table(ht);

  return 0;
}
#endif
