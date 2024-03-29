
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <syslog.h>
#include <unistd.h>

#include <yaml.h>

#include "cmdline.h"
#include "config.h"
#include "file_info.h"
#include "version.h"

// Sockets
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>

static int running;
static int pid_fd = -1;
static const char *pid_file_name = NULL;
static int sock = -1;

/**
 * @brief Обработчик сигналов
 *
 * @param sig
 */
void handle_signal(int sig)
{
    if ((sig == SIGINT) || (sig == SIGTERM))
    {
        syslog(LOG_INFO, "Debug: stopping daemon ... %d %d %s\n", pid_fd, sock, pid_file_name);
        /* Unlock and close lockfile */
        if (pid_fd != -1)
        {
            lockf(pid_fd, F_ULOCK, 0);
            close(pid_fd);
        }
        /* Try to delete lockfile */
        if (pid_file_name != NULL)
        {
            unlink(pid_file_name);
        }
        running = 0;
        if (sock >= 0)
            close(sock);
        /* Reset signal handling to default behavior */
        signal(SIGINT, SIG_DFL);
    }
    else if (sig == SIGHUP)
    {
        syslog(LOG_INFO, "Debug: reloading daemon config file ...\n");
        // read_conf_file(1);
    }
    else if (sig == SIGCHLD)
    {
        syslog(LOG_INFO, "Debug: received SIGCHLD signal\n");
    }
}

/**
 * @brief Демонизация
 *
 */
static void daemonize()
{
    pid_t pid = 0;
    int fd;

    pid = fork();
    if (pid < 0)
    {
        syslog(LOG_ERR, "Fail fork (1) pid=%d\n", pid);
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }
    if (setsid() < 0)
    {
        syslog(LOG_ERR, "Fail setsid (1) pid=%d\n", pid);
        exit(EXIT_FAILURE);
    }

    /* Ignore signal sent from child to parent process */
    signal(SIGCHLD, SIG_IGN);
    pid = fork();
    if (pid < 0)
    {
        syslog(LOG_ERR, "Fail fork (2) pid=%d\n", pid);
        exit(EXIT_FAILURE);
    }
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    /* Set new file permissions */
    umask(0);
    chdir("/");

    /* Close all open file descriptors */
    for (fd = sysconf(_SC_OPEN_MAX); fd >= 0; fd--)
    {
        close(fd);
    }

    /* Reopen stdin (fd = 0), stdout (fd = 1), stderr (fd = 2) */
    stdin = fopen("/dev/null", "r");
    stdout = fopen("/dev/null", "w+");
    stderr = fopen("/dev/null", "w+");

    /* Try to write PID of daemon to lockfile */
    if (pid_file_name != NULL)
    {
        char str[256];
        pid_fd = open(pid_file_name, O_RDWR | O_CREAT, 0640);
        if (pid_fd < 0)
        {
            /* Can't open lockfile */
            syslog(LOG_ERR, "Can't open lockfile <%s>\n", pid_file_name);
            exit(EXIT_FAILURE);
        }
        if (lockf(pid_fd, F_TLOCK, 0) < 0)
        {
            /* Can't lock file */
            syslog(LOG_WARNING, "fsized is already running\n");
            exit(EXIT_FAILURE);
        }
        /* Get current PID */
        sprintf(str, "%d\n", getpid());
        /* Write PID to lockfile */
        write(pid_fd, str, strlen(str));
    }
}

int main(int argc, char **argv)
{
    int retval = EXIT_SUCCESS;

    struct gengetopt_args_info args_info;

    openlog("fsized", LOG_PID | LOG_CONS, LOG_DAEMON);
    syslog(LOG_INFO, "Started (1) %s\n", argv[0]);

    if (cmdline_parser(argc, argv, &args_info) != 0)
    {
        fprintf(stderr, "cmdline_parser error");
        exit(EXIT_FAILURE);
    }

    /* Read configuration from config file */
    (void)read_conf_file(&args_info);

    printf("PID file name: <%s>\n", args_info.pid_file_arg);
    printf("socket file name: <%s>\n", args_info.unix_socket_name_arg);
    printf("Config: <%s>\n", args_info.config_arg);
    printf("Observed file: <%s>\n", args_info.observed_file_arg);

    pid_file_name = args_info.pid_file_arg;

    if (args_info.daemonize_flag)
    {
        daemonize();
    }

    /* Open system log and write message to it */
    syslog(LOG_INFO, "Started (2) %s\n", argv[0]);

    /* Daemon will handle two signals */
    signal(SIGINT, handle_signal);
    signal(SIGHUP, handle_signal);
    signal(SIGTERM, handle_signal);

    /* This global variable can be changed in function handling signal */
    running = 1;

    int msgsock;
    struct sockaddr_un server;
    char buf[1024];

    sock = socket(AF_UNIX, SOCK_STREAM, 0);
    if (sock < 0)
    {
        // perror("opening stream socket");
        syslog(LOG_ERR, "Can't create socket: %s\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto exit_1;
    }
    server.sun_family = AF_UNIX;
    strcpy(server.sun_path, args_info.unix_socket_name_arg);
    if (bind(sock, (struct sockaddr *)&server, sizeof(struct sockaddr_un)))
    {
        // perror("binding stream socket");
        syslog(LOG_ERR, "Can't bind stream socket: %s\n", strerror(errno));
        retval = EXIT_FAILURE;
        goto exit_1;
    }

    listen(sock, 5);

    while (running == 1)
    {
        msgsock = accept(sock, NULL, NULL);
        if (msgsock == -1)
        {
            // Сюда мы попадаем при штатном выходе.
            // Поэтому сообщения выводить не надо.
            // perror("accept");
            break;
        }
        else
        {
            if (get_fileinfo_str(args_info.observed_file_arg, buf) > 0)
            {
                send(msgsock, buf, strlen(buf), 0);
            }
            else
            {
                sprintf(buf, "Test string");
                send(msgsock, buf, strlen(buf), 0);
            }
        }
        close(msgsock);
    }

exit_1:
    close(sock);
    unlink(args_info.unix_socket_name_arg);

    cmdline_parser_free(&args_info);

    return retval;
}
