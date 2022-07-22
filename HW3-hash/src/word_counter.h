
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

/**
 * @brief      Чтение слов из файла
 *
 * @param[in]  fname  Имя файла
 * @param[in]  table  Хэш-таблица, куда читать
 *
 * @return     true  - файл успешно прочитан
 *             false - при чтении файла были ошибки
 */
bool read_words(const char *fname, HashTable *table);

/**
 * @brief      Вывод в консоль результата
 *
 * @param[in]  table  The table
 */
void print_words_array(HashTable *table);
