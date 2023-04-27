#include<stdio.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/types.h>



int main(){

    int key = ftok(".", 'C');

    int sems = semget(key, 1, IPC_CREAT | 0666);



    int newPID = fork();
    if(newPID == 0){
        
    }
    else{

    }


    return 0;
}