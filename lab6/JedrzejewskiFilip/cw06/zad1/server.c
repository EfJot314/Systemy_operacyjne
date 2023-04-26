#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<string.h>

#include"constData.h"







int main(){
    //tworze klucz
    int key = ftok(getenv("HOME"), SERVERCHAR);

    int msg = msgget(key, IPC_CREAT | 0666);
    if(msg == -1){
        perror("Blad podczas tworzenia kolejki!");
        exit(1);
    }


    //tablica klientow
    int msgTab[100];

    for(int i=0;i<100;i++){
        msgTab[i] = -1;
    }


    //glowna petla servera
    struct msgbuf com;
    while(1){
        //czekam na komunikat
        int result = msgrcv(msg, &com, sizeof(com.id)+sizeof(com.intData)+sizeof(com.charData), INT_MIN, 0);
        if(result != -1){
            //STOP
            if(com.mtype == STOP){
                msgTab[com.intData] = -1;
            }
            //LIST
            else if(com.mtype == LIST){
                //tworze str z lista
                char* myClients = (char*)calloc(MAXCOMLEN, sizeof(char));
                for(int i=0;i<100;i++){
                    if(msgTab[i] != -1){
                        char numberString[MAXCOMLEN];
                        sprintf(numberString, "%d", i);
                        strcat(myClients, numberString);
                        strcat(myClients, " ");
                    }
                }
                //tworze wiadomosc
                struct msgbuf newMsg;
                newMsg.mtype = LIST;
                newMsg.intData = 0;
                strcpy(newMsg.charData, myClients);
                //wysylam wiadomosc do klienta
                msgsnd(msgTab[com.id], &newMsg, sizeof(com.id)+sizeof(com.intData)+sizeof(com.charData), 0);   
            }
            //2ALL
            else if(com.mtype == _2ALL){
                //tworze wiadomosc
                struct msgbuf newMsg;
                newMsg.mtype = MESSAGE;
                newMsg.intData = com.id;
                strcpy(newMsg.charData, com.charData);
                //wysylam wiadomosc do wszystkich klientow
                for(int i=0;i<100;i++){
                    if(i != com.id && msgTab[i] != -1){
                        msgsnd(msgTab[i], &newMsg, sizeof(com.id)+sizeof(com.intData)+sizeof(com.charData), 0);
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
                msgsnd(msgTab[com.intData], &newMsg, sizeof(com.id)+sizeof(com.intData)+sizeof(com.charData), 0);   
            }
            //INIT
            else if(com.mtype == INIT){
                //dodaje nowego clienta
                int newID;
                for(int i=0;i<100;i++){
                    if(msgTab[i] == -1){
                        newID = i;
                        msgTab[newID] = msgget(com.intData, 0666);
                        break;
                    }
                }
                

                //tworze wiadomosc
                struct msgbuf newMsg;
                newMsg.mtype = INIT;
                newMsg.intData = newID;
                strcpy(newMsg.charData, "");
                //wysylam wiadomosc do klienta
                msgsnd(msgTab[newID], &newMsg, sizeof(com.id)+sizeof(com.intData)+sizeof(com.charData), 0);   
            }


        }
        
    }


    //usuwam kolejke
    msgctl(key, IPC_RMID, NULL);



    return 0;
}