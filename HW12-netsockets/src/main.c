
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>

#include "version.h"

void print_usage(const char *prog_name) { printf("Usage: %s <input file>\n", prog_name); }

void print_version(const char *prog_name)
{
    printf("%s %d.%d.%d\n", prog_name, PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR,
           PROJECT_VERSION_PATCH);
}

int main(int argc, char const *argv[])
{
    int retval = EXIT_SUCCESS;
    // int fd;
    // struct stat sb;
    // char *addr = NULL;
    // uint32_t crc;

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
        exit(EXIT_FAILURE);
    }

    struct addrinfo hints;
    struct addrinfo *addr_info, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_STREAM; /* Datagram socket */
    hints.ai_flags = AI_PASSIVE;     /* For wildcard IP address */
    hints.ai_protocol = 0;           /* Any protocol */
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int ret = getaddrinfo(argv[1], NULL, &hints, &addr_info);
    if (ret != 0)
    {
        fprintf(stderr, "get addr info error: %d %s\n", ret, gai_strerror(ret));
        exit(EXIT_FAILURE);
    }

    for (rp = addr_info; rp != NULL; rp = rp->ai_next)
    {
        // sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        // if (sfd == -1)
        //     continue;

        // if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
        //     break; /* Success */

        // close(sfd);
        char ip_str[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(((struct sockaddr_in *)rp->ai_addr)->sin_addr), ip_str,
                  INET_ADDRSTRLEN);
        printf("%p,   %u,   %s\n", rp, ((struct sockaddr_in *)rp->ai_addr)->sin_addr.s_addr,
               ip_str);
    }

    if (connect(sock_fd, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0)
    {
        perror("connect");
        close(sock_fd);
        return EXIT_FAILURE;
    }
    if (send(sock_fd, argv[2], strlen(argv[2]), 0) < 0)
    {
        perror("send");
        close(sock_fd);
        return EXIT_FAILURE;
    }
    int len = 0;
    while ((r = recv(sock_fd, &buffer[len], BUF_SIZE - len, 0)) > 0)
    {
        len += r;
    }
    if (r < 0)
    {
        perror("recv");
        close(sock_fd);
        return EXIT_FAILURE;
    }
    puts(buffer);
    shutdown(sock_fd, SHUT_RDWR);
    close(sock_fd);

    freeaddrinfo(addr_info);

    return retval;
}