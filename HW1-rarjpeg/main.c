
#include <stdio.h>
#include <string.h>

void print_usage(const char *prog_name)
{
    printf("%s\n", prog_name);
}

void print_version(const char *prog_name)
{
    printf("%s 0.0.0\n", prog_name);
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

    printf("File for analise %s\n", argv[1]);

    return 0;
}