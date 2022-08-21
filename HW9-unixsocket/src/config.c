
#include <fcntl.h>
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
}

struct parser_state
{
    enum state state; /* The current parse state */
    enum Keys key_state;
    char * scan_file_name;
    char * socket_name;
    bool daemon;
};

int consume_event(struct parser_state *s, yaml_event_t *event)
{
    switch (event->type)
    {
    case YAML_STREAM_START_EVENT:
        printf("YAML_STREAM_START_EVENT\n");
        break;
    case YAML_DOCUMENT_START_EVENT:
        printf("YAML_DOCUMENT_START_EVENT\n");
        s->key_state = KEY_NONE;
        break;
    case YAML_STREAM_END_EVENT:
        printf("YAML_STREAM_END_EVENT\n");
        s->state = STATE_STOP; /* All done. */
        s->scan_file_name = NULL;
        s->socket_name = NULL;
        s->daemon = false;
        break;
    case YAML_MAPPING_START_EVENT:
        printf("YAML_MAPPING_START_EVENT\n");
        break;
    case YAML_DOCUMENT_END_EVENT:
        printf("YAML_DOCUMENT_END_EVENT\n");
        break;
    case YAML_SCALAR_EVENT:
        printf("YAML_SCALAR_EVENT %s\n", (char *)event->data.scalar.value);
        switch (s->key_state)
        {
        case KEY_NONE:
        {
            if(strcmp(event->data.scalar.value, "file_name") == 0)
            {
                s->key_state = KEY_SCAN_FILE_NAME;
                break;
            }
            else if(strcmp(event->data.scalar.value, "socket_name") == 0)
            {
                s->key_state = KET_SOCKET_NAME;
                break;
            }
            else if(strcmp(event->data.scalar.value, "daemonize") == 0)
            {
                s->key_state = KEY_DAEMON;
                break;
            }
            break;
        }
        case KEY_SCAN_FILE_NAME:
        

        default:
            break;
        }
        break;
    case YAML_SEQUENCE_START_EVENT:
        printf("YAML_SEQUENCE_START_EVENT\n");
        break;
    case YAML_MAPPING_END_EVENT:
        printf("YAML_MAPPING_END_EVENT\n");
        break;
    case YAML_SEQUENCE_END_EVENT:
        printf("YAML_SEQUENCE_END_EVENT\n");
        break;

    default:
        break;
    }
    return SUCCESS;
}

int read_conf_file(struct gengetopt_args_info *args_info)
{
    int code;
    enum status status;
    struct parser_state state;
    yaml_parser_t parser;
    yaml_event_t event;
    FILE *fd_conf;

    yaml_parser_initialize(&parser);
    fd_conf = fopen(args_info->config_arg, "r");
    yaml_parser_set_input_file(&parser, fd_conf);

    do
    {
        status = yaml_parser_parse(&parser, &event);
        if (status == FAILURE)
        {
            fprintf(stderr, "yaml_parser_parse error\n");
            code = EXIT_FAILURE;
            goto done;
        }
        status = consume_event(&state, &event);
        if (status == FAILURE)
        {
            fprintf(stderr, "consume_event error\n");
            code = EXIT_FAILURE;
            goto done;
        }
        yaml_event_delete(&event);
    } while (state.state != STATE_STOP);

done:
    yaml_parser_delete(&parser);
    return code;
}