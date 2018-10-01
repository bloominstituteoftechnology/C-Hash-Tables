#include <b_hashtables.h>
#include "../utils/minunit.h"

char *basic_hash_table_test()
{
    BasicHashTable *ht = create_hash_table(8);

    char *return_value = hash_table_retrieve(ht, "key-0");
    mu_assert(return_value == NULL, "Initialized value is not NULL");

    hash_table_insert(ht, "key-0", "val-0");
    return_value = hash_table_retrieve(ht, "key-0");
    mu_assert(strcmp(return_value, "val-0") == 0, "Value is not stored correctly");

    hash_table_insert(ht, "key-0", "new-val-0");
    return_value = hash_table_retrieve(ht, "key-0");
    mu_assert(strcmp(return_value, "new-val-0") == 0, "Value is not overwritten correctly");

    hash_table_remove(ht, "key-0");
    return_value = hash_table_retrieve(ht, "key-0");
    mu_assert(return_value == NULL, "Deleted value is not NULL");

    return NULL;
}


char *all_tests()
{
    mu_suite_start();

    mu_run_test(basic_hash_table_test);

    return NULL;
}

RUN_TESTS(all_tests);