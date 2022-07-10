
#pragma once

#include <stdio.h>
#include <stdbool.h>

#include "hashtable.h"

#define BUF_SIZE 4096

struct WordDescr
{
    char *word;
    size_t counter;
    struct WordDescr *next;
};

bool read_words(FILE *hfile, HashTable *table);