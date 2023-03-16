#include<stdio.h>
#include<string.h>
#include<sys/wait.h>
#include<sys/resource.h>




int main(int argn, char* argv[]){
    //pobieranie argumentu i sprawdzanie czy wszystko z nim jest ok
    if(argn == 2){
        char* number_as_char = argv[1];
        int n = atoi(number_as_char);
        if(n > 0 || (n == 0 && strcmp(number_as_char, "0") == 0)){
            
            //glowna petla tworzaca procesy 
            for(int i=0;i<n;i++){
                int newPID = fork();
                //polecenia dla dziecka
                if(newPID == 0){
                    printf("PID dziecka: %d, PID rodzica: %d\n", getpid(), getppid());
                    return 0;
                }
                //czekam na dziecko zeby pojsc dalej
                wait(NULL);

            }
            
            //print argumentu na koncu
            printf("argv[1] = %d\n", n);

        }
        else{
            printf("Niepoprawny argument!\n");
        }
    }   
    else{
        printf("Niepoprawna liczba argumentow!\n");
    }


    return 0;
}