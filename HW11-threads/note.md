# Notes

> Чтение всех файлов в 1 поток: **cpu 1:33,92 total**

## Задание

1. Общее количество отданных байт
2. 10 наиболее тяжелых URL-ов
    - `map<string, int>`
    - Из request извлечь URL
3. 10 наиболее частых рефереров
    - `map<string, int>`

**Разбор строки:** ' ', ' ', ' ', '] "', `request`'" ', `status code`' ', `отдано байт`' "', `referer`'" "', `user-agent`'EOL' 

**request:** ' ', `URL`'?| '

https://github.com/jekstrand/rb-tree


## Версия без потоков

./logscan 1 logs  50,31s user 8,51s system 48% cpu 2:01,37 total
./logscan 1 logs  48,49s user 8,93s system 56% cpu 1:40,82 total
./logscan 1 logs  49,34s user 9,17s system 53% cpu 1:48,58 total
./logscan 1 logs  48,03s user 9,54s system 50% cpu 1:53,68 total
./logscan 1 logs  49,12s user 10,61s system 46% cpu 2:09,32 total

```
CNT = 20843482
Parts: 200 404 302 204 400 304 500 301 405 503 499 422 403 000 408 
All objects size: 139603179994
Status errors: 0
URL's count: 47730
Referers's count: 107541


URL's report:
==========
1258532  </random>
219925  </>
164097  </best/>
57482  </стоят-пассажиры-в-аэропорту-посадочный-досмотр/>
50527  </like/1>
46103  </tag/мужик/>
45023  </new/>
43326  </rss/>
28771  </да-алё-да-да-ну-как-там-с-деньгами/>
26944  </tag/секс/>


Referer's report:
==========
4672038  <->
600019  <https://www.google.com/>
207404  <https://yandex.ru/>
123171  <https://www.google.ru/>
97354  <https://baneks.site/best/>
50127  <https://baneks.site/new/>
42120  <http://yandex.ru/searchapp?text=>
37415  <https://baneks.site/>
32471  <https://baneks.site/best/?p=2>
24728  <https://www.google.com.ua/>
```

Доброго дня. Разбираюсь сейчас с многопоточностью. У нас в 22 лекции был такой
пример с condition variable:

```c
#include <pthread.h>

struct msg
{
    struct msg *m_next;
    /* ... другие поля структуры ... */
};

struct msg *workq;
pthread_cond_t qready = PTHREAD_COND_INITIALIZER;
pthread_mutex_t qlock = PTHREAD_MUTEX_INITIALIZER;

void process_msg()
{
    struct msg *mp;
    for (;;)
    {
        pthread_mutex_lock(&qlock);
        while (workq == NULL)
            pthread_cond_wait(&qready, &qlock);
        mp = workq;
        workq = mp->m_next;
        pthread_mutex_unlock(&qlock);
        /* обработка сообщения mp */
    }
}

void enqueue_msg(struct msg *mp)
{
    pthread_mutex_lock(&qlock);
    mp->m_next = workq;
    workq = mp;
    pthread_mutex_unlock(&qlock);
    pthread_cond_signal(&qready);
}
```

Вопрос - как организовать ожидание, если я хочу ограничить длину
очереди `workq`? То есть перед тем как добавлять новое сообщение,
я хочу подождать, пока хотя бы одно сообщение обработается.