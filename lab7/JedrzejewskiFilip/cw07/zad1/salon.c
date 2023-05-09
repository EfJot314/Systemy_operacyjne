#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/mman.h>
#include<sys/sem.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>
#include"header.h"


int main(){

    //klucze
	int key0 = ftok(".", K1);
	int key1 = ftok(".", K2);
	int key2 = ftok(".", K3);
	int key3 = ftok(".", K4);

    //pamiec wspolna
    int fryzM = shmget(key0, 0, 0666);
	int chairM = shmget(key2, 0, 0666);
	int waitM = shmget(key3, 0, 0666);

    int* fryzTab = (int*)shmat(fryzM, NULL, 0666);
	int* chairTab = (int*)shmat(chairM, NULL, 0666);
	int* waitTab = (int*)shmat(waitM, NULL, 0666);



    while(1){
        //czekam, zeby nie dostac oczoplasu
        sleep(1);


		//zbieram dane
		int fNum = 0;
		for(int i=0;i<M;i++){
			if(fryzTab[i] > 0){
				fNum++;
			}
		}
		int cNum = 0;
		for(int i=0;i<N;i++){
			if(chairTab[i] > 0){
				cNum++;
			}
		}
		int wNum = 0;
		for(int i=0;i<P;i++){
			if(waitTab[i] > 0){
				wNum++;
			}
		}

		//wywietlam dane
		printf("------------------------\n");

		printf("Fryzjerzy: %d/%d \n", fNum, M);
        printf("Fotele: %d/%d \n", cNum, N);
        printf("Miejsca w poczekalni: %d/%d \n", wNum, P);

		printf("------------------------\n");
	}

    //odlaczam pamiec wspoldzielona
    shmdt((void*)fryzTab);
	shmdt((void*)chairTab);
	shmdt((void*)waitTab);

    return 0;
}