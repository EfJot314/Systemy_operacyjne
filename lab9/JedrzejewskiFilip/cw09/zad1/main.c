#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>


//kolory
#define MY_COLOR_RED     "\x1b[31m"
#define MY_COLOR_GREEN   "\x1b[32m"
#define MY_COLOR_YELLOW  "\x1b[33m"
#define MY_COLOR_RESET   "\x1b[0m"

#define nElf 10
#define nRen 9



//mutexy
pthread_mutex_t elfMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mikolajMutex = PTHREAD_MUTEX_INITIALIZER;

//condy
pthread_cond_t elfCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t elfWaitCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t reniferCond = PTHREAD_COND_INITIALIZER;
pthread_cond_t mikolajCond = PTHREAD_COND_INITIALIZER;

//zmienne wspoldzielone
int waitingElfs = 0;
int elfID[3];
int waitingRenifers = 0;


int flag = 1;

void handler(int signum){
    //koncze dzialanie programu -> przerywam petle
    flag = 0;
}


int randint(int a, int b){
    return a + rand() % (b-a);
}




void* renifer(void* args){
    //zbieram id z wejscia
    int id = *(int*)args;
    free((int*)args);
    //glowna petla renifera
    while(1){
        //najpierw wakacje w cieplych krajach
        sleep(randint(5,10));

        //potem zalatwianie formalnosci z Mikolajem
        pthread_mutex_lock(&mikolajMutex);
        waitingRenifers++;
        printf(MY_COLOR_YELLOW "Renifer: czeka %d reniferów na Mikołaja, %d\n" MY_COLOR_RESET, waitingRenifers, id);
        //czekam na Mikolaja
        if(waitingRenifers < 9){
            while(waitingRenifers < 9){
                pthread_cond_wait(&reniferCond, &mikolajMutex);
            }
        }
        else{
            //krzycze do pozostalych ze czas sie zbierac
            pthread_cond_broadcast(&reniferCond);

            //jesli juz sa wszyscy to budzimy Mikolaja
            if(waitingRenifers == 9){
                //budze Mikolaja
                printf(MY_COLOR_YELLOW "Renifer: wybudzam Mikołaja, %d\n" MY_COLOR_RESET, id);
                pthread_cond_broadcast(&mikolajCond);
            }
        }
        
        //czekamy na dowiezienie wszystkich prezentow
        while(waitingRenifers != 0){
            pthread_cond_wait(&reniferCond, &mikolajMutex);
        }

        pthread_mutex_unlock(&mikolajMutex);

    }
}


void* elf(void* args){
    //zbieram id z wejscia
    int id = *(int*)args;
    free((int*)args);
    //glowna petla elfa
    while(1){
        //najpierw praca
        sleep(randint(2,5));

        //potem problemy
        pthread_mutex_lock(&mikolajMutex);
        //jesli problemy aktualnie sa rozwiazywane to nie zglaszam zadnych
        while(waitingElfs == -1){
            pthread_cond_broadcast(&elfCond);
        }

        //jesli jednak mikolaj spi albo jezdzi sobie z reniferami po swiecie to zglaszam swoje problemy
        int waiting = 0;
        int myInd = 0;
        if(waitingElfs < 3){
            pthread_mutex_lock(&elfMutex);
            elfID[waitingElfs] = id;
            pthread_mutex_unlock(&elfMutex);
            waitingElfs++;
            printf(MY_COLOR_GREEN "Elf: czeka %d elfów na Mikołaja, %d\n" MY_COLOR_RESET, waitingElfs, id);
            if(waitingElfs == 3){
                printf(MY_COLOR_GREEN "Elf: wybudzam Mikołaja, %d\n" MY_COLOR_RESET, id);
                pthread_cond_broadcast(&mikolajCond);
            }
            waiting = 1;
            myInd = waitingElfs-1;
        }
        else{
            printf(MY_COLOR_GREEN "Elf: samodzielnie rozwiązuję swój problem, %d\n" MY_COLOR_RESET, id);
        }
        pthread_mutex_unlock(&mikolajMutex);

        if(waiting){
            //czekam na rozwiazanie moich problemow
            pthread_mutex_unlock(&elfMutex);
            while(elfID[myInd] == id){
                pthread_cond_wait(&elfWaitCond, &elfMutex);
            }
            printf(MY_COLOR_GREEN "Elf: Mikołaj rozwiązuje problem, %d\n" MY_COLOR_RESET, id);
            pthread_mutex_unlock(&elfMutex);
            
        }

    }
}



int main(){
    //dla randoma
    srand(time(NULL));

    //ustawiam wartosci domyslne
    for(int i=0;i<3;i++){
        elfID[i] = -1;
    }


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


    //przechwytuje ctr+c
    signal(SIGINT, handler);


    //nieskonczona petla bedaca Mikolajem
    while(flag){
        pthread_mutex_lock(&mikolajMutex);
        //spie
        while(waitingElfs != 3 && waitingRenifers < 9){
            pthread_cond_wait(&mikolajCond, &mikolajMutex);
        }
        //budze sie
        printf(MY_COLOR_RED "Mikołaj: budzę się\n" MY_COLOR_RESET);
        //obsluga elfow
        if(waitingElfs == 3){
            printf(MY_COLOR_RED "Mikołaj: rozwiązuje problemy elfów %d, %d, %d \n" MY_COLOR_RESET, elfID[0], elfID[1], elfID[2]);
            //ustawiam tak, aby elfy nie mialy chwilowo problemow
            waitingElfs = -1;
            // pthread_mutex_lock(&elfMutex);
            for(int i=0;i<3;i++){
                pthread_mutex_lock(&elfMutex);
                elfID[i] = -1;
                pthread_cond_broadcast(&elfWaitCond);
                pthread_mutex_unlock(&elfMutex);

                sleep(randint(1,2));
                
            }
            
            waitingElfs = 0;
            pthread_cond_broadcast(&elfCond);
        }
        //obsluga reniferow
        if(waitingRenifers == 9){
            //dostarczam zabawki
            printf(MY_COLOR_RED "Mikołaj: dostarczam zabawki\n" MY_COLOR_RESET);
            sleep(randint(2,4));
            waitingRenifers = 0;
            pthread_cond_broadcast(&reniferCond);
        }
        

        pthread_mutex_unlock(&mikolajMutex);
        
    }


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
    // pthread_cancel(*mikolajT);
    // free(mikolajT);

    return 0;
}