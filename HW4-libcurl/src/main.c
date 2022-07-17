
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <cjson/cJSON.h>
#include <curl/curl.h>

#include "getter.h"
#include "parser.h"
#include "version.h"

void print_usage(const char *prog_name)
{
    printf("Usage: %s <location>\n", prog_name);
    printf("Supported location types:\n"
           "    /paris                  # city name\n"
           "    /~Eiffel+tower          # any location (+ for spaces)\n"
           "    /Москва                 # Unicode name of any location in any "
           "language\n"
           "    /muc                    # airport code (3 letters)\n"
           "    /@stackoverflow.com     # domain name\n"
           "    /94107                  # area codes\n"
           "    /-78.46,106.79          # GPS coordinates\n\n");
}

void print_version(const char *prog_name)
{
    printf("%s %d.%d.%d\n", prog_name, PROJECT_VERSION_MAJOR,
           PROJECT_VERSION_MINOR, PROJECT_VERSION_PATCH);
}

int main(int argc, char const *argv[])
{
    char *url;

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
        url = make_url("Moscow");
    }
    else
    {
        url = make_url(argv[1]);
    }

    if (url == NULL)
    {
        fprintf(stderr, "Wrong place\n");
        return EXIT_FAILURE;
    }

    // Выполнение запроса
    char *resp = do_get(url);
    if (resp == NULL)
    {
        fprintf(stderr, "Error while do request \"%s\"\n", url);
        free(url);
        return EXIT_FAILURE;
    }

    free(url);

    // Разбор принятых данных
    parse_data(resp);
    free(resp);

    return EXIT_SUCCESS;
}