#include<stdio.h>
#include<sys/sem.h>
#include<sys/ipc.h>
#include<sys/types.h>



int main(){

    int key = ftok(".", 'C');

    int sems = semget(key, 1, IPC_CREAT | 0666);

    struct sembuf sss;
    sss.sem_num = 0;
    sss.sem_num = -1;
    sss.sem_flg = IPC_NOWAIT;

    

    semop(sems, &sss, 1);

    semctl(sems, 0, SETVAL, 1);

    union semum cc;

    semctl(sems, 0, GETVAL, cc);

    printf("%d\n", cc.val);

    semctl(sems, 0, IPC_RMID);


    return 0;
}