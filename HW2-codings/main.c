
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
    uint16_t *table;
} coding_type;


const coding_type coding_name[] = 
{
    {"cp-1252", cp_1252},
    {"koi-8", cp_1252},
    {"iso-8859-5" cp_1252},
};


void print_usage(const char *prog_name)
{
    printf("Usage: %s <file to convert> <file encoding>\n"
           "   file encoding: one of:\n", prog_name);
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


bool translate_to_stream(FILE *pfile, char char_in, uint16_t *table)
{
    int cnt;
    uint8_t out_buf[6];
    uint16_t out_val;

    if(char_in < 0x7f)
    {
        cnt = fwrite(&char_in, 1, 1, pfile);
        if(cnt != 1)
        {
            perror("Can't write to file");
            return false;
        }
        return true;
    }

    out_val = table[(char_in - 0x7f)];
    out_buf[0] = 0xd0;
    
    return false;
}


int main(int argc, char const *argv[])
{
    if((argc > 1) && (strcmp(argv[1], "--version") == 0))
    {
        print_version(argv[0]);
        return 0;
    }
    if(argc < 3)
    {
        print_usage(argv[0]);
        return 0;
    }

    int ret_val = 1;
    FILE *pfile = NULL;

    if(!regular_file_check(argv[1]))
    {
        fprintf(stderr, "The \"%s\" is not a regular file\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    pfile = fopen(argv[1], "rb");
    if(pfile == NULL)
    {
        fprintf(stderr, "Can not open file \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    

    fclose(pfile);

    return ret_val;
}