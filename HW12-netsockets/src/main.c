
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


int prt_send(int sock_fd, const char *buf, int len)
{
    int retval = 0;
    if ((retval = send(sock_fd, buf, len, 0)) < 0)
    {
        perror("send");
    }
    return !(retval < 0);
}

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

    if (len == size)
    {
        buffer[size - 1] = '\0';
    }
    else
    {
        buffer[len] = '\0';
    }
    return retval;
}

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

int main(int argc, char *argv[])
{
    int retval = EXIT_SUCCESS;
    int message_size;
    char *message_string;
    char *message_p;

    struct gengetopt_args_info args_info;
    if (cmdline_parser(argc, argv, &args_info) != 0)
    {
        exit(1);
    }

    puts("Test string parts:");
    message_size = 0;
    for (unsigned int i = 0; i < args_info.inputs_num; ++i)
    {
        message_size += strlen(args_info.inputs[i]) + 1;
    }
    message_string = malloc(message_size + 1);
    message_p = message_string;
    for (unsigned int i = 0; i < args_info.inputs_num; ++i)
    {
        strcpy(message_p, args_info.inputs[i]);
        message_p += strlen(args_info.inputs[i]);
        *message_p = ' ';
        message_p++;
    }
    printf("Message: <%s>\n", message_string);
    printf("all_flags %d\nfont_name <%d>\n", args_info.all_fonts_flag, args_info.font_name_given);

    struct addrinfo hints;
    struct addrinfo *addr_info, *rp;
    int sock_fd;
    char buffer[BUF_SIZE];
    int recv_count;
    char cmd_buf[256];

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;     /* For wildcard IP address */
    hints.ai_protocol = IPPROTO_TCP; /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int ret = getaddrinfo(SERVICE_NAME, "telnet", &hints, &addr_info);
    if (ret != 0)
    {
        fprintf(stderr, "get addr info error: %d %s\n", ret, gai_strerror(ret));
        exit(EXIT_FAILURE);
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
        return EXIT_FAILURE;
    }
    puts("Connected");

    // Отсюда:
    // https://www.rfc-editor.org/rfc/rfc1073
    prt_send(sock_fd, "\xff\xfb\x1f", 3); // "\255\251\31"
    prt_send(sock_fd, "\xff\xfa\x1f\x00\x50\x00\x64\xff\xf0", 9); // "\255\250\31\0\80\0\100\255\240"
    recv_count = prt_recv(sock_fd, buffer, BUF_SIZE);

    if ((args_info.print_fonts_flag) || (strlen(message_string) == 0))
    {
        prt_send(sock_fd, "figlet\r\n", 8);
        recv_count = prt_recv(sock_fd, buffer, BUF_SIZE);
        puts(buffer);
    }
    else if (args_info.all_fonts_flag)
    {
        prt_send(sock_fd, "figlet\r\n", 8);
        prt_recv(sock_fd, buffer, BUF_SIZE);
        char **fonts = get_fonts(buffer, BUF_SIZE);
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
        for(unsigned int i = 0; i < args_info.font_name_given; i++)
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

    freeaddrinfo(addr_info);

    free(message_string);

    cmdline_parser_free (&args_info);

    return retval;
}