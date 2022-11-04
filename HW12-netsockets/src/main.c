
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "cmdline.h"
#include "version.h"

#define BUF_SIZE (1024 * 10)
#define SERVICE_NAME "telehack.com"

/**
 * @brief Передача команды серверу
 *
 * Если передача не удалась, в stderr выводится сообщение
 * об ошибке.
 *
 * @param sock_fd - сокет, куда посылать
 * @param buf     - буфер, который посылать
 * @param len     - размер данных в буфере
 * @return int  0 - передача удалась
 *             !0 - передача не удалась
 */
int prt_send(int sock_fd, const char *buf, int len)
{
    int retval = 0;
    if ((retval = send(sock_fd, buf, len, 0)) < 0)
    {
        perror("send");
    }
    return !(retval < 0);
}

/**
 * @brief Прием сообщения от сервера
 *
 * При ошибке приема в stderr выводится сообщение об ошибке.
 *
 * @param sock_fd - сокет, откуда принимать
 * @param buffer  - указатель на выделенную память, куда принимать
 * @param size    - размер буфера
 * @return int    - количество принятых байт.
 */
int prt_recv(int sock_fd, char *buffer, int size)
{
    ssize_t len = 0;
    ssize_t r = 0;
    int retval = 0;
    while ((r = recv(sock_fd, &buffer[len], size - len, 0)) > 0)
    {
        // printf("%ld\n", r);
        // for (int i = len; i < (r + len); i++)
        // {
        //     if (isprint(buffer[i]) || buffer[i] == '\n')
        //     {
        //         putc(buffer[i], stdout);
        //     }
        //     else
        //     {
        //         printf("\\x%02x", (unsigned char)buffer[i]);
        //     }
        // }
        // fflush(stdout);
        len += r;
        if (strncmp("\r\n.", &buffer[len - 3], 3) == 0)
            break;
    }

    if (r < 0)
    {
        perror("recv");
        retval = r;
    }
    else
    {
        retval = len;
    }

    // if (len == size)
    // {
    //     buffer[size - 1] = '\0';
    // }
    // else
    // {
    //     buffer[len] = '\0';
    // }
    return retval;
}

/**
 * @brief Печать результата выполнения команды figlet
 *
 * TODO: Удалить лишние строки из ответа
 *
 * @param buffer - буфер с сообщением
 * @param len    - размер сообщения в буфере
 */
void print_figlet(const char *buffer, int len)
{
    for (const char *c = buffer; c < (buffer + len); c++)
    {
        if (isprint(*c))
        {
            putc(*c, stdout);
        }
        else if (*c == '\n')
        {
            putc('\n', stdout);
        }
    }
    putc('\n', stdout);
}

/**
 * @brief Получение списка шрифтов из ответа сервера на команду figlet.
 *
 * После использования полученный массив необходимо удалить
 * с помощью функции clear_fonts.
 *
 * @param buf     - буфер с ответом от сервера
 * @param size    - размер ответа в буфере
 * @return char** - массив строк с названиями шрифтов
 */
char **get_fonts(const char *buf, int size)
{
    char **fonts = calloc(1024, sizeof(char *));
    if (fonts == NULL)
        return NULL;
    char **font = fonts;
    const char *b, *e;
    const char *fb;
    b = buf;
    while (strncmp(b, "figlet fonts:", 13) != 0)
    {
        e = strchr(b, '\r');
        if (e == NULL)
        {
            return NULL;
        }
        b = e + 2;
    }
    b += 15; // strlen("figlet fonts:") + "\r\n"
    while ((*b != '.') && (b < (buf + size)))
    {
        while ((strncmp(b, "\r\n", 2) != 0) && (b < (buf + size)))
        {
            while (*b == ' ')
            {
                b++;
            }
            if (*b == '\r')
            {
                break;
            }
            fb = b;
            while (((*b != ' ') && (*b != '\r')) && (b < (buf + size)))
            {
                b++;
            }
            *font = strndup(fb, b - fb);
            font++;
        }
        b += 2;
    }
    font = NULL;
    return fonts;
}

/**
 * @brief Удаление списка шрифтов, полученного функцией get_fonts
 *
 * @param fonts - список шрифтов
 */
void clear_fonts(char **fonts)
{
    char **font = fonts;
    while (*font != NULL)
    {
        free(*font);
        font++;
    }
    free(fonts);
}

/**
 * @brief Создание строки сообщения из параметров запуска
 *
 * Строка должна быть удалена после использования.
 *
 * @param args_info
 * @return char*
 */
char *create_message(const struct gengetopt_args_info *args_info)
{
    char *message_string;
    char *message_p;
    int message_size = 0;
    for (unsigned int i = 0; i < args_info->inputs_num; ++i)
    {
        message_size += strlen(args_info->inputs[i]) + 1;
    }
    if(message_size == 0)
    {
        return NULL;
    }

    message_string = malloc(message_size + 1);
    if (message_string == NULL)
    {
        return NULL;
    }

    message_p = message_string;
    for (unsigned int i = 0; i < args_info->inputs_num; ++i)
    {
        strcpy(message_p, args_info->inputs[i]);
        message_p += strlen(args_info->inputs[i]);
        *message_p = ' ';
        message_p++;
    }
    return message_string;
}

/**
 * @brief Подключение к "telehack.com"
 * 
 * @return int - sock_fd. Отрицательное значение - ошибка.
 */
int connect_to_server()
{
    struct addrinfo hints;
    struct addrinfo *addr_info, *rp;
    int sock_fd;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int ret = getaddrinfo(SERVICE_NAME, "telnet", &hints, &addr_info);
    if (ret != 0)
    {
        fprintf(stderr, "get addr info error: %d %s\n", ret, gai_strerror(ret));
        return -1;
    }

    for (rp = addr_info; rp != NULL; rp = rp->ai_next)
    {
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(((struct sockaddr_in *)rp->ai_addr)->sin_addr), ip_str,
                  INET_ADDRSTRLEN);
        printf("%s %s port: %d\n", SERVICE_NAME, ip_str,
               ntohs(((struct sockaddr_in *)rp->ai_addr)->sin_port));
        sock_fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sock_fd > 0)
            break;
    }

    if (connect(sock_fd, rp->ai_addr, rp->ai_addrlen) < 0)
    {
        perror("connect");
        close(sock_fd);
        freeaddrinfo(addr_info);
        return -2;
    }

    freeaddrinfo(addr_info);
    return sock_fd;
}

/**
 * @brief main
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[])
{
    int retval = EXIT_SUCCESS;
    char *message_string;

    struct gengetopt_args_info args_info;
    if (cmdline_parser(argc, argv, &args_info) != 0)
    {
        return EXIT_FAILURE;
    }

    message_string = create_message(&args_info);
    printf("Message: <%s>\n", message_string);
    // printf("all_flags %d\nfont_name <%d>\n", args_info.all_fonts_flag,
    // args_info.font_name_given);

    if (message_string == NULL)
    {
        fprintf(stderr, "Message string must be specified\n\n");
        cmdline_parser_print_help();
        return EXIT_FAILURE;
    }

    int sock_fd = connect_to_server();
    if(sock_fd <= 0)
    {
        return EXIT_FAILURE;
    }
    puts("Connected");

    char buffer[BUF_SIZE];
    int recv_count;
    char cmd_buf[256];

    // Отсюда:
    // https://www.rfc-editor.org/rfc/rfc1073
    // Нужно, чтобы список шрифтов не разбивался.
    // TODO: ширину брать из текущего терминала.
    prt_send(sock_fd, "\xff\xfb\x1f", 3); // "\255\251\31"
    prt_send(sock_fd, "\xff\xfa\x1f\x00\x50\x00\x64\xff\xf0", 9); // "\255\250\31\0\80\0\100\255\240"
    recv_count = prt_recv(sock_fd, buffer, BUF_SIZE);

    if ((args_info.print_fonts_flag) || (strlen(message_string) == 0))
    {
        prt_send(sock_fd, "figlet\r\n", 8);
        recv_count = prt_recv(sock_fd, buffer, BUF_SIZE);
        if(recv_count == BUF_SIZE)
        {
            buffer[BUF_SIZE] = '\0';
        }
        else
        {
            buffer[recv_count] = '\0';
        }
        puts(buffer);
    }
    else if (args_info.all_fonts_flag)
    {
        prt_send(sock_fd, "figlet\r\n", 8);
        recv_count = prt_recv(sock_fd, buffer, BUF_SIZE);
        char **fonts = get_fonts(buffer, recv_count);
        char **font = fonts;
        int cnt = 0;
        while ((*font != NULL) && (cnt < 5))
        {
            printf("Font: %s\n", *font);
            snprintf(cmd_buf, 256, "figlet /%s %s\r\n", *font, message_string);
            prt_send(sock_fd, cmd_buf, strlen(cmd_buf));
            recv_count = prt_recv(sock_fd, buffer, BUF_SIZE);
            print_figlet(buffer, recv_count);
            font++;
            cnt++;
        }
        clear_fonts(fonts);
    }
    else if (args_info.font_name_given != 0)
    {
        for (unsigned int i = 0; i < args_info.font_name_given; i++)
        {
            printf("Font: %s\n", args_info.font_name_arg[i]);
            snprintf(cmd_buf, 256, "figlet /%s %s\r\n", args_info.font_name_arg[i], message_string);
            prt_send(sock_fd, cmd_buf, strlen(cmd_buf));
            recv_count = prt_recv(sock_fd, buffer, BUF_SIZE);
            print_figlet(buffer, recv_count);
        }
    }
    else
    {
        snprintf(cmd_buf, 256, "figlet %s\r\n", message_string);
        prt_send(sock_fd, cmd_buf, strlen(cmd_buf));
        recv_count = prt_recv(sock_fd, buffer, BUF_SIZE);
        print_figlet(buffer, recv_count);
    }

    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);

    free(message_string);

    cmdline_parser_free(&args_info);

    return retval;
}