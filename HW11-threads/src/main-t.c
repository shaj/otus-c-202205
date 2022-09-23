
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>

void* foo(void *arg)
{
    (void)arg;
    sleep(1);
    printf("thread #%ld\n", pthread_self());
    return (void*)0;
}

#define THRD_CNT 5

int main()
{
    pthread_t pool[THRD_CNT];

    for(int i=0; i < THRD_CNT; i++)
    {
        pthread_create(&pool[i], NULL, foo, NULL);
    }
    for(int i=0; i < THRD_CNT; i++)
    {
        pthread_join(pool[i], NULL);
    }

    return 0;
}
