#include<stdio.h>
#include<stdlib.h>







int main(){

    int fd[2];

    pipe(fd);

    int newPID = fork();

    //jesli dziecko
    if(newPID == 0){
        char* buf = calloc(100, sizeof(char));
        close(fd[1]);
        read(fd[0], buf, 100);
        printf("%s\n", buf);
        free(buf);
    }
    //jesli rodzic
    else{
        close(fd[0]);
        write(fd[1], "Hello world!", 100);

    }




}