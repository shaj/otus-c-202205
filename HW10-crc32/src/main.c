
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
    if (stat(fname, &sb) == -1)
    {
        return false;
    }
    if ((sb.st_mode & S_IFMT) != S_IFREG)
    {
        return false;
    }
    return true;
}

int main(int argc, char const *argv[])
{
    int retval = EXIT_FAILURE;
    int fd;

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
    if (!regular_file_check(argv[1]))
    {
        fprintf(stderr, "The \"%s\" is not a regular file\n", argv[1]);
        exit(EXIT_FAILURE);
    }
    fd = open(argv[1], O_RDONLY);
    if (fd == -1)
    {
        fprintf(stderr, "Can not open file \"%s\"\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    struct stat sb;
    char *addr = NULL;
    size_t length = (100 * 1024 * 1024) & ~(sysconf(_SC_PAGE_SIZE) - 1);
    off_t offset = 0;
    // off_t pa_offset;
    off_t debug_val = 0;

    fstat(fd, &sb);

    printf("System page size %lu\n", length);
    printf("file <%s>  size %zi  blocks %lu\n", argv[1], sb.st_size, sb.st_blocks);

    for(offset = 0; offset < sb.st_size; offset += length)
    {
        if((sb.st_size - offset) < length) 
            length = sb.st_size - offset;
        addr = mmap(addr, length, PROT_READ, MAP_PRIVATE, fd, offset);
        if (addr == MAP_FAILED)
        {
            fprintf(stderr, "Can not mmap file \"%s\" at length %lu  offset %lu\n",
                    argv[1], length, offset);
            retval = EXIT_FAILURE;
            goto exit_1;
        }
        debug_val  += 1;
    }

exit_1:
    printf("debug_val = %lu %lu\n", debug_val, length);
    munmap(addr, length);
    close(fd);

    return retval;
}