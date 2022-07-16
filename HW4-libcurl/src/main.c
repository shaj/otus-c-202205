
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cjson/cJSON.h>
#include <curl/curl.h>

#include "version.h"
#include "getter.h"
#include "parser.h"

void print_usage(const char *prog_name)
{
    printf("Usage: %s <input file>\n", prog_name);
}

void print_version(const char *prog_name)
{
    printf("%s %d.%d.%d\n", prog_name, PROJECT_VERSION_MAJOR,
           PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
}



int main(int argc, char const *argv[])
{
    char uri[1024];

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
        strcpy(uri, "https://wttr.in/Moscow?format=j1");
        // exit(EXIT_FAILURE);
    }
    else
    {
        if(strlen(argv[1]) >= (sizeof(uri)-2))
        {
            strncpy(uri, argv[1], (sizeof(uri)-2));
            uri[(sizeof(uri)-1)] = '\0';
        }
        else
        {
            strcpy(uri, argv[1]);
        }
        
    }

    // Выполнение запроса
    char *resp = do_get(uri);
    if(resp == NULL)
    {
        fprintf(stderr, "Error while do request \"%s\"\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Разбор принятых данных
    struct WeatherData *w_data = parse_data(resp);
    free(resp);
    if(w_data == NULL)
    {
        fprintf(stderr, "Error while do request \"%s\"\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Печать структуры WeatherData
    print_weather_data(w_data);
    free_weather_data(w_data);

    return EXIT_SUCCESS;
}