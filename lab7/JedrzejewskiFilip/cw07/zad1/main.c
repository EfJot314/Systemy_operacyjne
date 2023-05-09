#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/mman.h>
#include<sys/sem.h>
#include<signal.h>
#include<unistd.h>
#include<string.h>


#define M 5
#define N 6
#define P 5


long comSize = 1000;


union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};


int main(){
	//tworze 3 zbiory kluczy i semaforow (1-fryzjerzy 2-fotele 3-poczekalnia)
	int key1 = ftok(".", 'F');
	int key2 = ftok(".", 'C');
	int key3 = ftok(".", 'P');


	int fryz = semget(key1, M, IPC_CREAT | 0666);
	int chairs = semget(key2, N, IPC_CREAT | 0666);
	int waiters = semget(key3, P, IPC_CREAT | 0666);

	//tworze pamiec wspolna
	int fryzM = shmget(key1, M*sizeof(int), IPC_CREAT | 0666);
	int chairM = shmget(key1, N*sizeof(int), IPC_CREAT | 0666);
	int waitM = shmget(key1, P*sizeof(int), IPC_CREAT | 0666);

	//otwieram pamiec wspolna
	int* fryzTab = (int*)shmat(fryzM, NULL, 0666);
	int* chairTab = (int*)shmat(chairM, NULL, 0666);
	int* waitTab = (int*)shmat(waitM, NULL, 0666);

	//inicjalizacja semaforow i czyszczenie tablic
	union semun arg;
	arg.val = 1;
	for(int i=0;i<M;i++){
		semctl(fryz, i, SETVAL, arg);
		fryzTab[i] = 0;
	}
	for(int i=0;i<N;i++){
		semctl(chairs, i, SETVAL, arg);
		// chairTab[i] = 0;
	}
	for(int i=0;i<P;i++){
		semctl(waiters, i, SETVAL, arg);
		waitTab[i] = 0;
	}



	int fPIDs[M];

	//tworze fryzjerow
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
		while(1){
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
			// sleep(1);
			printf("------------------------\n");

			printf("Fryzjerzy: %d/%d \n", fNum, M);

			printf("------------------------\n");
		}
		
		
		
	}
	//kod pobierania danych
	else{

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
		semctl(chairs, 0, IPC_RMID, NULL);
		semctl(waiters, 0, IPC_RMID, NULL);

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



