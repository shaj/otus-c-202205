
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hashtable.h"
#include "powtable.h"

/// Размер хэш-таблицы
#define HASH_TABLE_SIZE 31

// static const unsigned long long p_pow[POW_SIZE];


/**
 * @brief      Вычисление хэша для строки
 *
 * @param[in]  buf   The buffer
 *
 * @return     The hash.
 */
int get_str_hash(const char *buf, int size)
{
    unsigned long long hash = 0;
    int j = 0;
    for (const char *pc = buf; *pc != '\0'; pc++)
    {
        hash += (*pc - 'a' + 1) * p_pow[j % POW_SIZE];
        j++;
    }
    return hash % size;
}

HashTable * hashtable_create(size_t table_size)
{
    HashTable *table = malloc(sizeof(struct HashTable));
    if(table == NULL)
        return NULL;
    table->size = table_size;
    table->taken = 0;
    table->table = malloc(sizeof(struct WordInfo *) * table_size);
    if(table->table == NULL)
    {
        free(table);
        return NULL;
    }
    memset(table->table, 0, sizeof(struct WordInfo *) * table_size);
    return table;
}


/**
 * Создание структуры для хранения слова
 * и добавление туда слова.
 *
 * @param[in]  buf   Null terminated string
 *
 * @return     true - структура создана и слово добавлено
 */
struct WordInfo *create_word_descr(const char *buf)
{
    struct WordInfo *wd = (struct WordInfo *)malloc(sizeof(struct WordInfo));
    if (wd == NULL)
    {
        fprintf(stderr, "ERROR. Out of memory\n");
        return NULL;
    }
    wd->word = (char *)malloc(strlen(buf) + 1);
    if (wd->word == NULL)
    {
        free(wd);
        fprintf(stderr, "ERROR. Out of memory\n");
        return NULL;
    }
    strcpy(wd->word, buf);
    wd->counter = 1;
    return wd;
}


/**
 * @brief      Инициализация таблицы
 *
 * @return     
 */
HashTable * hashtable_init()
{
    return hashtable_create(HASH_TABLE_SIZE);
}

int hashtable_probe(HashTable *table, const char *str, int hash)
{
    if(hash >= table->size)
        return -1;
    int idx = hash;
    while((table->table[idx] != NULL) 
        && (strcmp(table->table[idx]->word, str) != 0))
    {
        idx++;
        if(idx == table->size)
            idx = 0;
        if(idx == hash)
        {
            fprintf(stderr, "ERROR. Hash table is broken :(\n");
            return -1;
        }
    }
    return idx;
}

struct WordInfo * put_new_word(HashTable *table, const char *str, int hash)
{
    int idx = hashtable_probe(table, str, hash);
    if(idx == -1)
        return NULL;
    if(table->table[idx] == NULL)
    {
        table->table[idx] = create_word_descr(str);
        table->taken++;
    }
    return table->table[idx];
}


struct WordInfo * find_wi(HashTable *table, const char *str, int hash)
{
    int idx = hashtable_probe(table, str, hash);
    if(idx == -1)
        return NULL;
    return table->table[idx];
}


bool resize_table(HashTable *table)
{
    bool err = false;
    int new_table_size = table->size * 2;
    HashTable *new_table = hashtable_create(new_table_size);
    if(new_table == NULL)
    {
        fprintf(stderr, "Can not resize hash teble\n");
        return false;
    }
    for(int i=0; i<table->size; i++)
    {
        if(table->table[i] != NULL)
        {
            int hash = get_str_hash(table->table[i]->word, new_table_size);
            int idx = hashtable_probe(new_table, table->table[i]->word, hash);
            if(idx == -1)
            {
                err = true;
                break;
            }
            new_table->table[idx] = table->table[i];
            new_table->taken++;
        }
    }
    if(err)
    {
        free(new_table->table);
        free(new_table);
        return false;
    }
    else
    {
        free(table->table);
        table->table = new_table->table;
        table->size = new_table->size;
        free(new_table);
        return true;
    }
}

const struct WordInfo * hashtable_add(HashTable *table, const char *str)
{
    int hash = get_str_hash(str, table->size);
    struct WordInfo * wi = find_wi(table, str, hash);
    if(wi != NULL)
    {
        wi->counter++;
        return wi;
    }

    if((table->size - table->taken) < (table->size / 2))
    {
        if(!resize_table(table))
        {
            fprintf(stderr, "Can not create a data structure\n");
            return NULL;
        }
    }
    return put_new_word(table, str, hash);
}


/**
 * @brief      Получение значения из таблицы для строки.
 *
 * @param[in]  table  Таблица
 * @param[in]  hash   Строка
 *
 * @return     Указатель на данные
 */
const struct WordInfo * hashtable_get(HashTable *table, const char *str)
{
    int hash = get_str_hash(str, table->size);
    return find_wi(table, str, hash);
}

/**
 * @brief      Удаление таблицы из памати
 *
 * @param      table  The table
 */
void hashtable_free(HashTable *table)
{
    if(table != NULL)
    {
        for(int i=0; i<table->size; i++)
        {
            if(table->table[i] != NULL)
            {
                if(table->table[i]->word != NULL)
                {
                    free(table->table[i]->word);
                }
                free(table->table[i]);
            }
        }
        free(table);
    }
}
