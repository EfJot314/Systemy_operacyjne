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


long comSize = 1000;





int main(){
	//tworze 3 zbiory kluczy i semaforow (1-fryzjerzy 2-fotele 3-poczekalnia)
	int key0 = ftok(".", K1);
	int key1 = ftok(".", K2);
	int key2 = ftok(".", K3);
	int key3 = ftok(".", K4);


	//semafory
	int fryz = semget(key1, M, IPC_CREAT | 0666);


	//tworze pamiec wspolna
	int fryzM = shmget(key0, M*sizeof(int), IPC_CREAT | 0666);
	int chairM = shmget(key2, N*sizeof(int), IPC_CREAT | 0666);
	int waitM = shmget(key3, P*sizeof(int), IPC_CREAT | 0666);

	//otwieram pamiec wspolna
	int* fryzTab = (int*)shmat(fryzM, NULL, 0666);
	int* chairTab = (int*)shmat(chairM, NULL, 0666);
	int* waitTab = (int*)shmat(waitM, NULL, 0666);

	//inicjalizacja semaforow i czyszczenie tablic
	union semun arg;
	arg.val = 1;
	for(int i=0;i<M;i++){
		fryzTab[i] = 0;
		semctl(fryz, i, SETVAL, arg);
	}
	for(int i=0;i<N;i++){
		chairTab[i] = 0;
	}
	for(int i=0;i<P;i++){
		waitTab[i] = 0;
	}



	

	//tworze fryzjerow
	int fPIDs[M];
	for(int i=0;i<M;i++){

		char str[10];
		sprintf(str, "%d", i);
		
		int fPID = fork();
		if(fPID == 0){
			execl("./fryz", str, NULL);
		}
		else{
			fPIDs[i] = fPID;
		}

	}


	int salonPID = fork();
	//kod salonu
	if(salonPID == 0){
		execl("./salon", NULL);
	}
	//kod pobierania danych
	else{
		//petla glowna programu - obsluga salonu
		while(1){
			//zbieram dane z wejscia
			char* comm = NULL;
			getline(&comm, &comSize, stdin);

			//komenda wyjscia z programu
			if(strcmp(comm, "exit\n") == 0){
				kill(salonPID, SIGKILL);
				for(int i=0;i<M;i++){
					kill(fPIDs[i], SIGKILL);
				}
				return 0;
			}

			//jesli nie wychodze to pewnie cche dodac nowego klienta
			int fryzTime = atoi(comm);
			if(fryzTime <= 0){
				perror("Niepoprawny czas!");
				continue;
			}


			//szukam fryzjera
			int foundFryz = -1;
			for(int i=0;i<M;i++){
				//jesli jakis fryzjer jest wolny to bierze klienta, ew jesli nie bedzie wolnych foteli to go zwroci do poczekalni
				if(fryzTab[i] == 0){
					foundFryz = i;
					fryzTab[i] = fryzTime;
					union semun arg;
					arg.val = 0;
					semctl(fryz, i, SETVAL, arg);
					break;
				}
			}



			//jesli nie znalazl sie wolny fryzjer to klient idzie do poczekalni
			if(foundFryz == -1){
				for(int i=0;i<P;i++){
					if(waitTab[i] == 0){
						waitTab[i] = fryzTime;
						break;
					}
				}
			}


		}

		//usuwam semafory
		semctl(fryz, 0, IPC_RMID, NULL);

		//usuwam pamiec wspolna
		shmdt((void*)fryzTab);
		shmdt((void*)chairTab);
		shmdt((void*)waitTab);

		shmctl(fryzM, IPC_RMID, NULL);
		shmctl(chairM, IPC_RMID, NULL);
		shmctl(waitM, IPC_RMID, NULL);


	}
	
	
	

	return 0;
}



