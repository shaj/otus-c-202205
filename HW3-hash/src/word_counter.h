
#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "hashtable.h"

/// Размер буфера для одного слова
#define BUF_SIZE 4096

/**
 * @brief      Структура для описания одного слова
 *
 * Односвязный список для разрешения коллизий
 */
struct WordDescr
{
    char *word;
    size_t counter;
    struct WordDescr *next;
};

bool read_words(const char *fname, HashTable table);
void print_words_array(HashTable table);
void hashtable_delete(HashTable table);
