
#pragma once

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

struct WordInfo
{
    char *word;
    int counter;
};

/// Хэш-таблица
typedef struct HashTable HashTable;

HashTable * hashtable_init();
const struct WordInfo *  hashtable_add(HashTable *table, const char *str);
const struct WordInfo *  hashtable_get(HashTable *table, const char *str);
void hashtable_free(HashTable *table);

struct WordInfo * hashtable_iterate(HashTable *table, struct WordInfo *iter);


/**
 * @brief      { struct_description }
 */
struct HashTable
{
    struct WordInfo **table;
    int size;
    int taken;
};

#endif // _HASHTABLE_H_