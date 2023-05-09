#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/mman.h>
#include<sys/sem.h>
#include<signal.h>
#include<unistd.h>


#define M 5
#define N 6
#define P 5



union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};



int main(int argc, char* argv[]){

    int num = atoi(argv[0]);


    struct sembuf * waiting = (struct sembuf*)calloc(1, sizeof(struct sembuf));
	waiting->sem_num = num;
	waiting->sem_op = 0;


    //otwieram semafory
	int key1 = ftok(".", 'F');
	int key2 = ftok(".", 'C');
	int key3 = ftok(".", 'P');
	int fryz = semget(key1, 0, 0666);
	int chairs = semget(key2, 0, 0666);
	int waiters = semget(key3, 0, 0666);

    //pamiec wspolna
    int fryzM = shmget(key1, 0, 0666);
	int chairM = shmget(key1, 0, 0666);
	int waitM = shmget(key1, 0, 0666);

    int* fryzTab = (int*)shmat(fryzM, NULL, 0666);
	int* chairTab = (int*)shmat(chairM, NULL, 0666);
	int* waitTab = (int*)shmat(waitM, NULL, 0666);
    


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
            printf("pracujemy!\n");
            sleep(fryzTime);
            //zwalniam fotel
            chairTab[foundChair] = 0;
            //ide spac
            union semun arg;
	        arg.val = 1;
            semctl(fryz, num, SETVAL, arg);
        }

	}


    //odlaczam pamiec wspoldzielona
    shmdt((void*)fryzTab);
	shmdt((void*)chairTab);
	shmdt((void*)waitTab);


	return 0;
}