

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "hashtable.h"
#include "word_counter.h"

static struct
{
    size_t words_readed;
} telemetr;

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

    telemetr.words_readed = 0;

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
    for (;;)
    {
        in_char = fgetc(hfile);
        if (feof(hfile))
        {
            *pc = '\0';
            if (buf[0] != '\0')
            {
                if (!hashtable_add(table, buf))
                {
                    retval = false;
                }
                else
                {
                    telemetr.words_readed++;
                }
            }
            break;
        }
        if (pc >= (buf + BUF_SIZE))
        {
            fprintf(stderr, "Too long word. More than %d bytes.", BUF_SIZE);
            *pc = '\0';
            if (buf[0] != '\0')
            {
                if (!hashtable_add(table, buf))
                {
                    retval = false;
                    break;
                }
                telemetr.words_readed++;
            }
        }
        if (in_char > 0x7f)
        {
            add_char_to_buf(in_char, &pc);
        }
        else if (isalnum(in_char))
        {
            add_char_to_buf(in_char, &pc);
        }
        else
        {
            *pc = '\0';
            if (buf[0] != '\0')
            {
                if (!hashtable_add(table, buf))
                {
                    retval = false;
                    break;
                }
                telemetr.words_readed++;
            }
            pc = buf;
        }
    }

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
    size_t words_stored = 0;

    if (table == NULL)
        return;
    if (hashtable_get_size(table) == 0)
        return;

    struct HashtableIterator iter;
    hashtable_iter_init(&iter, table);
    while (iter.wi != NULL)
    {
        printf("%6d  \"%s\" %d\n", iter.idx, iter.wi->word, iter.wi->counter);
        // printf("%s\n", iter.wi->word);
        words_stored += iter.wi->counter;
        hashtable_iter_next(&iter);
    }
    fprintf(stderr, "\n\nSize: %d\nTaken: %d\n", hashtable_get_size(table),
            hashtable_get_taken(table));
    fprintf(stderr, "\nПрочитано слов %ld\nСохранено слов %ld\n", telemetr.words_readed,
            words_stored);
}
