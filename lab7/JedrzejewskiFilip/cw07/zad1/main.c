#include<stdio.h>
#include<stdlib.h>
#include<sys/shm.h>
#include<sys/ipc.h>
#include<sys/mman.h>
#include<signal.h>


#define maxComLen 1000

#define M 10
#define N 6
#define P 5


long comSize = 1000;




int main(){
	//tworze 3 zbiory semaforow (1-fryzjerzy 2-fotele 3-poczekalnia)
	int key1 = ftok(".", 'F');
	int key2 = ftok(".", 'C');
	int key3 = ftok(".", 'P');
	int fryz = semget(key1, M, IPC_CREAT | 0666);
	int chairs = semget(key2, N, IPC_CREAT | 0666);
	int waiters = semget(key3, P, IPC_CREAT | 0666);


	int fd[2];
	pipe(fd);


	//tworze fryzjerow
	for(int i=0;i<M;i++){
		int num = i;
		if(fork() == 0){
			while(1){
				printf("%d \n", num);
				break;
			}
			return 0;
		}

	}


	int salonPID = fork();
	//kod salonu
	if(salonPID == 0){
		
		//zamykam koncowke do pisania
		close(fd[1]);

		

		while(1){
			char res[maxComLen];
			read(fd[0], res, maxComLen);

			int fryzTime = atoi(res);
			if(fryzTime == 0){
				perror("Niepoprawna dana!");
				continue;
			}



			printf("%s\n", res);
		}
		
	}
	//kod pobierania danych
	else{
		//zamykam koncowke do czytania
		close(fd[0]);

		while(1){
			//zbieram dane z wejscia
			char* comm = NULL;
			getline(&comm, &comSize, stdin);

			//wysylam klienta do salonu
			write(fd[1], comm, maxComLen);

			free(comm);

			break;
		}

		//usuwam semafory
		semctl(fryz, NULL, IPC_RMID, NULL);
		semctl(chairs, NULL, IPC_RMID, NULL);
		semctl(waiters, NULL, IPC_RMID, NULL);


	}
	
	
	
	
	

	return 0;
}



