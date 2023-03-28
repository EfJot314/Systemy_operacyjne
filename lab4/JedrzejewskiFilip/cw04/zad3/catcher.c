#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>


int stoper = 1;


void sendSignal(int PID, int value){
    union sigval ssv;
    ssv.sival_int = value;
    sigqueue(PID, SIGUSR1, ssv);
}

void handle(int sig_no, siginfo_t *info, void *ucontext){
    int senderPID = info->si_pid;
    union sigval sv = info->si_value;

    printf("Otrzymano sygnal!\n");
    
    //wysylam potwierdzenie
    sendSignal(senderPID, 0);
    
}


int main(){

    //wypisanie PIDu
    printf("PID: %d\n", getpid());


    //ustawiam obsluge sygnalu SIGUSR1
    struct sigaction act; 
    act.sa_handler = handle;
    sigemptyset(&act.sa_mask); 
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);



    //czekam na sygnaly
    while(stoper){

    }


    return 0;
}