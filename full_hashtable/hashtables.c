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
  if (pair != NULL) {
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
  ht->storage = (LinkedPair**)calloc(capacity, sizeof(LinkedPair*));

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
	int index = hash(key, ht->capacity);

	if(ht->storage[index]){
                LinkedPair *a = ht->storage[index];
		//printf("a value %p \n", a);
		//printf("ht-> value %p \n", ht->storage[index]);

		int flag =0;

		while(a!=NULL){
			if(strcmp(a->key, key)==0){
				printf("a->key: %s key: %s \n", a->key, key);

				printf("Key Value pair already exists, overwriting the value...\n");
                                //destroy_pair(a);
                		//a = create_pair(key, value);
				a->value = NULL;
				a->value = value;
				flag = 1;

			}
			//printf("a before %p \n", a);
			//printf("a->next before assignment  %p \n", a->next);
                        a = a->next;
			//printf("a after next assignment %p \n", a);
		}

		if(flag==0) {
			//printf("Creating new pair \n");
			//printf("a value %p \n", a);
			//printf("ht->storage[index]->next value %p \n", ht->storage[index]->next);
			ht->storage[index]->next = create_pair(key, value);
			printf("Inserted %s and %s \n", key, value);
		}

	}
	else{
		ht->storage[index] = create_pair(key, value);
		printf("Inserted %s and %s \n", key, value);
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
	int index = hash(key, ht->capacity);
	
	if(ht->storage[index]){
        	LinkedPair *a = ht->storage[index];
		LinkedPair *a_previous = a;

                while(a!=NULL){
                        if(strcmp(a->key, key)==0){
				if(a->next==NULL){
					printf("a->key is: %s and key is: %s\n", a->key, key);
					printf("a address %p \n", a);
                                	destroy_pair(a);
					a = NULL;
					printf("Destroyed %s \n", key);
					//printf("a->val should be NULL %s \n", a->value);
					break;
				}
				else{
					printf("a->key is: %s and key is: %s\n", a->key, key);
					a_previous->next = a->next;
					printf("a address where a_prev %p \n ", a);
					//printf("a->key is: %s and key is: %s\n", a->key, key);
					destroy_pair(a);
                                        a = NULL;
					printf("Destroyed %s \n", key);
					//printf("a->val should be NULL %s \n", a->value);
                                        break;
				}
                        }
			a_previous = a;
                        a = a->next;

                }
	}
	else{
		printf("The Key you are trying to remove could not be found");
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
  
	int index = hash(key, ht->capacity);

        if(ht->storage[index]){
                LinkedPair *a = ht->storage[index];

                while(a!=NULL){
                        if(a->key == key){
                                return a->value;
			}
                        a = a->next;

                }
		return NULL;
        }
        else{
                printf("The Key you are trying to retrieve could not be found");
		return NULL;
        }
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(HashTable *ht)
{
	for(int i=0; i < ht->capacity; i++){
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
  int new_cap = 2 * ht->capacity;	
  HashTable *new_ht = create_hash_table(new_cap);

  for(int i=0; i < ht->capacity; i++){
	
	LinkedPair *b = ht->storage[i]; 

	while(b != NULL){  
  		hash_table_insert(new_ht, ht->storage[i]->key, ht->storage[i]->value);
		b = b->next;
	}	
  }

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
