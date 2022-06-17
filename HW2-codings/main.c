
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <byteswap.h>

#include "version.h"

void print_usage(const char *prog_name)
{
    printf("Usage: %s <file to convert> <file encoding>\n"
           "   file encoding: one of\n"
           "       \"cp-1252\",\n"
           "       \"koi-8\",\n"
           "       \"iso-8859-5\"\n", prog_name);
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
    if(stat(argv[1], &sb) == -1)
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