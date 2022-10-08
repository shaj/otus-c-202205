
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <pthread.h>


struct Message
{
    size_t sz;
    char *line;
};

static struct Message msg;
static pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

static pthread_cond_t revready = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t revlock = PTHREAD_MUTEX_INITIALIZER;

void* foo()
{
    size_t sl;
    for(;;)
    {
        pthread_mutex_lock(&qlock);
        while(msg.sz == 0)
            pthread_cond_wait(&qready, &qlock);
        sl = msg.sz;
        if(sl < 20)
            msg.sz = 0;
        pthread_mutex_unlock(&qlock);

        pthread_mutex_lock(&revlock);
        pthread_cond_signal(&revready);
        pthread_mutex_unlock(&revlock);

        if(sl >= 20)
            break;
        printf("thread #%ld : %ld START\n", pthread_self(), sl);
        sleep(sl);
        printf("thread #%ld : %ld complete\n", pthread_self(), sl);
    }
    return (void*)0;
}

#define THRD_CNT 5

int main()
{
    pthread_t pool[THRD_CNT];
    int ptargs[THRD_CNT];

    msg.sz = 0;

    for(int i=0; i < THRD_CNT; i++)
    {
        ptargs[i] = THRD_CNT - i;
        pthread_create(&pool[i], NULL, foo, &ptargs[i]);
    }
    for (size_t i = 0; i < 20; i++)
    {
        printf("main %ld start\n", i);
        pthread_mutex_lock(&qlock);
        msg.sz = i % 4 + 1;
        pthread_cond_signal(&qready);
        pthread_mutex_unlock(&qlock);

        pthread_mutex_lock(&revlock);
        pthread_cond_wait(&revready, &revlock);
        pthread_mutex_unlock(&revlock);

    }
    
    printf("main stopping...\n");
    pthread_mutex_lock(&qlock);
    msg.sz = 21;
    pthread_cond_broadcast(&qready);
    pthread_mutex_unlock(&qlock);

    for(int i=0; i < THRD_CNT; i++)
    {
        pthread_join(pool[i], NULL);
    }

    return 0;
}
