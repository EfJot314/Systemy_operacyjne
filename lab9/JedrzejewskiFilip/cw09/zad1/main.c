#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>



#define nElf 10
#define nRen 9


int flag = 1;

void handler(int signum){
    //koncze dzialanie programu -> przerywam petle
    flag = 0;
}


int randint(int a, int b){
    return a + rand() % (b-a);
}


void* mikolaj(){
    printf("Jestem Mikolajem!\n");
}


void* renifer(void* args){
    //zbieram id z wejscia
    int id = *(int*)args;
    free((int*)args);
    printf("Jestem reniferem nr %d!\n", id);
    //glowna petla renifera
    while(1){
        //najpierw wakacje w cieplych krajach
        sleep(randint(5,10));

        //potem rozwozenie
        

    }
}


void* elf(void* args){
    //zbieram id z wejscia
    int id = *(int*)args;
    free((int*)args);
    printf("Jestem elfem nr %d!\n", id);
    //glowna petla elfa
    while(1){
        //najpierw praca
        sleep(randint(2,5));

        //potem problemy


    }
}



int main(){
    //dla randoma
    srand(time(NULL));


    //tworze renifery
    pthread_t ** renifers = (pthread_t**)calloc(nRen, sizeof(pthread_t*));
    for(int i=0;i<nRen;i++){
        int *id = (int*)malloc(sizeof(int));
        *id = i;
        void* args = (void*)id;
        pthread_t* newT = (pthread_t*)malloc(sizeof(pthread_t));
        pthread_create(newT, NULL, &renifer, args);
        renifers[i] = newT;
    }


    //tworze elfy
    pthread_t ** elfs = (pthread_t**)calloc(nElf, sizeof(pthread_t*));
    for(int i=0;i<nElf;i++){
        int *id = (int*)malloc(sizeof(int));
        *id = i;
        void* args = (void*)id;
        pthread_t* newT = (pthread_t*)malloc(sizeof(pthread_t));
        pthread_create(newT, NULL, &elf, args);
        elfs[i] = newT;
    }

    //tworze Mikolaja
    pthread_t* mikolajT = (pthread_t*)malloc(sizeof(pthread_t));
    pthread_create(mikolajT, NULL, &mikolaj, NULL);


    //przechwytuje ctr+c
    signal(SIGINT, handler);


    //nieskonczona petla blokujaca program
    while(flag){}


    //zabijam renifery
    for(int i=0;i<nRen;i++){
        pthread_cancel(*renifers[i]);
        free(renifers[i]);
    }
    free(renifers);

    //zabijam elfy
    for(int i=0;i<nElf;i++){
        pthread_cancel(*elfs[i]);
        free(elfs[i]);
    }
    free(elfs);

    //zabijam mikolaja
    pthread_cancel(*mikolajT);
    free(mikolajT);

    return 0;
}