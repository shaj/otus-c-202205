
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <byteswap.h>

#include "version.h"
#include "tables.h"


typedef struct
{
    const char *name;
    const uint16_t *table;
} coding_type;


const coding_type coding_name[] = 
{
    {"cp-1251", cp_1251},
    {"koi8", koi8},
    {"iso-8859-5", iso_8859_5},
};

#define CODINGS (sizeof(coding_name)/sizeof(coding_name[0]))

void print_usage(const char *prog_name)
{
    printf("Usage: %s <input file> <file encoding> <output file>\n"
           "   file encoding is one of:\n", prog_name);
    for(size_t i=0; i<(sizeof(coding_name)/sizeof(coding_name[0])); i++)
    {
        printf("      \"%s\"\n", coding_name[i].name);
    }
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


/**
 * Перевод одного ASCII символа в unicode.
 * Результат записывается в файл.
 *
 * @param      pfile    файл для записи результата
 * @param[in]  char_in  символ для перевода
 * @param[in]  table    таблица соответствия
 *
 * @return     при успешном переводе и успешной записи в файл возвращает true
 */
bool translate_to_file(FILE *pfile, unsigned char char_in, const uint16_t *table)
{
    int cnt;
    int length;
    uint8_t out_buf[3];
    uint16_t out_val;

    if(char_in < 0x80)
    {
        cnt = fwrite(&char_in, 1, 1, pfile);
        if(cnt != 1)
        {
            perror("Can't write to file");
            return false;
        }
        return true;
    }

    out_val = table[char_in - 0x80];
    if(out_val & 0xf800)
    {
        out_buf[0] = 0xe0 | ((out_val >> 12) & 0x0f);
        out_buf[1] = 0x80 | ((out_val >> 6) & 0x3f);
        out_buf[2] = 0x80 | (out_val & 0x3f);
        length = 3;
    }
    else
    {
        out_buf[0] = 0xc0 | ((out_val >> 6) & 0x1f);
        out_buf[1] = 0x80 | (out_val & 0x3f);
        length = 2;
    }

    cnt = fwrite(&out_buf, length, 1, pfile);
    if(cnt != 1)
    {
        perror("Can't write to file");
        return false;
    }

    return true;
}


/**
 * Перевод входного файла по таблице в unicode,
 * и запись результата в выходной файл
 *
 * @param      pfin   входной файл
 * @param[in]  table  таблица перевода
 * @param      pfout  выходной файл
 *
 * @return     при успешно переводе возвращает true
 */
bool translate(FILE *pfin, const uint16_t *table, FILE *pfout)
{
    bool retval = false;
    uint8_t buf_8;

    while(fread(&buf_8, sizeof(buf_8), 1, pfin))
    {
        retval = translate_to_file(pfout, buf_8, table);
        if(retval == false)
        {
            break;
        }
    }
    return retval;
}


int main(int argc, char const *argv[])
{
    const uint16_t *in_table = NULL;
    size_t coding_iter;
    int retval = EXIT_FAILURE;
    FILE *pfin = NULL;
    FILE *pfout = NULL;

    // Проверка аргументов командной строки
    if((argc > 1) && (strcmp(argv[1], "--version") == 0))
    {
        print_version(argv[0]);
        return EXIT_SUCCESS;
    }
    if(argc < 4)
    {
        fprintf(stderr, "\nToo few parameters\n\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Поиск таблицы перевода
    for(coding_iter=0; coding_iter<CODINGS; coding_iter++)
    {
        if(strcmp(argv[2], coding_name[coding_iter].name) == 0)
        {
            in_table = coding_name[coding_iter].table;
            break;
        }
    }
    if(coding_iter == CODINGS)
    {
        fprintf(stderr, "Translate from coding \"%s\" not implemented\n", argv[2]);
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Открытие входного и выходного файлов
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
    pfout = fopen(argv[3], "wb");
    if(pfout == NULL)
    {
        fprintf(stderr, "Can not open file \"%s\" to write\n", argv[3]);
        if(pfin)
            fclose(pfin);
        exit(EXIT_FAILURE);
    }

    // Перевод
    if(!translate(pfin, in_table, pfout))
    {
        fprintf(stderr, "Translation is not complete. Errors have occurred.\n");
    }
    else
    {
        fprintf(stderr, "Translation complete.\n");
        retval = EXIT_SUCCESS;
    }

    fclose(pfin);
    fclose(pfout);

    return retval;
}