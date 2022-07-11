

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "word_counter.h"

static inline void add_char_to_buf(int in_char, char **pc)
{
    **pc = (char)in_char;
    (*pc)++;
}

void add_word(HashTable table, char *buf, size_t len)
{
    struct WordDescr **word_descr;

    if ((buf[0] == '\0') || (len == 0))
    {
        return;
    }

    unsigned long long hash = get_str_hash(buf);

#ifdef DEBUG
    printf("Added word \"%s\" with hash 0x%08llx (0x%02llx)\n", buf, hash,
           hash % HASH_TABLE_SIZE);
#endif // DEBUG

    word_descr = (struct WordDescr **)hashtable_get(table, hash);
    while (((*word_descr) != NULL) && (strcmp((*word_descr)->word, buf) != 0))
    {
        word_descr = &((*word_descr)->next);
    }

    if ((*word_descr) == NULL)
    {
        *word_descr = (struct WordDescr *)malloc(sizeof(struct WordDescr));
        (*word_descr)->word = (char *)malloc(len + 1);
        strcpy((*word_descr)->word, buf);
        (*word_descr)->counter = 1;
        (*word_descr)->next = NULL;
    }
    else
    {
        if (strcmp((*word_descr)->word, buf) == 0)
        {
            (*word_descr)->counter++;
        }
        else
        {
            *word_descr = (struct WordDescr *)malloc(sizeof(struct WordDescr));
            (*word_descr)->word = (char *)malloc(len + 1);
            strcpy((*word_descr)->word, buf);
            (*word_descr)->counter = 1;
            (*word_descr)->next = NULL;
        }
    }
}

bool read_words(FILE *hfile, HashTable table)
{
    int cnt;
    char buf[BUF_SIZE];
    char *pc;
    int in_char;

    cnt = fseek(hfile, 0, SEEK_SET);
    if (cnt != 0)
    {
        perror("Error while read data");
        return false;
    }

    pc = buf;
    do
    {
        in_char = fgetc(hfile);
        if (feof(hfile))
        {
            *pc = '\0';
            add_word(table, buf, (pc - buf));
            break;
        }
        if (pc >= (buf + BUF_SIZE))
        {
            *pc = '\0';
            add_word(table, buf, (pc - buf));
            fprintf(stderr, "Too long word. More than %d bytes.", BUF_SIZE);
        }
        if (in_char > 0x7f)
        {
            add_char_to_buf(in_char, &pc);
        }
        else if (((in_char | 0x20) >= 'a') && ((in_char | 0x20) <= 'z'))
        {
            add_char_to_buf(in_char, &pc);
        }
        else
        {
            *pc = '\0';
            add_word(table, buf, (pc - buf));
            pc = buf;
        }
    } while (1);
    return true;
}

void print_words_array(HashTable table)
{
    struct WordDescr *word_descr;

#ifdef DEBUG
    printf("\n\nReport:\n");
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        if (table[i].data != NULL)
        {
            printf("Hash %4d | ", i);
            word_descr = (struct WordDescr *)table[i].data;
            while (word_descr != NULL)
            {
                printf(" \"%s\" %ld", word_descr->word, word_descr->counter);
                word_descr = word_descr->next;
            }
            printf("\n");
        }
    }
    printf("\n\n");
#endif // DEBUG
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        if (table[i].data != NULL)
        {
            word_descr = (struct WordDescr *)table[i].data;
            while (word_descr != NULL)
            {
                printf("\"%s\" %ld\n", word_descr->word, word_descr->counter);
                word_descr = word_descr->next;
            }
        }
    }
    return;
}

void free_word_descr(struct WordDescr *word_descr)
{
    if (word_descr->next != NULL)
        free_word_descr(word_descr->next);
    if (word_descr->word != NULL)
        free(word_descr->word);
    free(word_descr);
}

void hashtable_delete(HashTable table)
{
    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        if (table[i].data != NULL)
        {
            free_word_descr(table[i].data);
        }
    }
    return;
}
