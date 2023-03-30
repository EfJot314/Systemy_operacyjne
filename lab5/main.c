#include<stdio.h>
#include<stdlib.h>
#include<string.h>







int main(){

    //tworze pipe1
    int fd1[2];
    pipe(fd1);

    //tworze pipe2
    int fd2[2];
    pipe(fd2);

    //tworze dziecko
    int newPID = fork();

    //jesli dziecko
    if(newPID == 0){
        //zamykam nieuzywany koniec do pisania
        close(fd1[1]);
        //zamykam nieuzywany koniec do czytania
        close(fd2[0]);
        //tworzenie buforu
        char buf[100];
        //czytam do bufru z pipa
        read(fd1[0], buf, 100);
        //drukuje bufor
        printf("%s\n", buf);
        //modyfikuje bufor
        strcat(buf, " world!");
        //wysylam bufor
        write(fd2[1], buf, 100);
    }
    //jesli rodzic
    else{
        //zamykam nieuzywana koncowke do czytania
        close(fd1[0]);
        //zamykam nieuzywany koniec do pisania
        close(fd2[1]);
        //wysylam wiadomosc
        write(fd1[1], "Hello", 100);
        //czytam zwrot
        char buf[100];
        read(fd2[0], buf, 100);
        //printuje
        printf("%s\n", buf);

    }




}