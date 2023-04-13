#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<sys/msg.h>
#include<stdlib.h>




struct msgbuf {
  long mtype;         /* typ komunikatu   */
  char mtext[256];      /* tresc komunikatu */
};


int main(){
    //tworze klucz
    int key = ftok(".", 'F');

    int msg = msgget(key, IPC_CREAT | 0666);
    if(msg == -1){
        perror("Blad podczas tworzenia kolejki!");
        exit(1);
    }

    struct msgbuf m1 = {1, "Hello"};
    struct msgbuf m2 = {2, "world!"};

    msgsnd(msg, &m1, sizeof(m1.mtext), IPC_NOWAIT);
    msgsnd(msg, &m2, sizeof(m2.mtext), IPC_NOWAIT);

    //statystyczki
    struct msqid_ds buf;
    msgctl(msg, IPC_STAT, &buf);

    //wypisanie statystyczek
    printf("Liczba komunikatow w kolejce: %d\n", buf.msg_qnum);
    printf("Liczba bajtow w kolejce: %d\n", buf.msg_cbytes);
    printf("PID ostatniego zapisujacego procesu: %d\n", buf.msg_lspid);

    struct msgbuf r1;
    struct msgbuf r2;

    msgrcv(msg, &r1, sizeof(r1.mtext), 1, IPC_NOWAIT);
    msgrcv(msg, &r2, sizeof(r2.mtext), 2, IPC_NOWAIT);

    printf("Komunikaty: %s %s\n", r1.mtext, r2.mtext);

    

    return 0;
}