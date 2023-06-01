#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/un.h>
#include <netdb.h>

#define UNIX_PATH_MAX 108
int main() {
 	int fd = -1;
 	if((fd = socket(AF_UNIX, SOCK_DGRAM, 0 )) == -1) {
     	perror("Error craeting socket");
 	}

 	struct sockaddr_un addr;
 	addr.sun_family = AF_UNIX;
 	addr.sun_path[0] = '\0';
 	if(bind(fd,(struct sockaddr *)&addr, sizeof(struct sockaddr)) == -1) {
     	perror("Error binding");
 	}

 	char buf[20];
 	while(1){
     	if(read(fd, buf, 20) == -1)
         	perror("Error receiving message");
     	printf("Message from client \"%s\"\n",buf);
     	if(buf[0] == 'e') break;
 	}

 	shutdown(fd, SHUT_RDWR);
 	close(fd);
return 0;
}
