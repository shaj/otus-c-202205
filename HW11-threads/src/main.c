
#include <dirent.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "set.h"
#include "version.h"

void print_usage(const char *prog_name)
{
    printf("Usage: %s <number of threads> <log directory>\n", prog_name);
}

void print_version(const char *prog_name)
{
    printf("%s %d.%d.%d\n", prog_name, PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR,
           PROJECT_VERSION_PATCH);
}

struct StatData
{
    size_t cnt;
    size_t all_objects_size;
    size_t status_err;
    SimpleSet part_cnts;
};

void init_stat_data(struct StatData *data)
{
    data->cnt = 0;
    data->all_objects_size = 0;
    data->status_err = 0;
    set_init(&data->part_cnts);
}

void delete_stat_data(struct StatData *data)
{
    // ...
    set_destroy(&data->part_cnts);
}

int line_parser(struct StatData *pdata, const char *line, int len)
{
    pdata->cnt++;

    const char *pws = line;
    const char *request;
    int request_len;
    const char *status_code_str;
    int status_code_len;
    int status_code;
    const char *size_object_str;
    int size_object_len;
    int size_object;
    const char *referer;
    int referer_len;

    int ret;

    int part_cnt = 0;
    char out_str[64];

    pws = strchr(pws, ' '); // hyphen
    if (pws == NULL)
        return -1;
    pws++;
    pws = strchr(pws, ' '); // userid
    if (pws == NULL)
        return -1;
    pws++;
    pws = strchr(pws, ' '); // time
    if (pws == NULL)
        return -1;
    pws++;
    pws = strstr(pws, "] \""); // request
    if (pws == NULL)
        return -1;
    pws += 3;
    request = pws;
    pws = strstr(pws, "\" "); // status code
    if (pws == NULL)
        return -1;
    pws += 2;
    request_len = pws - request;
    status_code_str = pws;
    pws = strchr(pws, ' '); // size of the object
    if (pws == NULL)
        return -1;
    pws++;
    status_code_len = pws - status_code_str;
    size_object_str = pws;
    pws = strstr(pws, " \""); // referer
    if (pws == NULL)
        return -1;
    pws += 2;
    size_object_len = pws - size_object_str;
    referer = pws;
    pws = strstr(pws, "\" \""); // user_agent
    if (pws == NULL)
        return -1;
    pws += 3;
    referer_len = pws - referer;
    pws = strchr(pws, '\"');
    if (pws == NULL)
    {
        return -2;
    }
    pws++;
    if(*pws == ' ')
    {
        // printf("^^^%s\n", pws);
        return -2;
    }

    ret = sscanf(status_code_str, " %d ", &status_code);
    if (ret != 1)
    {
        fprintf(stderr, "status code err: %d %d : %0.10s\n", ret, status_code, status_code_str);
        return -3;
    }
    ret = sscanf(size_object_str, "%d ", &size_object);
    if (ret != 1)
    {
        fprintf(stderr, "size obect err: %d %d\n", ret, status_code);
        return -3;
    }

    if((status_code >= 200) && (status_code < 300))
    {
        pdata->all_objects_size += size_object;
    }
    else
    {
        pdata->status_err++;
    }

    (void)len;
    return 0;
}

void print_report(struct StatData *pdata)
{
    size_t ui;
    char **rep;

    printf("\n\nCNT = %lu\n", pdata->cnt);
    rep = set_to_array(&pdata->part_cnts, &ui);
    printf("Parts: ");
    for (size_t i = 0; i < ui; i++)
    {
        printf("%s ", rep[i]);
        free(rep[i]);
    }
    free(rep);
    printf("\nAll objects size: %lu\nStatus errors: %lu\n", pdata->all_objects_size, pdata->status_err);

}

int main(int argc, char const *argv[])
{
    int retval = EXIT_FAILURE;
    struct stat sb;
    int number_threads;

    // Проверка аргументов командной строки
    if ((argc > 1) && (strcmp(argv[1], "--version") == 0))
    {
        print_version(argv[0]);
        return EXIT_SUCCESS;
    }
    if (argc < 3)
    {
        fprintf(stderr, "\nToo few parameters\n\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    number_threads = atoi(argv[1]);
    if ((number_threads <= 0) || (number_threads > 50))
    {
        fprintf(stderr, "\nNumber of threads must be in 1 .. 50\n\n");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    if (stat(argv[2], &sb) == -1)
    {
        perror("Could not stat log directory");
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }
    if ((sb.st_mode & S_IFMT) != S_IFDIR)
    {
        fprintf(stderr, "The \"%s\" is not a directory\n", argv[2]);
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    struct dirent *de;
    DIR *dir = opendir(argv[2]);
    if (dir == NULL)
    {
        fprintf(stderr, "Could not open directory \"%s\"\n", argv[2]);
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    // Подготовить пулл потоков

    struct StatData pdata[1];
    char full_name[2048];
    init_stat_data(pdata);

    // Запустить сканирование
    while ((de = readdir(dir)) != NULL)
    {
        strcpy(full_name, argv[2]);
        strcat(full_name, "/");
        strcat(full_name, de->d_name);
        printf("%s\n", full_name);

        if (stat(full_name, &sb) == -1)
        {
            putc('1', stdout);
            fflush(stdout);
            continue;
        }
        if ((sb.st_mode & S_IFMT) != S_IFREG)
        {
            putc('2', stdout);
            fflush(stdout);
            continue;
        }

        FILE *fp;
        char *line = NULL;
        size_t len = 0;
        ssize_t read;
        int line_cnt = 0;

        fp = fopen(full_name, "r");
        if (fp == NULL)
        {
            putc('3', stdout);
            fflush(stdout);
            continue;
        }

        while ((read = getline(&line, &len, fp)) != -1)
        {
            line_cnt++;

            if (len == 0)
            {
                putc('4', stdout);
                fflush(stdout);
                continue;
            }
            int ret = line_parser(pdata, line, len);
            // if (ret != 0)
            // {
            //     fprintf(stderr, "Parsing error in %s : %d %d\n", de->d_name, line_cnt, ret);
            // }
        }

        fclose(fp);
        if (line)
        {
            free(line);
        }
    }

    // Вывести отчет
    print_report(pdata);

    delete_stat_data(pdata);
    closedir(dir);

    return retval;
}