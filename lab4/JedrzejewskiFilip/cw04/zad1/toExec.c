#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>


int main(int argc, char* argv[]){
    //pobieranie informacji o tym czy badany jest pending
    int pending = atoi(argv[1]);

    //jesli badam pending
    if(pending){
        //sprawdzam czy sygnal SIGUSR1 jest oczekujacy
        sigset_t pending_signals;
        sigpending(&pending_signals);
        printf("Potomek: %d\n", sigismember(&pending_signals, SIGUSR1));
    }
    //jesli nie badam pendingu to...
    else{
        //...wysylam do siebie SIGUSR1 po raz drugi by sprawdzic czy potomek dziedziczy po przodku
        raise(SIGUSR1);
    }
        

    return 0;
}