#ifndef hashtables_h
#define hashtables_h

typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;


HashTable *create_hash_table(int capacity);

void hash_table_insert(HashTable *ht, char *key, char *value);

void hash_table_remove(HashTable *ht, char *key);

char *hash_table_retrieve(HashTable *ht, char *key);

void destroy_hash_table(HashTable *ht);

HashTable *hash_table_resize(HashTable *ht);


#endif