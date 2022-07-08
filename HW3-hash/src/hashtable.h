
#pragma once

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

typedef struct
{
    unsigned long long table[256];
} HashTable;

bool hashtable_init(HashTable *table);
void hashtable_delete(HashTable *table);
bool hashtable_add(HashTable *table, void *data);
bool hashtable_remove(HashTable *table, void *data);
const void * hashtable_get(HashTable *table) const;

#endif // _HASHTABLE_H_