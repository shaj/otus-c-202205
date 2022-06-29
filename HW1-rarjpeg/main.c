
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <byteswap.h>

#include "pkzip.h"

/**
 * @brief Маркеры формата JPEG
 *
 */
enum
{
    JPEG_SOI = 0xd8ff,
    JPEG_S0F0 = 0xc0ff,
    JPEG_S0F2 = 0xc2ff,
    JPEG_DHT = 0xc4ff,
    JPEG_DQT = 0xdbff,
    JPEG_DRI = 0xddff,
    JPEG_SOS = 0xdaff,
    JPEG_COM = 0xfeff,
    JPEG_EOI = 0xd9ff,
    JPEG_RSTn = 0xd0ff,
    JPEG_APPn = 0xe0ff
};

void print_usage(const char *prog_name)
{
    printf("%s <rarjpeg file name>\n", prog_name);
}

void print_version(const char *prog_name) { printf("%s 0.0.0\n", prog_name); }

/**
 * @brief Проверка формата файла
 *
 * Функция проверяет, что файл является JPEG файлом
 *
 * @param pfile - указатель на объект FILE, открытый
 *                для чтения в бинарном режиме
 * @return true - файл является JPEG файлом
 * @return false - файл не является JPEG файлом
 */
bool test_file(FILE *pfile)
{
    size_t cnt;
    uint16_t buf;
    if (fseek(pfile, 0, SEEK_SET) != 0)
    {
        return false;
    }
    cnt = fread(&buf, 2, 1, pfile);
    return ((cnt == 1) && (buf == JPEG_SOI));
}

/**
 * @brief Определение позиции конца формата JPEG
 *
 * @param pfile - указатель на объект FILE, открытый
 *                для чтения в бинарном режиме
 * @param pos - указатель на структуру fpos_t в которой
 *              возвращается позиция конца формата JPEG
 * @return true - конец JPEG найден и позиция записана в pos
 * @return false - конец JPEG не найден. pos остается без изменений
 */
bool get_extpart_pos(FILE *pfile, fpos_t *pos)
{
    size_t cnt;
    uint16_t buf_16;
    uint8_t buf_8;
    bool jpeg_end_found = false;
    bool jpeg_err = false;

    if (fseek(pfile, 0, SEEK_SET) != 0)
    {
        return false;
    }
    cnt = fread(&buf_16, sizeof(buf_16), 1, pfile);
    if ((cnt != 1) && (buf_16 != JPEG_SOI))
    {
        return false;
    }
    while (!jpeg_end_found && !jpeg_err)
    {
        cnt = fread(&buf_16, sizeof(buf_16), 1, pfile);
        if (cnt != 1)
        {
            jpeg_err = true;
            break;
        }
        switch (buf_16)
        {
        case JPEG_S0F0:
        case JPEG_S0F2:
        case JPEG_DHT:
        case JPEG_DQT:
        case JPEG_APPn:
        case JPEG_RSTn:
        case JPEG_COM:
        {
            cnt = fread(&buf_16, sizeof(buf_16), 1, pfile);
            if (cnt != 1)
            {
                perror("Unexpected end of file\n");
                jpeg_err = true;
                break;
            }
            // Переставить байты в смещении
            buf_16 = __bswap_16(buf_16);
            if (fseek(pfile, buf_16 - 2, SEEK_CUR) != 0)
            {
                perror("Error parsing JPEG format\n");
                jpeg_err = true;
                break;
            }
            break;
        }
        case JPEG_SOS:
        {
            cnt = fread(&buf_16, sizeof(buf_16), 1, pfile);
            if (cnt != 1)
            {
                perror("Unexpected end of file\n");
                jpeg_err = true;
                break;
            }
            buf_16 = ((buf_16 & 0x00FF) << 8) | ((buf_16 & 0xFF00) >> 8);
            if (fseek(pfile, buf_16 - 2, SEEK_CUR) != 0)
            {
                perror("Error parsing JPEG format\n");
                jpeg_err = true;
                break;
            }
            while (!jpeg_end_found && !jpeg_err)
            {
                cnt = fread(&buf_8, sizeof(buf_8), 1, pfile);
                if (cnt != 1)
                {
                    perror("Unexpected end of file\n");
                    jpeg_err = true;
                    break;
                }
                if (buf_8 == 0xff)
                {
                    cnt = fread(&buf_8, 1, 1, pfile);
                    if (cnt != 1)
                    {
                        perror("Unexpected end of file\n");
                        jpeg_err = true;
                        break;
                    }
                    if (buf_8 == 0xd9)
                    {
                        if (fgetpos(pfile, pos) != 0)
                        {
                            jpeg_err = true;
                            perror("Error parsing JPEG format\n");
                            break;
                        }
                        jpeg_end_found = true;
                        break;
                    }
                }
            }
            break;
        }
        case JPEG_DRI:
        {
            break;
        }
        case JPEG_EOI:
        {
            if (fgetpos(pfile, pos) != 0)
            {
                jpeg_err = true;
                perror("Error parsing JPEG format");
                break;
            }
            jpeg_end_found = true;
            break;
        }
        default:
            jpeg_err = true;
            fprintf(stderr, "jpeg err. Readed 0x%04x\n", buf_16);
            break;
        };
    }

    return jpeg_end_found;
}

/**
 * @brief Проверка наличия и формата области файла за картинкой
 *
 * @param pfile - указатель на объект FILE, открытый
 *                для чтения в бинарном режиме
 * @return true - данные есть и они в формате zip
 * @return false - данных нет или формат данных не определен
 */
bool test_extpart(FILE *pfile)
{
    size_t cnt;
    uint32_t buf;

    if (fseek(pfile, -sizeof(EOCDR), SEEK_END) != 0)
    {
        perror("Error while reading zip format\n");
        return false;
    }
    cnt = fread(&buf, sizeof(buf), 1, pfile);
    if (cnt != 1)
    {
        return false;
    }

    if (buf == 0x06054B50)
    {
        return true;
    }
    return false;
}

/**
 * @brief Печать списка файлов из zip архива прилепленного к картинке
 *
 * @param pfile - указатель на объект FILE, открытый
 *                для чтения в бинарном режиме
 * @param zip_offset - смещение начала zip архива в файле
 */
void print_zip_file_list(FILE *pfile, fpos_t *zip_offset)
{
    size_t cnt;
    CentralDirectoryFileHeader cdfh;
    EOCDR eocdr;
    char *str;
    size_t strlen;

    if (fseek(pfile, -sizeof(eocdr), SEEK_END) != 0)
    {
        perror("Error while reading zip format\n");
        return;
    }
    cnt = fread(&eocdr, sizeof(eocdr), 1, pfile);
    if (cnt != 1)
    {
        perror("Error while reading zip format\n");
        return;
    }

    if (fsetpos(pfile, zip_offset) != 0)
    {
        perror("Error while reading zip format\n");
        return;
    }
    if (fseek(pfile, eocdr.centralDirectoryOffset, SEEK_CUR) != 0)
    {
        perror("Error while reading zip format\n");
        return;
    }
    for (int i = 0; i < eocdr.totalCentralDirectoryRecord; i++)
    {
        cnt = fread(&cdfh, sizeof(cdfh), 1, pfile);
        if (cnt != 1)
        {
            perror("Error while reading zip format\n");
        }

        strlen = cdfh.filenameLength + cdfh.extraFieldLength +
                 cdfh.fileCommentLength;
        str = malloc(strlen + 1);
        if (str != NULL)
        {
            cnt = fread(str, strlen, 1, pfile);
            if (cnt != 1)
            {
                perror("Error while reading zip format\n");
            }
            str[cdfh.filenameLength] = 0;
            printf("%s\n", str);
            free(str);
        }
        else
        {
            perror("Out of memory");
            break;
        }
    }

    return;
}

int main(int argc, char const *argv[])
{
    if (argc == 1)
    {
        print_usage(argv[0]);
        return 0;
    }
    if (strcmp(argv[1], "--version") == 0)
    {
        print_version(argv[0]);
        return 0;
    }

    printf("File for analise \"%s\"\n", argv[1]);

    struct stat sb;
    FILE *pfile = NULL;

    if (stat(argv[1], &sb) == -1)
    {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    if ((sb.st_mode & S_IFMT) != S_IFREG)
    {
        fprintf(stderr, "The \"%s\" is not a regular file\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    pfile = fopen(argv[1], "rb");
    if (pfile == NULL)
    {
        fprintf(stderr, "Can not open file \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    fpos_t zip_offset;
    int ret_val = 0;

    if (test_file(pfile))
    {
        get_extpart_pos(pfile, &zip_offset);
        if (test_extpart(pfile))
        {
            print_zip_file_list(pfile, &zip_offset);
            ret_val = 0;
        }
        else
        {
            printf("File \"%s\" does not have extended patition\n", argv[1]);
            ret_val = 1;
        }
    }
    else
    {
        printf("File \"%s\" is not a correct jpeg file\n", argv[1]);
        ret_val = 1;
    }

    fclose(pfile);

    return ret_val;
}