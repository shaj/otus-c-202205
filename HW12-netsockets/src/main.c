
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "version.h"

void print_usage(const char *prog_name) { printf("Usage: %s <input file>\n", prog_name); }

void print_version(const char *prog_name)
{
    printf("%s %d.%d.%d\n", prog_name, PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR,
           PROJECT_VERSION_PATCH);
}

int main(int argc, char const *argv[])
{
    int retval = EXIT_SUCCESS;
    int fd;
    struct stat sb;
    char *addr = NULL;
    uint32_t crc;

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

    // Открытие входного файлов
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "Can not open file \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    if(fstat(fd, &sb) == -1)
    {
        fprintf(stderr, "Can not stat file \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    if ((sb.st_mode & S_IFMT) != S_IFREG)
    {
        fprintf(stderr, "The \"%s\" is not a regular file\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    addr = mmap(addr, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (addr == MAP_FAILED)
    {
        fprintf(stderr, "Can not mmap file \"%s\" at length %lu\n",
                argv[1], sb.st_size);
        retval = EXIT_FAILURE;
        goto exit_1;
    }

    crc = 0;
    crc = crc32(crc, addr, sb.st_size);

    printf("%x  %s\n", crc, argv[1]);

    munmap(addr, sb.st_size);
exit_1:
    close(fd);

    return retval;
}