#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>


//deklaracje zmiennych
int catcherPID;
int stoper = 1;


//funkcja wysylajaca sygnal SIGUSR1 z wartocia value do procesu o podanym PID
void sendSignal(int PID, int value){
    union sigval ssv;
    ssv.sival_int = value;
    //wysylam sygnal i sprawdzam czy nie bylo z tym problemow
    int result = sigqueue(PID, SIGUSR1, ssv);
    if(result < 0){
        perror("Sygnal nie zostal wyslany!\n");
        //zeruje stoper, poniewaz chce przejsc do kolejnego polecenia
        stoper = 0;
    }
}


//handler sygnalu SIGUSR1
void handle(int sig_no, siginfo_t *info, void *ucontext){
    int senderPID = info->si_pid;
    union sigval sv = info->si_value;

    if(senderPID == catcherPID){
        //jesli otrzymalem potwierdzenie
        if(sv.sival_int == 0){
            //to pozwalam wyslac kolejny sygnal
            stoper = 0;
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
    if(argc >= 3){
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


        
        //przechodze po argumentach i wysylam polecenia do catchera
        for(int i=2;i<argc;i++){
            //zamieniam argument na liczbe i sprawdzam czy wszystko jest ok
            int tribeNumber = atoi(argv[i]);
            if(tribeNumber == 0){
                perror("Niepoprawny podany tryb pracy");
                exit(4);
            }

            //jesli jest ok, to wysylam polecenie do catchera
            sendSignal(catcherPID, tribeNumber);
            
            //jesli poleceniem byl tryb 5, to koncze prace sendera
            if(tribeNumber == 5){
                return 0;
            }

            //zatrzymuje dzialanie programu dopoki nie dostane potwierdzenia wykonania polecenia
            while(stoper){

            }
            stoper = 1;

        }
        

    }
    else{
        perror("Niepoprawna liczba argumentow!\n");
        exit(1);
    }


    return 0;
}