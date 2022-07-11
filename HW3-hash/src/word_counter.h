
#pragma once

#include <stdbool.h>
#include <stdio.h>

#include "hashtable.h"

#define BUF_SIZE 4096

struct WordDescr
{
    char *word;
    size_t counter;
    struct WordDescr *next;
};

bool read_words(FILE *hfile, HashTable table);

void print_words_array(HashTable table);
void hashtable_delete(HashTable table);
