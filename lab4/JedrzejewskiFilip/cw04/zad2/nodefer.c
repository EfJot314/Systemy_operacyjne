#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>


void handle(int sig_no){
    //print z handlera
    printf("Wiadomosc z handlera!\n");
    //przedluzam czas dzialania handlera
    sleep(1);
}


int main(){

    printf("\nProgram testujacy flage SA_NODEFER wysylajac sygnal SIGUSR1\n");
    printf("Flaga umozliwia odebranie sygnalu podczas handlowania poprzedniego sygnalu\n");
    printf("Sygnaly sa wykonywane po kolei\n");
    printf("Program wysyla sygnal SIGUSR1 100 razy, a handler wyspisuje informacje ze otrzymal sygal i spi 1s\n");
    printf("Po wyslaniu 100 sygnalow rodzic wysyla wiadomosc ze wyslal wszytsie sygnaly, a dziecko wtedy dalej produkuje zalegle napisy \n\n");

    sleep(1);


    //ustawiam obsluge sygnalu SIGUSR1
    struct sigaction act; 
    act.sa_handler = handle;
    sigemptyset(&act.sa_mask); 
    act.sa_flags = SA_NODEFER;
    sigaction(SIGUSR1, &act, NULL);

    int newPID = fork();
    if(newPID == 0){
        while(1){

        }
    }
    else{
        //wysylam sygnaly
        for(int i=0;i<100;i++){
            kill(newPID, SIGUSR1);
        }

        printf("Wiadomosc po wyslaniu wszytskich sygnalow.\n");

        //chwile czekam na dziecko
        sleep(5);
        //po czym je zabijam
        kill(newPID, SIGINT);
    }
    
    

    
    return 0;
}