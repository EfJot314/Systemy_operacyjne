#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include <time.h>


void handle(int sig_no, siginfo_t *info, void *ucontext){
    int sn = info->si_signo;
    int senderPID = info->si_pid;
    int usrID = info->si_uid;
    int sigCode = info->si_code;
    clock_t usrTime = info->si_utime;
    clock_t sysTime = info->si_stime;

    printf("Otrzymano sygnal nr: %d \n", sn);
    printf("PID z ktorego otrzymano sygnal: %d \n", senderPID); 
    printf("ID uzytkownika wysylajacego sygnal: %d \n", usrID); 
    printf("Kod sygnalu: %d \n", sigCode);
    printf("Zuzyty czas uzytkownika: %lf \n", usrTime);
    printf("Zuzyty czas systemowy: %lf \n", sysTime); 


    //wychodze z dziecka
    exit(0);



}


int main(){

    printf("\nProgram testujacy flage SA_SIGINFO wysylajac sygnal SIGUSR2\n");
    printf("Flaga udostepnia dostep do wiekszej liczby informacji o sygnale\n");
    printf("Program wysyla sygnal SIGUSR2, a handler wypisuje informacje dot tego sygnalu \n\n");

    sleep(1);


    //ustawiam obsluge sygnalu SIGUSR2
    struct sigaction act; 
    act.sa_handler = handle;
    sigemptyset(&act.sa_mask); 
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR2, &act, NULL);

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
        kill(newPID, SIGUSR2);


        //czekam chwile na dziecko, jak przyjdzie to dobrze, jak nie to tez dobrze
        sleep(1);
        
    }

    return 0;
}