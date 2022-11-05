# Простой HTTP сервер

## ?

## otus-io-uring

[Демозанятие](https://www.youtube.com/watch?v=g42hzMplhkQ)

[Пример на io-uring](https://gitlab.com/otusteam/otus-io-uring/-/tree/master/)

### Библиотеки-обёртки

* [libevent](https://libevent.org): supports `/dev/poll`, `kqueue(2)`, POSIX `select(2)`, Windows `select()`, `poll(2)`, and `epoll(4)`. Libevent includes support for several useful protocols, including DNS, HTTP, and a minimal RPC framework.
* [libuv](https://libuv.org): primarily developed for use by Node.js, but it’s also used by Luvit, Julia, pyuv, and others.
  * Full-featured event loop backed by epoll, kqueue, IOCP
  * Asynchronous DNS resolution
  * File system events
* [libev](http://software.schmorp.de/pkg/libev.html): used in GNU Virtual Private Ethernet, rxvt-unicode, auditd, the Deliantra MORPG Server and Client, and many other programs.
  * [Redis, hiredis, libev и multithread](https://habr.com/ru/post/131916)
* [libhv](https://github.com/ithewei/libhv)
* [GNU adns](https://gnu.org/software/adns)

## Старый пример

[aztec rikbtnd.c](https://github.com/ShajTester/rikor-aztec/blob/master/meta-facebook/meta-yosemite/recipes-yosemite/rikgpio-util/files/rikbtnd/rikbtnd.c)