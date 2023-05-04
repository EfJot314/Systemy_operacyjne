#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/mman.h>


#define M 10
#define N 6
#define P 5


long comSize = 1000;




int main(){
	//tworze 3 semafory (1-fryzjerzy 2-fotele 3-poczekalnia)
	int key1 = ftok(".", 'F');
	int key2 = ftok(".", 'C');
	int key3 = ftok(".", 'P');
	int fryz = semget(key1, M, IPC_CREAT | 0666);
	int chairs = semget(key2, N, IPC_CREAT | 0666);
	int waiters = semget(key3, P, IPC_CREAT | 0666);


	int fd[2];
	pipe(fd);


	int salonPID = fork();
	//kod salonu
	if(salonPID == 0){
		//zamykam koncowke do pisania
		close(fd[1]);

		char res[1000];
		read(fd[0], res, 1000);


		printf("%s\n", res);
	}
	//kod pobierania danych
	else{
		//zamykam koncowke do czytania
		close(fd[0]);

		while(1){
			//zbieram dane z wejscia
			char* comm = NULL;
			getline(&comm, &comSize, stdin);
			int fryzTime = atoi(comm);
			if(fryzTime == 0){
				perror("Niepoprawna dana!");
			}
			free(comm);

			//wysylam klienta do salonu
			write(fd[1], "fajnie", 1000);

			break;
		}

		//usuwam semafory
		semctl(fryz, NULL, IPC_RMID, NULL);
		semctl(chairs, NULL, IPC_RMID, NULL);
		semctl(waiters, NULL, IPC_RMID, NULL);

	}
	
	
	
	
	

	return 0;
}



