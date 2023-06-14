#include"header.h"


//deskryptor socketa
int sfd;


//handler SIGINT
void handler(int signo) {
  message msg = { .type = msg_disconnect };
  write(sfd, &msg, sizeof msg);
  exit(0);
}



int main(int argc, char** argv) {
    //pobieranie argumentow
    if (strcmp(argv[2], "web") == 0 && argc == 5){
        struct sockaddr_in addr;
        memset(&addr, 0, sizeof(addr));
        addr.sin_family = AF_INET;
        addr.sin_port = htons(atoi(argv[4]));
        if (inet_pton(AF_INET, argv[3], &addr.sin_addr) <= 0) {
            perror("Niepoprawny adres");
            exit(0);
        }
        sfd = socket(AF_INET, SOCK_STREAM, 0);
        connect(sfd, (struct sockaddr*) &addr, sizeof(addr));
    }
    else if (strcmp(argv[2], "unix") == 0 && argc == 4){
        struct sockaddr_un addr;
        memset(&addr, 0, sizeof(addr));
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, argv[3], sizeof(addr.sun_path));

        sfd = socket(AF_UNIX, SOCK_STREAM, 0);
        connect(sfd, (struct sockaddr*) &addr, sizeof(addr));
    }
    else {
        perror("Niepoprawna liczba argumentow!");
        exit(0);
    }
    char* nickname = argv[1];

    //obsluga sygnalow
    signal(SIGINT, handler);

    //wysylam swoj nick do serwera
    write(sfd, nickname, strlen(nickname));
    
    //tworze epolla
    int epoll_fd = epoll_create1(0);
    
    struct epoll_event stdin_event = { 
        .events = EPOLLIN | EPOLLPRI,
        .data = { .fd = STDIN_FILENO }
    };
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, STDIN_FILENO, &stdin_event);

    struct epoll_event fd_event = { 
        .events = EPOLLIN | EPOLLPRI | EPOLLHUP,
        .data = { .fd = sfd }
    };
    epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sfd, &fd_event);

    struct epoll_event events[2];

    //glowna petla
    while(1) {
        //przechodze po eventach
        int nread = epoll_wait(epoll_fd, events, 2, 1);
        for(int i = 0; i < nread; i++) {
            //starnardowe wejscie
            if (events[i].data.fd == STDIN_FILENO) {
                //czytam
                char buffer[512];

                int x = read(STDIN_FILENO, &buffer, 512);
                buffer[x] = 0;

                char cor[] = " \t\n";
                char *token;
                int idx = 0;
                token = strtok( buffer, cor );

                message_type type = -1;
                char other_nickname[MAX_MSG];
                char text[MAX_MSG];

                bool broke = false;

                //parser co wpisalem
                if (token == NULL) continue;
                while( token != NULL ) {
                    
                    switch (idx) {
                        //pierwsze slowo
                        case 0:
                            if(strcmp(token, "LIST") == 0){ 
                                type = msg_list; 
                            }
                            else if(strcmp(token, "2ALL") == 0){ 
                                type = msg_tall;
                                }
                            else if(strcmp(token, "2ONE") == 0){ 
                                type = msg_tone;
                            }
                            else if(strcmp(token, "STOP") == 0){ 
                                type = msg_stop;
                            }
                            else{
                                broke = true;
                                perror("Invalid command");
                                type = -1;
                            }
                            break;
                        //wiadomosc
                        case 1:
                            memcpy(text, token, strlen(token)*sizeof(char));
                            text[strlen(token)] = 0;
                            break;
                        //nickname odbiorcy
                        case 2:
                            memcpy(other_nickname, token, strlen(token)*sizeof(char));
                            other_nickname[strlen(token)] = 0;
                            break;
                        //cos jest zle
                        case 3:
                            broke = true;
                            break;
                    }

                    if (broke)
                        break;

                    //zwiekszam licznik i biore kolejna czesc
                    idx++;
                    token = strtok( NULL, cor );
                }

                if (broke)
                    continue;

                //wysylam odpowiednia wiadomosc
                message msg;
                msg.type = type;
                memcpy(&msg.other_nickname, other_nickname, strlen(other_nickname)+1);
                memcpy(&msg.text, text, strlen(text)+1);

                write(sfd, &msg, sizeof msg);



            } else { 
                //wiadomosci otrzymywane od serwera
                message msg;
                read(sfd, &msg, sizeof msg);

                if (msg.type == msg_username_taken) {
                    perror("This username is already taken\n");
                    close(sfd);
                    exit(0);
                } else if (msg.type == msg_server_full) {
                    perror("Server is full\n");
                    close(sfd);
                    exit(0);
                } else if (events[i].events & EPOLLHUP) {
                    perror("Disconnected\n");
                    exit(0);
                } else if (msg.type == msg_ping) {
                    write(sfd, &msg, sizeof msg);
                } else if (msg.type == msg_stop) {
                    perror("Stopping\n");
                    close(sfd);
                    exit(0);
                } else if (msg.type == msg_get) {
                    perror(msg.text);
                }
            }


        }
    }
}