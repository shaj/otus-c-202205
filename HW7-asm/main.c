
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static const char *empty_str = "";
static const char *int_format = "%ld ";
static const int data[] = {4, 8, 15, 16, 23, 42};


struct Item
{
    int data;
    struct Item *next;
};

void print_int(int i)
{
    printf(int_format, i);
    fflush(stdout);
}

int p(int i)
{
    return i & 1;
}

struct Item * add_element(int i, struct Item *list)
{
    struct Item *new_item = (struct Item *) malloc(sizeof(struct Item));
    if(new_item == NULL)
        exit(EXIT_FAILURE);
    new_item->data = i;
    new_item->next = list;
    return new_item;
}

void m(struct Item *list, void (*func)(int))
{
    if(list == NULL)
        return;
    func(list->data);
    m(list->next, func);
}

struct Item * f(struct Item *list, int (*func)(int), struct Item *new_list)
{
    struct Item *retval = new_list;
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
    struct Item *filtered;
    for (int i=(sizeof(data)/sizeof(data[0]))-1; i>=0; --i)
    {
        list = add_element(data[i], list);
    }

    m(list, print_int);
    puts(empty_str);
    filtered = f(list, p, NULL);
    m(filtered, print_int);
    puts(empty_str);

    free_list(list);
    free_list(filtered);

    return 0;
}
