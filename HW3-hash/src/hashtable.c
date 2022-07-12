
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "powtable.h"

static const unsigned long long p_pow[POW_SIZE];


/**
 * @brief      Вычисление хэша для строки
 *
 * @param[in]  buf   The buffer
 *
 * @return     The hash.
 */
unsigned long long get_str_hash(const char *buf)
{
    unsigned long long hash = 0;
    int j = 0;
    for (const char *pc = buf; *pc != '\0'; pc++)
    {
        hash += (*pc - 'a' + 1) * p_pow[j % POW_SIZE];
        j++;
    }
    return hash;
}

/**
 * @brief      Инициализация таблицы
 *
 * @param[in]  table  The table
 *
 * @return     Always true
 */
bool hashtable_init(HashTable table)
{
    memset(table, 0, sizeof(struct HashRow) * HASH_TABLE_SIZE);
    return true;
}


/**
 * @brief      Получение значения из таблицы дл яопределенного хэша
 * 
 * (Поиск в таблице по хэшу)
 *
 * @param[in]  table  The table
 * @param[in]  hash   The hash
 *
 * @return     Указатель на данные
 */
void *hashtable_get(HashTable table, unsigned long long hash)
{
    return &(table[hash % HASH_TABLE_SIZE].data);
}
