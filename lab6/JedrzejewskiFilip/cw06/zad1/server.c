#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<string.h>

#include"constData.h"







int main(){
    //tworze klucz
    int key = ftok("$HOME", SERVERCHAR);

    int msg = msgget(key, IPC_CREAT | 0666);
    if(msg == -1){
        perror("Blad podczas tworzenia kolejki!");
        exit(1);
    }

    //tablica klientow
    int nOfClients = 0;
    int msgTab[100];


    //glowna petla servera
    struct msgbuf com;
    while(1){
        //czekam na komunikat
        int result = msgrcv(msg, &com, sizeof(com.id)+sizeof(com.intData)+sizeof(com.charData), INIT, NULL);
        if(result != -1){
            //STOP
            if(com.mtype == STOP){
                //przesuwam pozostale adresy kolejek
                for(int i=com.intData;i<nOfClients-1;i++){
                    msgTab[i] = msgTab[i+1];
                }
                //zmniejszam liczbe aktywnych klientow
                nOfClients -= 1;
            }
            //LIST
            else if(com.mtype == LIST){
                //printuje wzytkich klientow
                for(int i=0;i<nOfClients;i++){
                    printf("%d ", msgTab[i]);
                }
                printf("\n");
            }
            //2ALL
            else if(com.mtype == _2ALL){
                //tworze wiadomosc
                struct msgbuf newMsg;
                newMsg.mtype = MESSAGE;
                newMsg.intData = com.id;
                strcpy(newMsg.charData, com.charData);
                //wysylam wiadomosc do wszystkich klientow
                for(int i=0;i<nOfClients;i++){
                    if(i != com.id){
                        msgsnd(msgTab[i], &newMsg, sizeof(com.id)+sizeof(com.intData)+sizeof(com.charData), NULL);
                    }
                }
            }
            //2ONE
            else if(com.mtype == _2ONE){
                //tworze wiadomosc
                struct msgbuf newMsg;
                newMsg.mtype = MESSAGE;
                newMsg.intData = com.id;
                strcpy(newMsg.charData, com.charData);
                //wysylam wiadomosc do klienta
                msgsnd(msgTab[com.intData], &newMsg, sizeof(com.id)+sizeof(com.intData)+sizeof(com.charData), NULL);   
            }


        }
        
    }


    //usuwam kolejke
    msgctl(key, IPC_RMID, NULL);



    return 0;
}