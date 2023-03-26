#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>


void handler(int signum){
    printf("Otrzymano sygnal!\n");
}


int main(int argc, char* argv[]){

    printf("\nBez execvp\n");

    //zmienna przechowujaca czy badam pending
    int pending = 0;

    //sprawdzam czy user podal argument
    if(argc == 2){
        //sprawdzam co napisal user
        if(strcmp(argv[1], "ignore") == 0){
            //ignoruje sygnal
            printf("Ignorowanie sygnalu SIGUSR1\n");
            signal(SIGUSR1, SIG_IGN);
        }
        else if(strcmp(argv[1], "handler") == 0){
            //ustawianie handlera
            printf("Obslugiwanie sygnalu SIGUSR1\n");
            signal(SIGUSR1, handler);
        }
        else if(strcmp(argv[1], "mask") == 0){
            //ustawiam maske na sygnal SIGUSR1
            printf("Maskowanie sygnalu SIGUSR1\n");
            sigset_t mask;
            sigemptyset(&mask);
            sigaddset(&mask, SIGUSR1);
            if(sigprocmask(SIG_SETMASK, &mask, NULL) < 0){
                perror("Nie udalo sie ustawic maski!\n");
                exit(1);
            }
        }
        else if(strcmp(argv[1], "pending") == 0){
            //ustawiam maske na sygnal SIGUSR1
            printf("Maskowanie sygnalu SIGUSR1 wraz z badaniem oczekiwania\n");
            sigset_t mask;
            sigemptyset(&mask);
            sigaddset(&mask, SIGUSR1);
            if(sigprocmask(SIG_SETMASK, &mask, NULL) < 0){
                perror("Nie udalo sie ustawic maski!\n");
                exit(1);
            }
            pending = 1;
        }
        else{
            perror("Niepoprawny argument!\n");
            exit(2);
        }

        //dalsze dzialania

        //wysylam do siebie sygnal SIGUSR1
        raise(SIGUSR1);

        //jesli badam pending, to sprawdzam czy sygnal SIGUSR1 jest oczekujacy
        if(pending){
            sigset_t pending_signals;
            sigpending(&pending_signals);
            printf("Przodek: %d\n", sigismember(&pending_signals, SIGUSR1));

        }

        //tworze potomka
        int newPID = fork();
        if(newPID == 0){
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
        }



    }
    else{
        perror("Niepoprawna liczba argumentow!\n");
        exit(3);
    }



    return 0;
}