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
  ht->storage = calloc(capacity, sizeof(LinkedPair*));

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
  unsigned int index = hash(key, ht->capacity);

  if(ht->storage[index] == NULL){
    ht->storage[index] = create_pair(key, value);
    printf("Inserted %s and %s \n", key, value);
  }
  
  else{
      LinkedPair *current_pair = ht->storage[index];
      
      while(current_pair){
         
         if(strcmp(current_pair->key, key)==0){
            printf("comparing keys %s and %s\n", current_pair->key, key);		 
            printf("Overwriting key %s \n", key);
            current_pair->value = value;
	    //n = NULL;
            return;
         }
	 else if(current_pair->next){
		 current_pair = current_pair->next;	
	 }
	 else{      
      		 current_pair->next = create_pair(key, value);   
     		 printf("Inserted %s and %s \n", key, value);	 
      	 }
      }
   }  
} 



/* Alternative solution to insert
	
void hash_table_insert(HashTable *ht, char *key, char *value)
{
        unsigned int index = hash(key, ht->capacity);

        if(ht->storage[index]){
                LinkedPair *currentPair = ht->storage[index];

                while(currentPair){                                      //if the index (exits) for the key is not empty, enter the loop
                        if(strcmp(currentPair->key, key)==0){           //check to see if there is a key match, overwrite the value     

                                printf("currentPair->key: %s key: %s \n", currentPair->key, key);
                                printf("Key Value pair already exists, overwriting the value...\n");
                                currentPair->value = value;
                                currentPair = NULL;
                        }
                        else if(currentPair->next){   //keep movining across the linked list till you find the key and overwrite the value or reach NULL
                                currentPair = currentPair->next;
                        }
                        else{                            //the given key is not yet created at the index, and reached NULL, create a new pair.  
                                currentPair->next = create_pair(key, value);
                                printf("Inserted %s and %s \n", key, value);
                        }
                }
        }
        else{                             //if the index is empty and does not have any key/value pairs, create a new pair
                ht->storage[index] = create_pair(key, value);
                printf("Inserted %s and %s \n", key, value);
            }
}   

*/



/****
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(HashTable *ht, char *key)
{
	unsigned int index = hash(key, ht->capacity);
  
  	LinkedPair *current_pair = ht->storage[index];
  	LinkedPair *last_pair;

  	for(int i=0; i<ht->capacity; i++){
    	
		while (current_pair != NULL && strcmp(current_pair->key, key) != 0) {
    			last_pair = current_pair;
    			current_pair = last_pair->next;
    		}

    		if (current_pair == NULL) {
    		printf("The key could not be removed: %s\n", key);
  		}

		else {
    			destroy_pair(ht->storage[index]);
    			ht->storage[index] = NULL;
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
  
	int index = hash(key, ht->capacity);

        if(ht->storage[index]){
                LinkedPair *currentPair = ht->storage[index];

                while(currentPair!=NULL){
                        if(currentPair->key == key){
                                return currentPair->value;
			}
                        currentPair = currentPair->next;

                }
		return NULL;
        }
        else{
                printf("The Key you are trying to retrieve could not be found \n");
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
