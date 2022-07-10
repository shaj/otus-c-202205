
#pragma once

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

#define HASH_TABLE_SIZE 256

struct HashRow
{
    unsigned long long hash;
    void *data
};

typedef struct HashRow HashTable[HASH_TABLE_SIZE];

size_t get_str_hash(const char *buf);

bool hashtable_init(HashTable table);
bool hashtable_delete(HashTable table);
bool hashtable_add(HashTable table, void *data);
bool hashtable_remove(HashTable table, void *data);
void * hashtable_get(HashTable table, size_t hash);

#endif // _HASHTABLE_H_