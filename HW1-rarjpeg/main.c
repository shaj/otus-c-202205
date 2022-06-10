
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

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
    // JPEG_RSTn = 0xd0ff,
    // JPEG_APPn = 0xe0ff
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

fpos_t get_extpart_offset(FILE *pfile)
{
    size_t cnt;
    uint16_t buf;
    bool jpeg_end_found = false;
    fpos_t jpeg_end_pos = 0;

    fseek(pfile, 0, SEEK_SET);
    cnt = fread(&buf, 2, 1, pfile);
    if((cnt != 1) && (buf != JPEG_SOI))
    {
        return false;
    }
    while(!jpeg_end_found)
    {
        cnt = fread(&buf, 2, 1, pfile);
        if(cnt != 1)
        {
            break;
        }
        switch(buf)
        {
        case JPEG_S0F0:
        case JPEG_S0F2:
        case JPEG_DHT:
        case JPEG_DQT:
        case JPEG_SOS:
        case JPEG_COM:
        {
            break;
        }
        case JPEG_DRI:
        {
            break;
        }
        case JPEG_EOI:
        {
            fgetpos(pfile, &jpeg_end_pos);
            jpeg_end_found = true;
            break;
        }
        default:
            break;
        };
    }

    return jpeg_end_pos;
}

bool test_extpart(FILE *pfile, size_t zip_offset)
{
    return false;
}

void print_zip_file_list(FILE *pfile, size_t zip_offset)
{
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

    size_t zip_offset;

    if(test_file(pfile))
    {
        zip_offset = get_end_jpeg(pfile);
        if(zip_offset == 0)
        {
            printf("File \"%s\" does not have extended patition\n", argv[1]);
            fclose(pfile);
            exit(1);
        }
        print_zip_file_list(pfile, zip_offset);
    }
    else
    {
        printf("File \"%s\" is not a correct jpeg file\n", argv[1]);
    }

    fclose(pfile);

    return 0;
}