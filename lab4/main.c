#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>


int counter = 0;

void foo(int signum){
    //obsluga sygnalu
    counter++;
    printf("ja cie ale sygnal :C, numer: %d \n", counter);
}


int main(){

    
    //definiuje co robie z danym sygnalem
    signal(SIGUSR1, foo);


    int newPID = fork();

    //jesli jestem tata to wysylam do dziecka sygnaly
    if(newPID > 0){
        for(int i=0;i<100;i++){
            kill(newPID, SIGUSR1);
        }
    }

    return 0;
}