

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
 * @param      pc       { parameter_description }
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
 * @param[in]  len    Длина слова. (Для скорости. Чтобы лишний раз не вычислять)
 */
void add_word(HashTable table, const char *buf, size_t len)
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
 * @return     true - файл успешно прочитан
 */
bool read_words(const char *fname, HashTable table)
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

    fclose(hfile);
    return true;
}


/**
 * @brief      Вывод в консоль результата
 *
 * @param[in]  table  The table
 */
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

    int wc = 0;
    int chain = 0;
    int max_chain = 0;
    int empty_cells = 0;

    for (int i = 0; i < HASH_TABLE_SIZE; i++)
    {
        if (table[i].data != NULL)
        {
            chain = 0;
            word_descr = (struct WordDescr *)table[i].data;
            while (word_descr != NULL)
            {
                printf("\"%s\" %ld\n", word_descr->word, word_descr->counter);
                word_descr = word_descr->next;
                chain++;
                wc++;
            }
            if(max_chain < chain)
            {
                max_chain = chain;
            }
        }
        else
        {
            empty_cells++;
        }
    }
    printf("\nВсего слов: %d\n", wc);
    printf("Пустых ячеек: %d из %d\n", empty_cells, HASH_TABLE_SIZE);
    printf("Максимальная длина цепочки: %d\n", max_chain);

    return;
}


/**
 * @brief      Удаление цепочки для одной ячейки таблицы
 *
 * @param      word_descr  The word description
 */
void free_word_descr(struct WordDescr *word_descr)
{
    if (word_descr->next != NULL)
        free_word_descr(word_descr->next);
    if (word_descr->word != NULL)
        free(word_descr->word);
    free(word_descr);
}


/**
 * @brief      Удаоение таблицы
 *
 * @param[in]  table  The table
 */
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
