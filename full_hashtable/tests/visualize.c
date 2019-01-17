#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    pair->key = NULL;
    pair->value = NULL;
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

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

HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable)); // allocate memory for a hash table;

  ht->capacity = capacity;
  ht->storage = calloc(capacity, sizeof(LinkedPair *)); // allocate the memory storage for a struct LinkedPair pointer;

  return ht;
}

void hash_table_insert(HashTable *ht, char *key, char *value)
{
  unsigned int target_index = hash(key, ht->capacity); // creates an array index for node to be inserted into
  LinkedPair *new_pair = create_pair(key, value); // creates a new node for insertion

  if (ht->storage[target_index] == 0) { // if the current index is empty, insert new_pair
    ht->storage[target_index] = new_pair;
  }
  else {
    while (ht->storage[target_index] != 0) { // if the current index is not empty, 3 things can happens
      // 1. check if the keys are the same, then overwrite existing value with the new value
      if (strcmp(ht->storage[target_index]->key, key) == 0) { // == or != compares base addresses, strcmp to compare values
        ht->storage[target_index]->value = value;
        break;
      }
      // 2. check if keys are different AND there's an empty "next" slot, insert new pair there
      else if (strcmp(ht->storage[target_index]->key, key) != 0 && ht->storage[target_index]->next == NULL) {
        ht->storage[target_index]->next = new_pair;
        break;
      }
      // 3. If neither of the terminating conditionals activates, must mean that it's a new linked list node to be attached
      // continue with while loop until there are matching keys or there's an empty "next" slot
      ht->storage[target_index] = ht->storage[target_index]->next;
    }
  }
}

void hash_table_remove(HashTable *ht, char *key)
{
  unsigned int target_index = hash(key, ht->capacity);
  LinkedPair *current = ht->storage[target_index]; // shortens the name

  while (current != 0) { // LOOP while there's a current node
    if (strcmp(current->key, key) == 0 && current->next == 0) { // EXIT 1 if strings are the same and there's NOT a linked node
      destroy_pair(current);
      break;
    }
    else if (strcmp(current->key, key) == 0 && current->next != 0) { // EXIT 2 if strings are same and there IS a linked node
      current->key = current->next->key;
      current->value = current->next->value;
      current->next = current->next->next;
      break;
    }
    current = current->next; // ITERATOR Move to the next linked node and continue with while loop to check
  }
}

char *hash_table_retrieve(HashTable *ht, char *key)
{
  unsigned int target_index = hash(key, ht->capacity);
  LinkedPair *current = ht->storage[target_index]; // shortens the name and serve as marker

  while (current != 0) { // LOOP while there's a current node
    if (strcmp(current->key, key) == 0) { // EXIT 1 if keys match, return the value
      return current->value;
    }
    else if (strcmp(current->key, key) != 0 && current->next == 0) { // EXIT 2 if no key matches and no linked node
      fprintf(stderr, "That key was not found.");
      return NULL;
    }
    current = current->next; // ITERATOR Move to the next linked node and continue with while loop to check
  }
  return 0;
}

void destroy_hash_table(HashTable *ht)
{

}

HashTable *hash_table_resize(HashTable *ht)
{
  HashTable *new_ht;

  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(2);

  hash_table_insert(ht, "tim", "texas\n"); // inserts index 1
  hash_table_insert(ht, "josh", "mexico\n"); // inserts index 1 -> INDEX COLLISION, sets on existing node's "next"
  hash_table_insert(ht, "tim", "california\n"); // inserts index 1 -> INDEX & KEY COLLISION, set on exisiting node's "value"

  // REMOVE (Choose only 1)
  // hash_table_remove(ht, "tim"); // removes tim from the 1st node and shift josh to 1st node
  // hash_table_remove(ht, "josh"); // removes josh from the 2nd node

  // RETRIEVE
  printf("%s\n", hash_table_retrieve(ht, "tim")); // returns california
  printf("%s\n", hash_table_retrieve(ht, "josh")); // returns josh

  return 0;
}
#endif
