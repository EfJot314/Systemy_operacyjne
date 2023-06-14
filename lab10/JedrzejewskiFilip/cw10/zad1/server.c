#include "header.h"
#include <pthread.h>

//aliasik na taka petelke
#define find(initializer, condition) ({ \
  int index = -1;                       \
  for (initializer)                     \
    if (condition) {                    \
      index = i;                        \
      break;                            \
    } index;                            \
})


//maksymalna liczba polazonych klientow
#define MAX_CLIENTS 20

//deskryptor epolla
int epoll_fd;


//strukturka opisujaca klienta
struct client {
  int fd;
  enum client_state { empty = 0, init, ready } state;
  char nickname[16];
  char symbol;
  struct game_state* game_state;
  bool responding;
} clients[MAX_CLIENTS];
typedef struct client client;

//strukturka opisujaca event
typedef struct event_data {
  enum event_type { socket_event, client_event } type;
  union payload { client* client; int socket; } payload;
} event_data;


//usuwanie klienta z serwera
void delete_client(client* client) {
  client->state = empty;
  client->nickname[0] = 0;
  epoll_ctl(epoll_fd, EPOLL_CTL_DEL, client->fd, NULL);
  close(client->fd);
}

//wysylanie wiadomosci do danego klienta, o danym typie i tresci
void send_msg(client* client, message_type type, char text[MAX_MSG]) {
  message msg;
  msg.type = type;
  memcpy(&msg.text, text, MAX_MSG*sizeof(char));
  write(client->fd, &msg, sizeof(msg));
}

//funkcja wywolywana po otrzymaniu wiadomosci od klienta
void on_client_message(client* client) {
  //tworzenie nowego klienta
  if (client->state == init) {
    int nick_size = read(client->fd, client->nickname, sizeof client->nickname - 1);
    int j = client - clients;
    //jezeli nie znaleziono takiego klienta
    if (find(int i = 0; i < MAX_CLIENTS; i++, i != j && strcmp(client->nickname, clients[i].nickname) == 0) == -1) {
      client->nickname[nick_size] = '\0';
      client->state = ready;
    } 
    //jezeli go znaleziono
    else {
      message msg = { .type = msg_username_taken };
      write(client->fd, &msg, sizeof msg);
      strcpy(client->nickname, "new client");
      delete_client(client); 
    }
  }
  //w pozostalych sytuacjach
  else {
    message msg;
    read(client->fd, &msg, sizeof msg);

    //odpowiedz klienta na pingowanie
    if (msg.type == msg_ping) {
      client->responding = true;
    //rozlaczenie klienta
    } else if (msg.type == msg_disconnect || msg.type == msg_stop) {
      delete_client(client);
    //klient pisze do wszystkich
    } else if (msg.type == msg_tall) {
      char out[256] = "";
      strcat(out, client->nickname);
      strcat(out, ": ");
      strcat(out, msg.text);

      for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].state != empty)
          send_msg(clients+i, msg_get, out);
      }
    //klient prosi o liste pozostalych uzytkownikow
    } else if (msg.type == msg_list) {
      for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].state != empty)
          send_msg(client, msg_get, clients[i].nickname);
      }
    //klient wysyla do innego uzytkownika
    } else if (msg.type == msg_tone) {
      char out[256] = "";
      strcat(out, client->nickname);
      strcat(out, ": ");
      strcat(out, msg.text);

      for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].state != empty) {
          if (strcmp(clients[i].nickname, msg.other_nickname) == 0) {
            send_msg(clients+i, msg_get, out);
          }
        }
      }
    } 
  }
}

//inicjacja socketu (bindowanie, listen i epoll)
void init_socket(int socket, void* addr, int addr_size) {
  bind(socket, (struct sockaddr*) addr, addr_size);
  listen(socket, MAX_CLIENTS);
  struct epoll_event event = { .events = EPOLLIN | EPOLLPRI };
  event_data* event_data = event.data.ptr = malloc(sizeof *event_data);
  event_data->type = socket_event;
  event_data->payload.socket = socket;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket, &event);
}

//tworzenie nowego klienta
client* new_client(int client_fd) {
  int i = find(int i = 0; i < MAX_CLIENTS; i++, clients[i].state == empty);
  if (i == -1) return NULL;
  client* client = &clients[i];

  client->fd = client_fd;
  client->state = init;
  client->responding = true;
  return client;
}

//funkcja pingujaca klientow
void* pingFunction() {
  static message msg = { .type = msg_ping };
  while(1) {
    //pinguje co 1 sekunde
    sleep(1);
    //sprawdzam czy wszyscy odpowiadaja na wyslane pingi
    for (int i = 0; i < MAX_CLIENTS; i++) {
      if (clients[i].state != empty) {
        if (clients[i].responding) {
          clients[i].responding = false;
          write(clients[i].fd, &msg, sizeof msg);
        }
        else delete_client(&clients[i]);
      }
    }
  }
}



int main(int argc, char** argv) {
  //zczytywanie danych z wejscia
  if (argc != 3) {
    perror("Niepoprawna liczba argumentow!");
    exit(0);
  }
  int port = atoi(argv[1]);
  char* socket_path = argv[2];

  //epoll
  epoll_fd = epoll_create1(0);


  //lokalny socket
  struct sockaddr_un local_addr = { .sun_family = AF_UNIX };
  strncpy(local_addr.sun_path, socket_path, sizeof local_addr.sun_path);
  unlink(socket_path);
  int local_sock = socket(AF_UNIX, SOCK_STREAM, 0);
  init_socket(local_sock, &local_addr, sizeof local_addr);

  //sieciowy socket
  struct sockaddr_in web_addr = {
    .sin_family = AF_INET, 
    .sin_port = htons(port),
    .sin_addr = { .s_addr = htonl(INADDR_ANY) },
  };
  int web_sock = socket(AF_INET, SOCK_STREAM, 0);
  init_socket(web_sock, &web_addr, sizeof web_addr);


  //watek do pingowania
  pthread_t ping_thread;
  pthread_create(&ping_thread, NULL, pingFunction, NULL);

  //glowna petla
  struct epoll_event events[10];
  while(1) {
    int nread = epoll_wait(epoll_fd, events, 10, -1);
    //przechodze po zdarzeniach
    for(int i=0;i<nread;i++) {
      event_data* data = events[i].data.ptr;
      //nowy klient
      if (data->type == socket_event) {
        int client_fd = accept(data->payload.socket, NULL, NULL);
        client* client = new_client(client_fd);
        //pelny serwer
        if (client == NULL) {
          message msg = { .type = msg_server_full };
          write(client_fd, &msg, sizeof msg);
          close(client_fd); 
          continue;
        }

        //epolle
        event_data* event_data = malloc(sizeof(event_data));
        event_data->type = client_event;
        event_data->payload.client = client;
        struct epoll_event event = { .events = EPOLLIN | EPOLLET | EPOLLHUP, .data = { event_data } };
        epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &event);
      
      //event zwiazany z klientem
      } else if (data->type == client_event) {
        if (events[i].events & EPOLLHUP) {
          delete_client(data->payload.client);
        }
        //w przeciwnym przypadku to wiadomosc od klienta, wiec odbieram
        else on_client_message(data->payload.client);
      }
    }
  }
}