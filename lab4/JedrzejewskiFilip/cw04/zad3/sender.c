#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>



int main(int argc, char* argv[]){
    //pobieram argumenty i sprawdzam czy wszystko z nimi jest ok
    if(argc == 2){
        //pobieram PID i sprawdzam czy jest ok
        int catcherPID = atoi(argv[1]);
        if(catcherPID == 0){
            perror("Podany PID jest niepoprawny!\n");
            exit(2);
        }

        union sigval sv;
        sv.sival_int = 314;
        

        sigqueue(catcherPID, SIGUSR1, sv);
        

    }
    else{
        perror("Niepoprawna liczba argumentow!\n");
        exit(1);
    }



    return 0;
}