
#pragma once

#ifndef _HASHTABLE_H_
#define _HASHTABLE_H_

/**
 * @brief      Струткра для хранения данных (ключ / значение)
 */
struct WordInfo
{
    char *key;
    void *value;
    int counter;

};

/// Хэш-таблица
typedef struct HashTable HashTable;

/**
 * @brief      Создание новой пустой таблицы
 *
 * @return     Указатель на новую таблицу
 */
HashTable *hashtable_init();

/**
 * @brief      Добавление ключа в таблицу
 * 
 * Данные key/value не копируются.
 *
 * @param      table  Таблица
 * @param[in]  key    Ключ
 * @param[in]  value  Указатель на значение
 *
 * @return     Структура, соответвующая ключу
 */
const struct WordInfo *hashtable_add(HashTable *table, char *key, void *value);

/**
 * @brief      Поиск данных по ключу
 *
 * @param      table  Таблица
 * @param[in]  str    Ключ
 *
 * @return     NULL, если ключ не найден
 */
const struct WordInfo *hashtable_get(HashTable *table, const char *key);

/**
 * @brief      Удаление таблицы и освобождение памяти
 *
 * @param      table  The table
 */
void hashtable_free(HashTable *table);

/**
 * @brief      Получение размера таблицы
 *
 * @param      table  The table
 *
 * @return     Размер таблицы
 */
int hashtable_get_size(HashTable *table);

/**
 * @brief      Получение количества элементов в таблице
 *
 * @param      table  The table
 *
 * @return     Количество элементов в таблице
 */
int hashtable_get_taken(HashTable *table);

struct HashtableIterator
{
    int idx;
    struct WordInfo *wi;
    HashTable *table;
};

/**
 * @brief      Инициализация итератора
 *
 * @param      iter   The iterator
 * @param      table  The table
 */
void hashtable_iter_init(struct HashtableIterator *iter, HashTable *table);

/**
 * @brief      Получение следующего элемента
 *
 * @param      iter  The iterator
 */
void hashtable_iter_next(struct HashtableIterator *iter);

#endif // _HASHTABLE_H_