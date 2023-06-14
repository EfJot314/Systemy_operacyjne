#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/epoll.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>


#define MAX_MSG 256

typedef enum {
    msg_ping,
    msg_username_taken,
    msg_server_full,
    msg_disconnect,
    msg_get,
    msg_init,
    msg_list,
    msg_tone,
    msg_tall,
    msg_stop,
} message_type;


typedef struct {
    message_type type;
    char text[MAX_MSG]; 
    char other_nickname[MAX_MSG];
} message;
