
#pragma once

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

/// Размер хэш-таблицы
#define HASH_TABLE_SIZE 512

struct HashRow
{
    // unsigned long long hash;
    void *data;
};

/// Хэш-таблица
typedef struct HashRow HashTable[HASH_TABLE_SIZE];

unsigned long long get_str_hash(const char *buf);
bool hashtable_init(HashTable table);
void *hashtable_get(HashTable table, unsigned long long hash);

#endif // _HASHTABLE_H_