

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "hashtable.h"
#include "word_counter.h"


inline void add_char_to_buf(int in_char, unsigned char *pc)
{
    *pc++ = (unsigned char)in_char;
}

void add_word(HashTable *table, unsigned char *buf, size_t len)
{
    struct WordDescr *pwd;
    struct WordDescr **word_descr;

    if((buf[0] == '\0') || (len == 0))
    {
        return;
    }
    size_t hash = get_str_hash(buf);

    pwd = (struct WordDescr*) hashtable_get(table, hash);
    word_descr = &pwd;
    while(((*word_descr) == NULL) && (strcmp((*word_descr)->word, buf) != 0))
    {
        word_descr = &((*word_descr)->next);
    }

    if((*word_descr) == NULL)
    {
        *word_descr = (struct WordDescr*) malloc(sizeof(struct WordDescr));
        (*word_descr)->word = (char*) malloc(len);
        strcpy(buf, (*word_descr)->word);
        (*word_descr)->counter = 1;
        (*word_descr)->next = NULL;
    }
    else
    {
        if(strcmp((*word_descr)->counter, buf) == 0)
        {
            (*word_descr)->counter++;
        }
        else
        {
            *word_descr = (struct WordDescr*) malloc(sizeof(struct WordDescr));
            (*word_descr)->word = (char*) malloc(len);
            strcpy(buf, (*word_descr)->word);
            (*word_descr)->counter = 1;
            (*word_descr)->next = NULL;
        }
    }
}

bool read_words(FILE *hfile, HashTable *table)
{
    int cnt;
    unsigned char buf[BUF_SIZE];
    unsigned char *pc;
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
        if (in_char > 0xff)
        {
            add_char_to_buf(in_char, pc);
        }
        else if (((in_char | 0x20) >= 'A') && ((in_char | 0x20) <= 'Z'))
        {
            add_char_to_buf(in_char, pc);
        }
        else
        {
            *pc = '\0';
            add_word(table, buf, (pc - buf));
        }
    } while (1);
}