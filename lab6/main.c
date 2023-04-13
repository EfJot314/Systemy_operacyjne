#include<stdio.h>
#include <unistd.h>
#include <limits.h>
#include "sys/msg.h"







int main(){
    //tworze klucz
    int key = ftok(".", 'F');

    int res = msgget(key, IPC_CREAT | 0666);
    if(res == -1){
        perror("Blad podczas tworzenia kolejki!");
        exit(1);
    }


    return 0;
}