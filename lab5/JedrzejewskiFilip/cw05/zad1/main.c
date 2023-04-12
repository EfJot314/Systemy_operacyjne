#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>


int main(int argc, char* argv[]){

    //wywolanie z jednym argumentem
    if(argc == 2){

    }
    //wywolanie z trzema argumentami
    else if(argc == 4){
        //pobieram argumenty
        char* email = argv[1];
        char* title = argv[2];
        char* content = argv[3];

        

    }
    else{
        perror("Niepoprawna liczba argumentow!");
    }

    return 0;
}