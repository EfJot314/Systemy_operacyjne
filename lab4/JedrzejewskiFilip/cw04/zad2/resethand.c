#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>



void handle(int sig_no){
    printf("Wiadomosc z handlera!\n");
}


int main(){

    //komentarz
    printf("\nProgram testujacy flage SA_RESETHAND wysylajac sygnal SIGUSR1\n");
    printf("Flaga resetuje handler za kazdym razem gdy zostanie odebrany sygnal\n");
    printf("Program ustawia handler, i wysyla dwa sygnaly w odstepie 1s \n\n");

    sleep(1);


    //ustawiam obsluge sygnalu SIGUSR1
    struct sigaction act; 
    act.sa_handler = handle;
    sigemptyset(&act.sa_mask); 
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);

    int newPID = fork();

    //dziecko
    if(newPID == 0){
        //czekam na sygnaly
        while(1){

        }
   
    }
    //rodzic
    else{
        //wysylam sygnal
        kill(newPID, SIGUSR1);

        //opoznienie dla pewnosci
        sleep(1);

        //wysylam drugi sygnal
        kill(newPID, SIGUSR1);

        //zabijam dziecko
        kill(newPID, SIGINT);


        //czekam chwile na dziecko, jak przyjdzie to dobrze, jak nie to tez dobrze
        sleep(1);
        
    }

    return 0;
}