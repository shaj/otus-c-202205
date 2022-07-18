
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char *empty_str = "";
static const char *int_format = "%ld ";
static const long long data[] = {4, 8, 15, 16, 23, 42};


struct Item
{
    long long data;
    struct Item *next;
};

void print_int(long long i)
{
    printf(int_format, i);
    fflush(stdout);
}

long long p(long long i)
{
    return i & 1;
}

struct Item * add_element(long long i, struct Item *list)
{
    struct Item *new_item = (struct Item *) malloc(sizeof(struct Item));
    if(new_item == NULL)
        exit(EXIT_FAILURE);
    new_item->data = i;
    new_item->next = list;
    return new_item;
}

void m(struct Item *list, void (*func)(long long))
{
    if(list == NULL)
        return;
    func(list->data);
    m(list->next, func);
}

struct Item * f(struct Item *list, long long (*func)(long long), struct Item *new_list)
{
    struct Item *retval;
    if(new_list == NULL)
        retval = list;
    else
        retval = new_list;
    if(list == NULL)
        return retval;

    if(func(list->data) != 0)
    {
        retval = add_element(list->data, retval);
    }
    return f(list->next, func, retval);
}

void free_list(struct Item *list)
{
    if(list->next != NULL)
    {
        free_list(list->next);
    }
    free(list);
}

int main()
{
    struct Item *list = NULL;
    for (int i=(sizeof(data)/sizeof(data[0]))-1; i>=0; --i)
    {
        list = add_element(data[i], list);
    }

    m(list, print_int);
    puts(empty_str);
    list = f(list, p, NULL);
    m(list, print_int);
    puts(empty_str);

    free_list(list);

    return 0;
}
