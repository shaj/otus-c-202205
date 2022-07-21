

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "hashtable.h"
#include "word_counter.h"

/**
 * @brief      Adds a character to buffer.
 *
 * @param[in]  in_char  In character
 * @param      pc       Двойной указательна строку, куда добавлять букву.
 */
static inline void add_char_to_buf(int in_char, char **pc)
{
    **pc = (char)in_char;
    (*pc)++;
}

/**
 * @brief      Добавление с лова в таблицу
 *
 * @param[in]  table  Таблица, куда добавлять
 * @param      buf    Указатель на null-terminated слово
 *
 * @return     true - слово успешно добавлено
 */
// bool add_word(HashTable table, const char *buf)
// {
//     struct WordDescr **word_descr;

//     if (buf[0] == '\0')
//     {
//         return true; // Ошибки нет. Просто пустую строку не добавляем.
//     }

//     unsigned long long hash = get_str_hash(buf);

// #ifdef DEBUG
//     printf("Added word \"%s\" with hash 0x%08llx (0x%02llx)\n", buf, hash,
//            hash % HASH_TABLE_SIZE);
// #endif // DEBUG

//     word_descr = (struct WordDescr **)hashtable_get(table, hash);
//     while (((*word_descr) != NULL) && (strcmp((*word_descr)->word, buf) != 0))
//     {
//         word_descr = &((*word_descr)->next);
//     }

//     if ((*word_descr) == NULL)
//     {
//         *word_descr = create_word_descr(buf);
//         if (*word_descr == NULL)
//         {
//             fprintf(stderr, "Can not add word to list.\n");
//             return false;
//         }
//     }
//     else
//     {
//         if (strcmp((*word_descr)->word, buf) == 0)
//         {
//             (*word_descr)->counter++;
//         }
//         else
//         {
//             *word_descr = create_word_descr(buf);
//             if (*word_descr == NULL)
//             {
//                 fprintf(stderr, "Can not add word to list.\n");
//                 return false;
//             }
//         }
//     }
//     return true;
// }

/**
 * @brief      Проверка типа входного файла
 *
 * @param[in]  fname - имя файла
 *
 * @return true - путь указывает на обычный файл
 */
bool regular_file_check(const char *fname)
{
    struct stat sb;
    if (stat(fname, &sb) == -1)
    {
        return false;
    }
    if ((sb.st_mode & S_IFMT) != S_IFREG)
    {
        return false;
    }
    return true;
}

/**
 * @brief      Чтение слов из файла
 *
 * @param[in]  fname  Имя файла
 * @param[in]  table  Хэш-таблица, куда читать
 *
 * @return     true  - файл успешно прочитан
 *             false - при чтении файла были ошибки
 */
bool read_words(const char *fname, HashTable *table)
{
    char buf[BUF_SIZE];
    char *pc;
    int in_char;
    FILE *hfile;

    // Открытие входного файлов
    if (!regular_file_check(fname))
    {
        fprintf(stderr, "The \"%s\" is not a regular file\n", fname);
        return false;
    }
    hfile = fopen(fname, "r");
    if (hfile == NULL)
    {
        fprintf(stderr, "Can not open file \"%s\"\n", fname);
        return false;
    }

    pc = buf;
    bool retval = true;
    do
    {
        in_char = fgetc(hfile);
        if (feof(hfile))
        {
            *pc = '\0';
            if(buf[0] != '\0')
            {
                if (!hashtable_add(table, buf))
                {
                    retval = false;
                }
            }
            break;
        }
        if (pc >= (buf + BUF_SIZE))
        {
            fprintf(stderr, "Too long word. More than %d bytes.", BUF_SIZE);
            *pc = '\0';
            if(buf[0] != '\0')
            {
                if (!hashtable_add(table, buf))
                {
                    retval = false;
                    break;
                }
            }
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
            if(buf[0] != '\0')
            {
                if (!hashtable_add(table, buf))
                {
                    retval = false;
                    break;
                }
            }
            pc = buf;
        }
    } while (1);

    fclose(hfile);
    return retval;
}


/**
 * @brief      Вывод в консоль результата
 *
 * @param[in]  table  The table
 */
void print_words_array(HashTable *table)
{
//     struct WordDescr *word_descr;

// #ifdef DEBUG
//     printf("\n\nReport:\n");
//     for (int i = 0; i < HASH_TABLE_SIZE; i++)
//     {
//         if (table[i].data != NULL)
//         {
//             printf("Hash %4d | ", i);
//             word_descr = (struct WordDescr *)table[i].data;
//             while (word_descr != NULL)
//             {
//                 printf(" \"%s\" %ld", word_descr->word, word_descr->counter);
//                 word_descr = word_descr->next;
//             }
//             printf("\n");
//         }
//     }
//     printf("\n\n");
// #endif // DEBUG

//     int wc = 0;
//     int chain = 0;
//     int max_chain = 0;
//     int empty_cells = 0;

//     for (int i = 0; i < HASH_TABLE_SIZE; i++)
//     {
//         if (table[i].data != NULL)
//         {
//             chain = 0;
//             word_descr = (struct WordDescr *)table[i].data;
//             while (word_descr != NULL)
//             {
//                 printf("\"%s\" %ld\n", word_descr->word, word_descr->counter);
//                 word_descr = word_descr->next;
//                 chain++;
//                 wc++;
//             }
//             if (max_chain < chain)
//             {
//                 max_chain = chain;
//             }
//         }
//         else
//         {
//             empty_cells++;
//         }
//     }
//     printf("\nВсего слов: %d\n", wc);
//     printf("Пустых ячеек: %d из %d\n", empty_cells, HASH_TABLE_SIZE);
//     printf("Максимальная длина цепочки: %d\n", max_chain);

    if(table == NULL)
        return;
    if(table->table == NULL)
        return;
    for(int i=0; i<table->size; i++)
    {
        if(table->table[i] != NULL)
        {
            printf("%5d  \"%s\" %d\n", i, table->table[i]->word, table->table[i]->counter);
        }
    }
    printf("\n\nSize: %d\nTaken: %d\n", table->size, table->taken);

    return;
}

