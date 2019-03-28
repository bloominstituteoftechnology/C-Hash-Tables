#include <hashtables.h>
#include "../utils/minunit.h"

char *test_hash_table_insertion_and_retrieval()
{
    struct HashTable *ht = create_hash_table(8);

    char *return_value = hash_table_retrieve(ht, "key-0");
    mu_assert(return_value == NULL, "Initialized value is not NULL");

    hash_table_insert(ht, "key-0", "val-0");
    hash_table_insert(ht, "key-1", "val-1");
    hash_table_insert(ht, "key-2", "val-2");
    hash_table_insert(ht, "key-3", "val-3");
    hash_table_insert(ht, "key-4", "val-4");
    hash_table_insert(ht, "key-5", "val-5");
    hash_table_insert(ht, "key-6", "val-6");
    hash_table_insert(ht, "key-7", "val-7");
    hash_table_insert(ht, "key-8", "val-8");
    hash_table_insert(ht, "key-9", "val-9");

    return_value = hash_table_retrieve(ht, "key-0");
    mu_assert(strcmp(return_value, "val-0") == 0, "Value is not stored correctly");
    return_value = hash_table_retrieve(ht, "key-1");
    mu_assert(strcmp(return_value, "val-1") == 0, "Value is not stored correctly");
    return_value = hash_table_retrieve(ht, "key-2");
    mu_assert(strcmp(return_value, "val-2") == 0, "Value is not stored correctly");
    return_value = hash_table_retrieve(ht, "key-3");
    mu_assert(strcmp(return_value, "val-3") == 0, "Value is not stored correctly");
    return_value = hash_table_retrieve(ht, "key-4");
    mu_assert(strcmp(return_value, "val-4") == 0, "Value is not stored correctly");
    return_value = hash_table_retrieve(ht, "key-5");
    mu_assert(strcmp(return_value, "val-5") == 0, "Value is not stored correctly");
    return_value = hash_table_retrieve(ht, "key-6");
    mu_assert(strcmp(return_value, "val-6") == 0, "Value is not stored correctly");
    return_value = hash_table_retrieve(ht, "key-7");
    mu_assert(strcmp(return_value, "val-7") == 0, "Value is not stored correctly");
    return_value = hash_table_retrieve(ht, "key-8");
    mu_assert(strcmp(return_value, "val-8") == 0, "Value is not stored correctly");
    return_value = hash_table_retrieve(ht, "key-9");
    mu_assert(strcmp(return_value, "val-9") == 0, "Value is not stored correctly");

    return NULL; 
}

char *test_hash_table_insertion_overwrites_correctly()
{
    struct HashTable *ht = create_hash_table(8);

    hash_table_insert(ht, "key-0", "val-0");
    hash_table_insert(ht, "key-1", "val-1");
    hash_table_insert(ht, "key-2", "val-2");
    hash_table_insert(ht, "key-3", "val-3");
    hash_table_insert(ht, "key-4", "val-4");
    hash_table_insert(ht, "key-5", "val-5");
    hash_table_insert(ht, "key-6", "val-6");
    hash_table_insert(ht, "key-7", "val-7");
    hash_table_insert(ht, "key-8", "val-8");
    hash_table_insert(ht, "key-9", "val-9");

    hash_table_insert(ht, "key-0", "new-val-0");
    hash_table_insert(ht, "key-1", "new-val-1");
    hash_table_insert(ht, "key-2", "new-val-2");
    hash_table_insert(ht, "key-3", "new-val-3");
    hash_table_insert(ht, "key-4", "new-val-4");
    hash_table_insert(ht, "key-5", "new-val-5");
    hash_table_insert(ht, "key-6", "new-val-6");
    hash_table_insert(ht, "key-7", "new-val-7");
    hash_table_insert(ht, "key-8", "new-val-8");
    hash_table_insert(ht, "key-9", "new-val-9");

    mu_assert(strcmp(hash_table_retrieve(ht, "key-0"), "new-val-0") == 0, "Value is not overwritten correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "key-1"), "new-val-1") == 0, "Value is not overwritten correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "key-2"), "new-val-2") == 0, "Value is not overwritten correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "key-3"), "new-val-3") == 0, "Value is not overwritten correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "key-4"), "new-val-4") == 0, "Value is not overwritten correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "key-5"), "new-val-5") == 0, "Value is not overwritten correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "key-6"), "new-val-6") == 0, "Value is not overwritten correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "key-7"), "new-val-7") == 0, "Value is not overwritten correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "key-8"), "new-val-8") == 0, "Value is not overwritten correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "key-9"), "new-val-9") == 0, "Value is not overwritten correctly");

    return NULL; 
}

char *test_hash_table_removes_correctly()
{
    struct HashTable *ht = create_hash_table(8);

    hash_table_insert(ht, "key-0", "val-0");
    hash_table_insert(ht, "key-1", "val-1");
    hash_table_insert(ht, "key-2", "val-2");
    hash_table_insert(ht, "key-3", "val-3");
    hash_table_insert(ht, "key-4", "val-4");
    hash_table_insert(ht, "key-5", "val-5");
    hash_table_insert(ht, "key-6", "val-6");
    hash_table_insert(ht, "key-7", "val-7");
    hash_table_insert(ht, "key-8", "val-8");
    hash_table_insert(ht, "key-9", "val-9");

    hash_table_remove(ht, "key-9");
    hash_table_remove(ht, "key-8");
    hash_table_remove(ht, "key-7");
    hash_table_remove(ht, "key-6");
    hash_table_remove(ht, "key-5");
    hash_table_remove(ht, "key-4");
    hash_table_remove(ht, "key-3");
    hash_table_remove(ht, "key-2");
    hash_table_remove(ht, "key-1");
    hash_table_remove(ht, "key-0");

    mu_assert(hash_table_retrieve(ht, "key-0") == NULL, "Deleted value is not NULL");
    mu_assert(hash_table_retrieve(ht, "key-1") == NULL, "Deleted value is not NULL");
    mu_assert(hash_table_retrieve(ht, "key-2") == NULL, "Deleted value is not NULL");
    mu_assert(hash_table_retrieve(ht, "key-3") == NULL, "Deleted value is not NULL");
    mu_assert(hash_table_retrieve(ht, "key-4") == NULL, "Deleted value is not NULL");
    mu_assert(hash_table_retrieve(ht, "key-5") == NULL, "Deleted value is not NULL");
    mu_assert(hash_table_retrieve(ht, "key-6") == NULL, "Deleted value is not NULL");
    mu_assert(hash_table_retrieve(ht, "key-7") == NULL, "Deleted value is not NULL");
    mu_assert(hash_table_retrieve(ht, "key-8") == NULL, "Deleted value is not NULL");
    mu_assert(hash_table_retrieve(ht, "key-9") == NULL, "Deleted value is not NULL");

    return NULL;
}

char *hash_table_resizing_test() {
    struct HashTable *ht = create_hash_table(8);

    hash_table_insert(ht, "resize-key-0", "resize-val-0");
    hash_table_insert(ht, "resize-key-1", "resize-val-1");
    hash_table_insert(ht, "resize-key-2", "resize-val-2");
    hash_table_insert(ht, "resize-key-3", "resize-val-3");
    hash_table_insert(ht, "resize-key-4", "resize-val-4");
    hash_table_insert(ht, "resize-key-5", "resize-val-5");
    hash_table_insert(ht, "resize-key-6", "resize-val-6");
    hash_table_insert(ht, "resize-key-7", "resize-val-7");
    hash_table_insert(ht, "resize-key-8", "resize-val-8");
    hash_table_insert(ht, "resize-key-9", "resize-val-9");

    ht = hash_table_resize(ht);

    mu_assert(ht->capacity == 16, "Resized hash table did not double capacity");

    mu_assert(strcmp(hash_table_retrieve(ht, "resize-key-0"), "resize-val-0") == 0, "Resized hash table did not copy values correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "resize-key-1"), "resize-val-1") == 0, "Resized hash table did not copy values correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "resize-key-2"), "resize-val-2") == 0, "Resized hash table did not copy values correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "resize-key-3"), "resize-val-3") == 0, "Resized hash table did not copy values correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "resize-key-4"), "resize-val-4") == 0, "Resized hash table did not copy values correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "resize-key-5"), "resize-val-5") == 0, "Resized hash table did not copy values correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "resize-key-6"), "resize-val-6") == 0, "Resized hash table did not copy values correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "resize-key-7"), "resize-val-7") == 0, "Resized hash table did not copy values correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "resize-key-8"), "resize-val-8") == 0, "Resized hash table did not copy values correctly");
    mu_assert(strcmp(hash_table_retrieve(ht, "resize-key-9"), "resize-val-9") == 0, "Resized hash table did not copy values correctly");

    return NULL;
}

char *all_tests()
{
    mu_suite_start();

    mu_run_test(test_hash_table_insertion_and_retrieval);
    mu_run_test(test_hash_table_insertion_overwrites_correctly);
    mu_run_test(test_hash_table_removes_correctly);
    mu_run_test(hash_table_resizing_test);

    return NULL;
}

RUN_TESTS(all_tests);
