
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "version.h"
#include "hashtable.h"


void print_usage(const char *prog_name)
{
    printf("Usage: %s <input file>\n", prog_name);
}


void print_version(const char *prog_name)
{
    printf("%s %d.%d.%d\n", prog_name, 
        PROJECT_VERSION_MAJOR,
        PROJECT_VERSION_MINOR,
        PROJECT_VERSION_PATCH);
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
    if(stat(fname, &sb) == -1)
    {
        return false;
    }
    if((sb.st_mode & S_IFMT) != S_IFREG)
    {
        return false;
    }
    return true;
}


int main(int argc, char const *argv[])
{
    int retval = EXIT_FAILURE;
    FILE *pfin = NULL;

    // Проверка аргументов командной строки
    if((argc > 1) && (strcmp(argv[1], "--version") == 0))
    {
        print_version(argv[0]);
        return EXIT_SUCCESS;
    }
    if(argc < 2)
    {
        fprintf(stderr, "\nToo few parameters\n\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Открытие входного файлов
    if(!regular_file_check(argv[1]))
    {
        fprintf(stderr, "The \"%s\" is not a regular file\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    pfin = fopen(argv[1], "rb");
    if(pfin == NULL)
    {
        fprintf(stderr, "Can not open file \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    HashTable words_array;
    hashtable_init(&words_array);
    // Анализ файла (подсчет слов)
    if(!read_words(pfin, &words_array))
    {
        fprintf(stderr, "Can not read words from file \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    fclose(pfin);

    // Печать результата
    print_words_array(&words_array);
    hashtable_delete(&words_array);

    return retval;
}