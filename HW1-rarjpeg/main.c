
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "pkzip.h"


/**
 * @brief Маркеры формата JPEG
 * 
 */
enum {
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

void print_version(const char *prog_name)
{
    printf("%s 0.0.0\n", prog_name);
}


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
    fseek(pfile, 0, SEEK_SET);
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
    uint16_t buf;
    uint8_t buf1;
    bool jpeg_end_found = false;
    bool jpeg_err = false;

    fseek(pfile, 0, SEEK_SET);
    cnt = fread(&buf, 2, 1, pfile);
    if((cnt != 1) && (buf != JPEG_SOI))
    {
        return false;
    }
    while(!jpeg_end_found && !jpeg_err)
    {
        cnt = fread(&buf, sizeof(buf), 1, pfile);
        if(cnt != 1)
        {
            jpeg_err = true;
            break;
        }
        switch(buf)
        {
        case JPEG_S0F0:
        case JPEG_S0F2:
        case JPEG_DHT:
        case JPEG_DQT:
        case JPEG_APPn:
        case JPEG_RSTn:
        case JPEG_COM:
        {
            cnt = fread(&buf, sizeof(buf), 1, pfile);
            if(cnt != 1)
            {
                perror("Unexpected end of file\n");
                jpeg_err = true;
                break;
            }
            buf = ((buf & 0x00FF) << 8) | ((buf & 0xFF00) >> 8);
            fseek(pfile, buf-2, SEEK_CUR);
            break;
        }
        case JPEG_SOS:
        {
            cnt = fread(&buf, sizeof(buf), 1, pfile);
            if(cnt != 1)
            {
                perror("Unexpected end of file\n");
                jpeg_err = true;
                break;
            }
            buf = ((buf & 0x00FF) << 8) | ((buf & 0xFF00) >> 8);
            fseek(pfile, buf-2, SEEK_CUR);
            while(!jpeg_end_found && !jpeg_err)
            {
                cnt = fread(&buf1, 1, 1, pfile);
                if(cnt != 1)
                {
                    perror("Unexpected end of file\n");
                    jpeg_err = true;
                    break;
                }
                if(buf1 == 0xff)
                {
                    cnt = fread(&buf1, 1, 1, pfile);
                    if(cnt != 1)
                    {
                        perror("Unexpected end of file\n");
                        jpeg_err = true;
                        break;
                    }
                    if(buf1 == 0xd9)
                    {
                        fgetpos(pfile, pos);
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
            fgetpos(pfile, pos);
            jpeg_end_found = true;
            break;
        }
        default:
            jpeg_err = true;
            fprintf(stderr, "jpeg err. Readed 0x%04x\n", buf);
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

    fseek(pfile, -sizeof(EOCDR), SEEK_END);
    cnt = fread(&buf, sizeof(buf), 1, pfile);
    if(cnt != 1)
    {
        return false;
    }

    if(buf == 0x06054B50)
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

    fseek(pfile, -sizeof(eocdr), SEEK_END);
    cnt = fread(&eocdr, sizeof(eocdr), 1, pfile);
    if(cnt != 1)
    {
        perror("Error while reading zip format\n");
        return;
    }

    fsetpos(pfile, zip_offset);
    fseek(pfile, eocdr.centralDirectoryOffset, SEEK_CUR);
    for (int i = 0; i < eocdr.totalCentralDirectoryRecord; i++)
    {
        cnt = fread(&cdfh, sizeof(cdfh), 1, pfile);
        if(cnt != 1)
        {
            perror("Error while reading zip format\n");
        }

        strlen = cdfh.filenameLength + cdfh.extraFieldLength + cdfh.fileCommentLength;
        str = malloc(strlen + 1);
        cnt = fread(str, strlen, 1, pfile);
        if(cnt != 1)
        {
            perror("Error while reading zip format\n");
        }
        str[cdfh.filenameLength] = 0;
        printf("%s\n", str);
        free(str);
    }

    return;
}

int main(int argc, char const *argv[])
{
    if(argc == 1)
    {
        print_usage(argv[0]);
        return 0;
    }
    if(strcmp(argv[1], "--version") == 0)
    {
        print_version(argv[0]);
        return 0;
    }

    printf("File for analise \"%s\"\n", argv[1]);

    FILE *pfile = NULL;
    pfile = fopen(argv[1], "rb");
    if(pfile == NULL)
    {
        fprintf(stderr, "Can not open file \"%s\"\n", argv[1]);
        exit(1);
    }

    fpos_t zip_offset;
    int ret_val = 0;

    if(test_file(pfile))
    {
        get_extpart_pos(pfile, &zip_offset);
        if(test_extpart(pfile))
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