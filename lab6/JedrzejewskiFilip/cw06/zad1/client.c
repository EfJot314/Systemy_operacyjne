#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<sys/msg.h>
#include<stdlib.h>

#include"constData.h"




int main(){
    //tworze klucz
    int key = ftok("$HOME", SERVERCHAR);


    //kolejka serwera
    int msg = msgget(key, 0666);
    if(msg == -1){
        perror("Blad podczas otwierania kolejki!");
        exit(1);
    }


    //kolejka klienta
    int myKey = ftok("$HOME", getpid());
    int myMsg = msgget(myKey, IPC_CREAT | 0666);


    


    //usuwam kolejke
    msgctl(myKey, IPC_RMID, NULL);




    return 0;
}