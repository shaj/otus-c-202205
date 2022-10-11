
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

#include <pthread.h>
#include <stdatomic.h>

#include "hashtable.h"
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


struct FileThreadData
{
    char *file_name;
    struct StatData *pdata;
};

struct ThrdList
{
    pthread_t pthrd;
    struct FileThreadData *thrd_data;
    struct ThrdList *next;
};

void ThrdList_init(struct ThrdList **list)
{
    *list = NULL;
}

void ThrdList_push(struct ThrdList **list, pthread_t pthrd, struct FileThreadData *data)
{
    struct ThrdList *new_it = malloc(sizeof(struct ThrdList));
    new_it->next = *list;
    new_it->pthrd = pthrd;
    new_it->thrd_data = data;
    *list = new_it;
}

pthread_t ThrdList_pop(struct ThrdList **list)
{
    if((list != NULL) && (*list != NULL))
    {
        pthread_t pthrd = (*list)->pthrd;
        struct ThrdList *new_list = (*list)->next;
        free((*list)->thrd_data->file_name);
        free((*list)->thrd_data);
        free(*list);
        *list = new_list;
        return pthrd;
    }
    else
    {
        return 0;
    }
}

struct StatData
{
    atomic_size_t cnt;
    atomic_size_t all_objects_size;
    atomic_size_t status_err;
    HashTable *part_cnts;
    HashTable *urls;
    HashTable *referers;

    pthread_mutex_t part_mutex;
    pthread_mutex_t urls_mutex;
    pthread_mutex_t referers_mutex;
    pthread_mutex_t cnt_mutex;
};

void init_stat_data(struct StatData *data)
{
    data->cnt = 0;
    data->all_objects_size = 0;
    data->status_err = 0;
    data->part_cnts = hashtable_init();;
    data->urls = hashtable_init();
    data->referers = hashtable_init();

    pthread_mutex_init(&(data->part_mutex), NULL);
    pthread_mutex_init(&(data->urls_mutex), NULL);
    pthread_mutex_init(&(data->referers_mutex), NULL);
    pthread_mutex_init(&(data->cnt_mutex), NULL);
}

void delete_stat_data(struct StatData *data)
{
    hashtable_free(data->part_cnts);
    hashtable_free(data->urls);
    hashtable_free(data->referers);

    pthread_mutex_destroy(&(data->part_mutex));
    pthread_mutex_destroy(&(data->urls_mutex));
    pthread_mutex_destroy(&(data->referers_mutex));
    pthread_mutex_destroy(&(data->cnt_mutex));
}

char *urldup(const char *request, int request_len)
{
    const char *pws;
    const char *url;
    const char *args;
    int url_size = 0;

    url = strchr(request, ' ');
    if (url == NULL)
    {
        return NULL;
    }
    url++;
    pws = strchr(url, ' ');
    if ((pws == NULL) || (pws > (request + request_len)))
    {
        url_size = request_len - (url - request);
    }
    else
    {
        args = strchr(url, '?');
        if ((args == NULL) || (args > (request + request_len)))
        {
            url_size = pws - url;
        }
        else
        {
            url_size = args - url;
        }
    }

    if (url_size == 0)
        return NULL;
    return strndup(url, url_size);
}

int line_parser(struct StatData *pdata, const char *line)
{
    atomic_fetch_add(&(pdata->cnt), 1);

    const char *pws = line;
    const char *request;
    int request_len;
    const char *status_code_str;
    int status_code_len;
    int status_code;
    const char *size_object_str;
    // int size_object_len;
    size_t size_object;
    const char *referer;
    char *referer_dup;
    int referer_len;
    char *url;
    size_t *url_payload;
    const struct WordInfo *wordinfo;

    int ret;

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
    request_len = pws - request;
    pws += 2;
    status_code_str = pws;
    pws = strchr(pws, ' '); // size of the object
    if (pws == NULL)
        return -1;
    status_code_len = pws - status_code_str;
    pws++;
    size_object_str = pws;
    pws = strstr(pws, " \""); // referer
    if (pws == NULL)
        return -1;
    // size_object_len = pws - size_object_str;
    pws += 2;
    referer = pws;
    pws = strstr(pws, "\" \""); // user_agent
    if (pws == NULL)
        return -1;
    referer_len = pws - referer;
    pws += 3;
    pws = strchr(pws, '\"');
    if (pws == NULL)
    {
        return -2;
    }
    pws++;
    if (*pws == ' ')
    {
        return -2;
    }

    ret = sscanf(status_code_str, " %d ", &status_code);
    if (ret != 1)
    {
        fprintf(stderr, "status code err: %d %d : %.10s\n", ret, status_code, status_code_str);
        return -3;
    }
    ret = sscanf(size_object_str, "%lu ", &size_object);
    if (ret != 1)
    {
        fprintf(stderr, "size obect err: %d %d\n", ret, status_code);
        return -3;
    }

    url = urldup(request, request_len);
    if (url != NULL)
    {
        url_payload = malloc(sizeof(size_t));
        if(url_payload != NULL)
            *url_payload = size_object;
        pthread_mutex_lock(&(pdata->urls_mutex));
        wordinfo = hashtable_add(pdata->urls, url, url_payload);
        if (wordinfo == NULL)
        {
            free(url);
            free(url_payload);
        }
        else if (wordinfo->counter != 1)
        {
            if((url_payload != NULL) && (wordinfo->value != NULL)
                && (*url_payload > *((size_t*)wordinfo->value)))
            {
                void *old_val = hashtable_set_value(pdata->urls, url, url_payload);
                free(old_val);
            }
            else
            {
                free(url_payload);
            }
            free(url);
        }
        pthread_mutex_unlock(&(pdata->urls_mutex));
    }

    if ((referer != NULL) && (referer_len != 0))
    {
        referer_dup = strndup(referer, referer_len);
        if (referer_dup != NULL)
        {
            pthread_mutex_lock(&(pdata->referers_mutex));
            wordinfo = hashtable_add(pdata->referers, referer_dup, NULL);
            if((wordinfo == NULL) || (wordinfo->counter != 1))
            {
                free(referer_dup);
            }
            pthread_mutex_unlock(&(pdata->referers_mutex));
        }
    }

    char *status_code_str_dup = strndup(status_code_str, status_code_len);
    if (status_code_str_dup != NULL)
    {
        pthread_mutex_lock(&(pdata->part_mutex));
        wordinfo = hashtable_add(pdata->part_cnts, status_code_str_dup, NULL);
        if((wordinfo == NULL) || (wordinfo->counter != 1))
        {
            free(status_code_str_dup);
        }
        pthread_mutex_unlock(&(pdata->part_mutex));
    }
    // if((status_code >= 200) && (status_code < 300))
    // if(status_code < 400)
    // {
    atomic_fetch_add(&(pdata->all_objects_size), size_object);
    // }
    // else
    // {
    //     atomic_fetch_add(&(pdata->status_err), 1);
    // }

    return 0;
}

char *http2utf8_dup(const char *str)
{
    int size = strlen(str);
    char *buf = malloc(size + 1);
    if (buf == NULL)
        return NULL;

    const char *c = str;
    char *b = buf;
    while (*c != 0)
    {
        if (*c != '%')
        {
            *b = *c;
            b++;
            c++;
        }
        else if (*(c + 1) != '%')
        {
            *b = (char)strtol(++c, NULL, 16);
            b++;
            c += 2;
        }
        else
        {
            *b = '%';
            b++;
            c += 2;
        }
    }
    *b = 0;
    return buf;
}

void print_report(struct StatData *pdata)
{
    printf("\n\nCNT = %lu\n", pdata->cnt);

    struct HashtableIterator iter;
    hashtable_iter_init(&iter, pdata->part_cnts);
    printf("Status code:\n");
    while (iter.wi != NULL)
    {
        printf("\"%s\" %d\n", iter.wi->key, iter.wi->counter);
        hashtable_iter_next(&iter);
    }
    
    printf("\nAll objects size: %lu\nStatus errors: %lu\n", pdata->all_objects_size,
           pdata->status_err);
    printf("URL's count: %d\n", hashtable_get_taken(pdata->urls));
    printf("Referers's count: %d\n", hashtable_get_taken(pdata->referers));

    // struct HashtableIterator iter;
    struct WordInfo *max_word;
    char *url_utf8;
    printf("\n\nURL's report:\n==========\n");
    for (int i = 0; i < 10; i++)
    {
        hashtable_iter_init(&iter, pdata->urls);
        max_word = iter.wi;
        while (iter.wi != NULL)
        {
            if(iter.wi->value != NULL)
            {
                if (*((size_t*)iter.wi->value) > *((size_t*)max_word->value))
                {
                    max_word = iter.wi;
                }
            }
            hashtable_iter_next(&iter);
        }
        url_utf8 = http2utf8_dup(max_word->key);
        if (url_utf8 != NULL)
        {
            printf("%10ld  <%s>\n", *((size_t*)max_word->value), url_utf8);
            free(url_utf8);
        }
        *((size_t*)max_word->value) = 0;
    }

    printf("\n\nReferer's report:\n==========\n");
    for (int i = 0; i < 10; i++)
    {
        hashtable_iter_init(&iter, pdata->referers);
        max_word = iter.wi;
        while (iter.wi != NULL)
        {
            if (iter.wi->counter > max_word->counter)
            {
                max_word = iter.wi;
            }
            hashtable_iter_next(&iter);
        }
        url_utf8 = http2utf8_dup(max_word->key);
        if (url_utf8 != NULL)
        {
            printf("%10d  <%s>\n", max_word->counter, url_utf8);
            free(url_utf8);
        }
        max_word->counter = 0;
    }
}


void* file_scan(void *d)
{
    struct FileThreadData *data = (struct FileThreadData*) d;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int line_cnt = 0;

    fp = fopen(data->file_name, "r");
    if (fp == NULL)
    {
        putc('3', stdout);
        fflush(stdout);
        return NULL;
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
        line_parser(data->pdata, line);
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
    return NULL;
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

    struct ThrdList *thrd_list;
    ThrdList_init(&thrd_list);
    
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

        pthread_t pthrd;
        char *full_name_cpy = strdup(full_name);
        struct FileThreadData *thrd_data = malloc(sizeof(struct FileThreadData));
        thrd_data->file_name = full_name_cpy;
        thrd_data->pdata = pdata;
        pthread_create(&pthrd, NULL, file_scan, thrd_data);
        ThrdList_push(&thrd_list, pthrd, thrd_data);
        printf("thrd: 0x%016lx\n", pthrd);
    }

    while (thrd_list != NULL)
    {
        pthread_join(thrd_list->pthrd, NULL);
        printf("joined 0x%016lx\n", ThrdList_pop(&thrd_list));
        fflush(stdout);
    }

    printf("all joined\n");
    fflush(stdout);

    // Вывести отчет
    print_report(pdata);

    delete_stat_data(pdata);
    closedir(dir);

    return retval;
}