#include <stdio.h>
#include <stdlib.h>

//Structure
typedef struct Element {
    int key;
    int value;
} Element; /* you had a syntax error here */

typedef struct HashTable {
    int size; /* we will need the size for the traversal */
    Element *table; /* leave it as a pointer */
} HashTable; /* a syntax error here too */

HashTable* createHashTable(int size) {
    HashTable *Ranking = malloc(sizeof(HashTable));

    /* set the pointer to point to a dynamic array of size 'size' */
    /* this way you don't have to hardcode the size */
    Ranking->table = malloc(sizeof(Element) * size); 
    Ranking->size = size;

    /* initialisation is a bit different because we don't have pointers here */
    /* only table is a pointer, not its elements */
    int i;
    for (i = 0; i < size; i++)
    {
      Ranking->table[i].key = 0;
      Ranking->table[i].value = 0;
    }

    return Ranking;
}

/* I implemented a fake hashFunction just to test the code */
/* all it does is make sure the key does not exceed the size of the table */
int hashFunction(int key, int size)
{
    return (key % size);
}

//Insert element
void insertElement(HashTable *Ranking, int key, int value) {

    int h = hashFunction(key, Ranking->size);
    int i = 0;

    /* if hash is full and key doesn't exist your previous loop would have gone on forever, I've added a check */
    /* also notice that I check if table[h] has empty key, not if it's null as this is not a pointer */
    while(Ranking->table[h].key != 0 && (i < Ranking->size)) {

        if(Ranking->table[h].key == key) {
            Ranking->table[h].value = value;
            return; /* break is intended to quit the loop, but actually we want to exit the function altogether */
        }

        h = (h + 1) % Ranking->size; /* changed 11 to the size specified */
        i++; /* advance the loop index */
        
    }

    /* okay found a free slot, store it there */
    if(Ranking->table[h].key == 0) {
        /* we now do direct assignment, no need for pointers */
        Ranking->table[h].key = key;
        Ranking->table[h].value = value;
    }
}

int main() {

    int size = 0;
    scanf(" %d", &size);

    HashTable *Ranking = createHashTable(size);

    insertElement(Ranking, 113, 10); /* this is just a test, 113 will be hashed to be less than size */

    /* we free everything we have malloc'ed */
    free(Ranking->table);
    free(Ranking);

    return 0;
}
