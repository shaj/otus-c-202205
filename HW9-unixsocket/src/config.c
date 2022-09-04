
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <yaml.h>

#include "config.h"

enum status
{
    SUCCESS = 1,
    FAILURE = 0
};

enum state
{
    STATE_START, /* start state */
    STATE_STOP   /* end state */
};

enum Keys
{
    KEY_NONE,
    KEY_SCAN_FILE_NAME,
    KET_SOCKET_NAME,
    KEY_DAEMON
};

struct parser_state
{
    enum state state; /* The current parse state */
    enum Keys key_state;
    bool observed_file_given;
    char *observed_file_name;
    bool socket_name_given;
    char *socket_name;
    bool daemon_given;
    bool daemon;
};

int consume_event(struct parser_state *s, yaml_event_t *event)
{
    switch (event->type)
    {
    case YAML_DOCUMENT_START_EVENT:
        s->key_state = KEY_NONE;
        break;
    case YAML_STREAM_END_EVENT:
        s->state = STATE_STOP; /* All done. */
        break;
    case YAML_SCALAR_EVENT:
        switch (s->key_state)
        {
        case KEY_NONE:
        {
            if (strcmp((char *)event->data.scalar.value, "file_name") == 0)
            {
                s->key_state = KEY_SCAN_FILE_NAME;
                break;
            }
            else if (strcmp((char *)event->data.scalar.value, "socket_name") == 0)
            {
                s->key_state = KET_SOCKET_NAME;
                break;
            }
            else if (strcmp((char *)event->data.scalar.value, "daemonize") == 0)
            {
                s->key_state = KEY_DAEMON;
                break;
            }
            break;
        }
        case KEY_SCAN_FILE_NAME:
            s->observed_file_name = strdup((char *)event->data.scalar.value);
            s->observed_file_given = true;
            s->key_state = KEY_NONE;
            printf("%s\n", s->observed_file_name);
            break;
        case KET_SOCKET_NAME:
            s->socket_name = strdup((char *)event->data.scalar.value);
            s->socket_name_given = true;
            s->key_state = KEY_NONE;
            printf("%s\n", s->socket_name);
            break;
        case KEY_DAEMON:
        {
            if (strcmp((char *)event->data.scalar.value, "true") == 0)
                s->daemon = true;
            else
                s->daemon = false;
            s->daemon_given = true;
            s->key_state = KEY_NONE;
            break;
        }

        default:
            break;
        }
        break;
    default:
        break;
    }
    return SUCCESS;
}

int read_conf_file(struct gengetopt_args_info *args_info)
{
    int code = EXIT_SUCCESS;
    struct parser_state state;
    yaml_parser_t parser;
    yaml_event_t event;
    FILE *fd_conf;

    fd_conf = fopen(args_info->config_arg, "r");
    if (fd_conf == NULL)
    {
        perror("Can't open config file");
        return EXIT_FAILURE;
    }

    yaml_parser_initialize(&parser);
    yaml_parser_set_input_file(&parser, fd_conf);

    state.state = STATE_START;
    state.key_state = KEY_NONE;
    state.observed_file_given = false;
    state.observed_file_name = NULL;
    state.socket_name_given = false;
    state.socket_name = NULL;
    state.daemon_given = false;
    state.daemon = false;

    do
    {
        if (FAILURE == yaml_parser_parse(&parser, &event))
        {
            fprintf(stderr, "yaml_parser_parse error\n");
            code = EXIT_FAILURE;
            goto done;
        }
        if (FAILURE == consume_event(&state, &event))
        {
            fprintf(stderr, "consume_event error\n");
            code = EXIT_FAILURE;
            goto done;
        }
        yaml_event_delete(&event);
    } while (state.state != STATE_STOP);

    if (!args_info->unix_socket_name_given && state.socket_name_given)
    {
        if (args_info->unix_socket_name_arg != NULL)
            free(args_info->unix_socket_name_arg);
        args_info->unix_socket_name_arg = strdup(state.socket_name);
    }

    if (!args_info->observed_file_given && state.observed_file_given)
    {
        if (args_info->observed_file_arg != NULL)
            free(args_info->observed_file_arg);
        args_info->observed_file_arg = strdup(state.observed_file_name);
    }

    if (!args_info->daemonize_given && state.daemon_given)
    {
        args_info->daemonize_flag = (int)state.daemon;
    }

done:
    fclose(fd_conf);

    if (state.observed_file_name != NULL)
        free(state.observed_file_name);
    if (state.socket_name != NULL)
        free(state.socket_name);

    yaml_parser_delete(&parser);
    return code;
}