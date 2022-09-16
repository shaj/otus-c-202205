
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "powtable.h"

/// Начальный размер хэш-таблицы
#define HASH_TABLE_SIZE 31

/**
 * @brief      Структура для хранения хэш-таблицы
 */
struct HashTable
{
    struct WordInfo **table;
    int size;
    int taken;
};

/**
 * @brief      Получение размера таблицы
 *
 * @param      table  The table
 *
 * @return     Размер таблицы
 */
int hashtable_get_size(HashTable *table)
{
    if (table != NULL)
    {
        return table->size;
    }
    return 0;
}

/**
 * @brief      Получение количества элементов в таблице
 *
 * @param      table  The table
 *
 * @return     Количество элементов в таблице
 */
int hashtable_get_taken(HashTable *table)
{
    if (table != NULL)
    {
        return table->taken;
    }
    return 0;
}

/**
 * @brief      Инициализация итератора
 *
 * @param      iter   The iterator
 * @param      table  The table
 */
void hashtable_iter_init(struct HashtableIterator *iter, HashTable *table)
{
    iter->table = table;
    iter->idx = 0;
    if ((table == NULL) || (table->size == 0) || (table->table == NULL))
    {
        iter->wi = NULL;
        return;
    }
    while (iter->table->table[iter->idx] == NULL)
    {
        iter->idx++;
        if (iter->idx >= iter->table->size)
        {
            iter->wi = NULL;
            return;
        }
    }
    iter->wi = iter->table->table[iter->idx];
}

/**
 * @brief      Получение следующего элемента
 *
 * @param      iter  The iterator
 */
void hashtable_iter_next(struct HashtableIterator *iter)
{
    iter->idx++;
    while ((iter->idx < iter->table->size) && (iter->table->table[iter->idx] == NULL))
    {
        iter->idx++;
    }
    if (iter->idx >= iter->table->size)
    {
        iter->wi = NULL;
    }
    else
    {
        iter->wi = iter->table->table[iter->idx];
    }
}

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
        hash += (unsigned long long)(*pc) * p_pow[j % POW_SIZE];
        j++;
    }
    return hash % size;
}

/**
 * @brief      Создание таблицы заданного размера
 *
 * @param[in]  table_size  Размер новой таблицы
 *
 * @return     Указатель на новую таблицу
 */
HashTable *hashtable_create(size_t table_size)
{
    HashTable *table = malloc(sizeof(struct HashTable));
    if (table == NULL)
        return NULL;
    table->size = table_size;
    table->taken = 0;
    table->table = malloc(sizeof(struct WordInfo *) * table_size);
    if (table->table == NULL)
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
 * @brief      Создание новой пустой таблицы
 *
 * @return     Указатель на новую таблицу
 */
HashTable *hashtable_init() { return hashtable_create(HASH_TABLE_SIZE); }

/**
 * @brief      Реализация поиска элемента с открытой адресацией
 *
 * @param      table  The table
 * @param[in]  str    Строка, которую ищем
 * @param[in]  hash   Хэш строки
 *
 * @return     Индекс элемента в таблице
 */
int hashtable_probe(HashTable *table, const char *str, int hash)
{
    if (hash >= table->size)
        return -1;
    int idx = hash;
    while ((table->table[idx] != NULL) && (strcmp(table->table[idx]->word, str) != 0))
    {
        idx++;
        if (idx == table->size)
            idx = 0;
        if (idx == hash)
        {
            fprintf(stderr, "ERROR. Hash table is broken :(\n");
            return -1;
        }
    }
    return idx;
}

/**
 * @brief      Добавление ключа в таблицу
 *
 * @param      table  The table
 * @param[in]  str    Ключ
 * @param[in]  hash   Заранее вычисленный хэш
 *
 * @return     Структура данных по ключу
 */
struct WordInfo *put_new_word(HashTable *table, const char *str, int hash)
{
    int idx = hashtable_probe(table, str, hash);
    if (idx == -1)
        return NULL;
    if (table->table[idx] == NULL)
    {
        table->table[idx] = create_word_descr(str);
        table->taken++;
    }
    return table->table[idx];
}

/**
 * @brief      Поиск в таблице по ключу
 *
 * @param      table  The table
 * @param[in]  str    Ключ
 * @param[in]  hash   Заранее вычисленный хэш
 *
 * @return     Структура данных по ключу
 */
struct WordInfo *find_wi(HashTable *table, const char *str, int hash)
{
    int idx = hashtable_probe(table, str, hash);
    if (idx == -1)
        return NULL;
    return table->table[idx];
}

/**
 * @brief      Изменение размера таблицы
 *
 * @param      table  The table
 *
 * @return     true - изменение размера прошло успешно
 *             false - при изменении размера произошли ошибки.
 *                     Осталась старая таблица.
 */
bool resize_table(HashTable *table)
{
    bool err = false;
    int new_table_size = table->size * 2;
    HashTable *new_table = hashtable_create(new_table_size);
    if (new_table == NULL)
    {
        fprintf(stderr, "Can not resize hash teble\n");
        return false;
    }
    for (int i = 0; i < table->size; i++)
    {
        if (table->table[i] != NULL)
        {
            int hash = get_str_hash(table->table[i]->word, new_table_size);
            int idx = hashtable_probe(new_table, table->table[i]->word, hash);
            if (idx == -1)
            {
                err = true;
                break;
            }
            if (new_table->table[idx] != NULL)
            {
                fprintf(stderr,
                        "ERROR. Cell is busy. Word \"%s\" %d (%d). Replacing with word \"%s\" %d "
                        "(%d)\n",
                        new_table->table[idx]->word, new_table->table[idx]->counter, idx,
                        table->table[i]->word, table->table[i]->counter, i);
            }
            new_table->table[idx] = table->table[i];
            new_table->taken++;
        }
    }
    if (err)
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

/**
 * @brief      Добавление ключа в таблицу
 *
 * @param      table  Таблица
 * @param[in]  str    Ключ
 *
 * @return     Структура, соответвующая ключу
 */
const struct WordInfo *hashtable_add(HashTable *table, const char *str)
{
    int hash = get_str_hash(str, table->size);
    struct WordInfo *wi = find_wi(table, str, hash);
    if (wi != NULL)
    {
        wi->counter++;
        return wi;
    }

    if ((table->size - table->taken) < (table->size / 4))
    {
        if (!resize_table(table))
        {
            fprintf(stderr, "Can not create a data structure\n");
            return NULL;
        }
        hash = get_str_hash(str, table->size);
    }
    return put_new_word(table, str, hash);
}

/**
 * @brief      Поиск данных по ключу
 *
 * @param      table  Таблица
 * @param[in]  str    Ключ
 *
 * @return     { description_of_the_return_value }
 */
const struct WordInfo *hashtable_get(HashTable *table, const char *str)
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
    if (table != NULL)
    {
        for (int i = 0; i < table->size; i++)
        {
            if (table->table[i] != NULL)
            {
                if (table->table[i]->word != NULL)
                {
                    free(table->table[i]->word);
                }
                free(table->table[i]);
            }
        }
        free(table->table);
        free(table);
    }
}
