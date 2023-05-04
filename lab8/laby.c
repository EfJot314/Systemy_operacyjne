#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>




void f1(void* arg){
    printf("HW\n");
}


int main(int argn, char* argc[]){

    // int n = atoi(argc[1]);


    pthread_t* thr = (pthread_t*)calloc(1, sizeof(pthread_t)); 
    pthread_create(&thr, NULL, f1, NULL);



    free(thr);
    



    return 0;
}