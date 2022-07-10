
#include <stdbool.h>
#include <stdlib.h>

#include "hashtable.h"

#define POW_SIZE 256

static unsigned long long p_pow[POW_SIZE];

size_t get_str_hash(const char *buf)
{
    for (size_t i = 1; i < POW_SIZE; ++i)
    {
        p_pow[i] = p_pow[i - 1] * p;
    }

    for (int i = 0; i < n; ++i)
    {
        unsigned long long hash = 0;
        for (size_t j = 0; j < s[i].length(); ++j)
            hash += (s[i][j] - 'a' + 1) * p_pow[j];
        hashes[i] = make_pair(hash, i);
    }

    return 0;
}

bool hashtable_init(HashTable table)
{

    memset(table, 0, sizeof(table));
    return false;
}

bool hashtable_delete(HashTable table)
{
    // Not implemented yet
    return false;
}

bool hashtable_add(HashTable table, void *data)
{
    // Not implemented yet
    return false;
}

bool hashtable_remove(HashTable table, void *data)
{
    // Not implemented yet
    return false;
}

void *hashtable_get(HashTable table, size_t hash)
{
    // Not implemented yet
    return NULL;
}
