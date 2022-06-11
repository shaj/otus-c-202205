
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "pkzip.h"

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

bool test_file(FILE *pfile)
{
    size_t cnt;
    uint16_t buf;
    fseek(pfile, 0, SEEK_SET);
    cnt = fread(&buf, 2, 1, pfile);
    return ((cnt == 1) && (buf == JPEG_SOI));
}

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
            printf("0x%04X  ", buf);
            cnt = fread(&buf, sizeof(buf), 1, pfile);
            if(cnt != 1)
            {
                printf("Unexpected end of file\n");
                jpeg_err = true;
                break;
            }
            buf = ((buf & 0x00FF) << 8) | ((buf & 0xFF00) >> 8);
            printf("0x%04X\n", buf);
            fseek(pfile, buf-2, SEEK_CUR);
            break;
        }
        case JPEG_SOS:
        {
            printf("JPEG_SOS\n");
            cnt = fread(&buf, sizeof(buf), 1, pfile);
            if(cnt != 1)
            {
                printf("Unexpected end of file\n");
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
                    printf("Unexpected end of file\n");
                    jpeg_err = true;
                    break;
                }
                if(buf1 == 0xff)
                {
                    cnt = fread(&buf1, 1, 1, pfile);
                    if(cnt != 1)
                    {
                        printf("Unexpected end of file\n");
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
            printf("jpeg err. Readed 0x%04x\n", buf);
            break;
        };
    }

    return jpeg_end_found;
}

bool test_extpart(FILE *pfile, fpos_t *zip_offset)
{
    size_t cnt;
    EOCDR eocdr;

    fseek(pfile, -sizeof(eocdr), SEEK_END);
    cnt = fread(&eocdr, sizeof(eocdr), 1, pfile);
    if(cnt != 1)
    {
        return false;
    }

    if(eocdr.signature == 0x06054B50)
    {
        return true;
    }
    return false;
}

void print_zip_file_list(FILE *pfile, fpos_t *zip_offset)
{
    size_t cnt;
    CentralDirectoryFileHeader cdfh;
    EOCDR eocdr;
    char *str;
    size_t strlen;

    // fsetpos(pfile, zip_offset);
    fseek(pfile, -sizeof(eocdr), SEEK_END);
    cnt = fread(&eocdr, sizeof(eocdr), 1, pfile);
    if(cnt != 1)
    {
        printf("Error while reading zip format (1)\n");
        return;
    }

    printf("EOCDR sig 0x%08X\neocdr is %lu\n", eocdr.signature, sizeof(eocdr));
    printf("%d\n%d\n%d\n%d\n%d\n", eocdr.numberCentralDirectoryRecord,
            eocdr.totalCentralDirectoryRecord,
            eocdr.sizeOfCentralDirectory,
            eocdr.centralDirectoryOffset,
            eocdr.commentLength);

    fsetpos(pfile, zip_offset);
    fseek(pfile, eocdr.centralDirectoryOffset, SEEK_CUR);
    for (int i = 0; i < eocdr.totalCentralDirectoryRecord; i++)
    {
        cnt = fread(&cdfh, sizeof(cdfh), 1, pfile);
        if(cnt != 1)
        {
            printf("Error while reading zip format (2)\n");
        }

        // printf("\nCentralDirectoryFileHeader sig 0x%08X\nfilenameLength %d\n", cdfh.signature, cdfh.filenameLength);
        strlen = cdfh.filenameLength + cdfh.extraFieldLength + cdfh.fileCommentLength;
        str = malloc(strlen + 1);
        cnt = fread(str, strlen, 1, pfile);
        if(cnt != 1)
        {
            printf("Error while reading zip format (2)\n");
        }
        // str[strlen] = 0;
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
        printf("Can not open file \"%s\"\n", argv[1]);
        exit(1);
    }

    fpos_t zip_offset;
    int ret_val = 0;

    if(test_file(pfile))
    {
        get_extpart_pos(pfile, &zip_offset);
        if(test_extpart(pfile, &zip_offset))
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