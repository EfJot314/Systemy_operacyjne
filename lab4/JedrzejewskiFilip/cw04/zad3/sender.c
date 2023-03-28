#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>


int catcherPID;
int stoper = 1;


void sendSignal(int PID, int value){
    union sigval ssv;
    ssv.sival_int = 1;
    sigqueue(PID, SIGUSR1, ssv);
}

void handle(int sig_no, siginfo_t *info, void *ucontext){
    int senderPID = info->si_pid;
    union sigval sv = info->si_value;

    if(senderPID == catcherPID){
        //jesli otrzymalem potwierdzenie
        if(sv.sival_int == 0){
            //to pozwalam wyslac kolejny sygnal
            stoper = 0;
            printf("Fajnie, wrocilo!\n");
        }
        else{
            perror("Nie otrzymano potwierdzenia wykonania polecenia!\n");
            exit(3);
        }
    }
    else{
        perror("Otrzymano sygnal z nieznanego zrodla!\n");
    }
}


int main(int argc, char* argv[]){
    //pobieram argumenty i sprawdzam czy wszystko z nimi jest ok
    if(argc == 2){
        //pobieram PID i sprawdzam czy jest ok
        catcherPID = atoi(argv[1]);
        if(catcherPID == 0){
            perror("Podany PID jest niepoprawny!\n");
            exit(2);
        }


        //ustawiam obsluge sygnalu SIGUSR1
        struct sigaction act; 
        act.sa_handler = handle;
        sigemptyset(&act.sa_mask);
        act.sa_flags = SA_SIGINFO;
        sigaction(SIGUSR1, &act, NULL);


        //wysylam sygnal
        sendSignal(catcherPID, 1);

        while(stoper){
            
        }
        

    }
    else{
        perror("Niepoprawna liczba argumentow!\n");
        exit(1);
    }



    return 0;
}