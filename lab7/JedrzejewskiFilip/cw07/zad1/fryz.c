#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/mman.h>
#include<sys/sem.h>
#include<signal.h>
#include<unistd.h>
#include"header.h"





int main(int argc, char* argv[]){

    int num = atoi(argv[0]);


    struct sembuf * waiting = (struct sembuf*)calloc(1, sizeof(struct sembuf));
	waiting->sem_num = num;
	waiting->sem_op = 0;


    //klucze
	int key0 = ftok(".", K1);
	int key1 = ftok(".", K2);
	int key2 = ftok(".", K3);
	int key3 = ftok(".", K4);

    //semafory
	int fryz = semget(key1, 0, 0666);

    //pamiec wspolna
    int fryzM = shmget(key0, 0, 0666);
	int chairM = shmget(key2, 0, 0666);
	int waitM = shmget(key3, 0, 0666);

    int* fryzTab = (int*)shmat(fryzM, NULL, 0666);
	int* chairTab = (int*)shmat(chairM, NULL, 0666);
	int* waitTab = (int*)shmat(waitM, NULL, 0666);
    


    //glowna petla fryzjera
    while(1){
		semop(fryz, waiting, 1);

        int fryzTime = fryzTab[num];


        //szukam fotela
        int foundChair = -1;
        for(int i=0;i<N;i++){
            if(chairTab[i] == 0){
                chairTab[i] = 1;
                foundChair = i;
                break;
            }
        }


        //jesli nie znalazlem to do poczekalni
        if(foundChair == -1){
            for(int i=0;i<P;i++){
                if(waitTab[i] == 0){
                    waitTab[i] = fryzTime;
                    break;
                }
            }
            //niewazne czy jest w poczekalni miejsce dla klienta czy nie, to fryzjer go porzuca
            fryzTab[num] = 0;
            union semun arg;
	        arg.val = 1;
            semctl(fryz, num, SETVAL, arg);
        
        }
        //jesli znalazlem fotel to pracuje
        else{
            sleep(fryzTime);

            //po pracy zwalniam fotel
            chairTab[foundChair] = 0;

            //koncze prace
            fryzTab[num] = 0;

            //ide spac
            union semun arg;
	        arg.val = 1;
            semctl(fryz, num, SETVAL, arg);

            //sprawdzm czy ktos jest w poczekalni
            if(waitTab[0] > 0){
                //jesli ktos jest to szukam dla niego fryzjera
                for(int i=0;i<M;i++){
                    if(fryzTab[i] == 0){
                        fryzTab[i] = waitTab[0];
                        //przesuwam waiterow
                        for(int j=1;j<P;j++){
                            waitTab[j-1] = waitTab[j];
                            waitTab[j] = 0;
                        }
                        union semun arg;
                        arg.val = 0;
                        semctl(fryz, i, SETVAL, arg);
                        break;
                    }
                }
            }

            
        }

	}


    //odlaczam pamiec wspoldzielona
    shmdt((void*)fryzTab);
	shmdt((void*)chairTab);
	shmdt((void*)waitTab);


	return 0;
}