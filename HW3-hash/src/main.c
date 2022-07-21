
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "hashtable.h"
#include "version.h"
#include "word_counter.h"

void print_usage(const char *prog_name)
{
    printf("Usage: %s <input file>\n", prog_name);
}

void print_version(const char *prog_name)
{
    printf("%s %d.%d.%d\n", prog_name, PROJECT_VERSION_MAJOR,
           PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
}

int main(int argc, char const *argv[])
{
    // Проверка аргументов командной строки
    if ((argc > 1) && (strcmp(argv[1], "--version") == 0))
    {
        print_version(argv[0]);
        return EXIT_SUCCESS;
    }
    if (argc < 2)
    {
        fprintf(stderr, "\nToo few parameters\n\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Инициализация таблицы
    HashTable *words_array = hashtable_init();

    // Анализ файла (подсчет слов)
    if (!read_words(argv[1], words_array))
    {
        hashtable_free(words_array);
        fprintf(stderr, "Can not read words from file \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Печать результата
    print_words_array(words_array);
    hashtable_free(words_array);

    return EXIT_SUCCESS;
}