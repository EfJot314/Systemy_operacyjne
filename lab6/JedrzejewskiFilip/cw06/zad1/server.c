#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>
#include<time.h>
#include<fcntl.h>

#include"constData.h"


#define MAX_N_OF_CLIENTS 300


int loop = 1;
int msg;
int msgTab[MAX_N_OF_CLIENTS];
int fd;



void saveLogs(int type, int id, int intData, char* text){
    //tworze wiadomosc do zapisania
    char* toSave = (char*)calloc(MAXCOMLEN, sizeof(char));

    time_t t = time(NULL);
    struct tm tim = *localtime(&t);
    char* date = (char*)calloc(MAXCOMLEN, sizeof(char));
    sprintf(date, "%d-%d-%d=%d:%d:%d", tim.tm_year+1900, tim.tm_mon+1, tim.tm_mday, tim.tm_hour, tim.tm_min, tim.tm_sec);

    strcpy(toSave, date);

    free(date);

    strcat(toSave, " ---> ");
    char help[100];
    sprintf(help, "%d", id);
    strcat(toSave, help);
    strcat(toSave, " ---> ");
    if(type == INIT) strcat(toSave, "INIT");
    else if(type == STOP) strcat(toSave, "STOP");
    else if(type == LIST) strcat(toSave, "LIST");
    else if(type == _2ALL) strcat(toSave, "2ALL");
    else if(type ==_2ONE) strcat(toSave, "2ONE");
    strcat(toSave, " ---> ");
    sprintf(help, "%d", intData);
    strcat(toSave, help);
    strcat(toSave, " ---> ");
    strcat(toSave, text);
    strcat(toSave, "\n");

    //zapis do pliku
    write(fd, toSave, strlen(toSave)*sizeof(char));

}

void handle(int sig_no){
    struct msgbuf com;
    com.mtype = STOP;
    //bede konczyl glowna petle
    loop = 0;
    //koncze wszystkich klientow
    for(int i=0;i<MAX_N_OF_CLIENTS;i++){
        if(msgTab[i] != -1){
            msgsnd(msgTab[i], &com, sizeof(com.id)+sizeof(com.intData)+sizeof(com.charData), 0);   
        }
        
    }

    //czekam az klienci sie pozalatwiaja
    while(1){
        int n = 0;
        for(int i=0;i<MAX_N_OF_CLIENTS;i++){
            if(msgTab[i] != -1){
                n++;
            }
        }
        if(n == 0){
            break;
        }

        int result = msgrcv(msg, &com, sizeof(com.id)+sizeof(com.intData)+sizeof(com.charData), STOP, 0);
        if(result != -1){
            //zapisuje komunikat do pliku
            saveLogs(com.mtype, com.id, com.intData, com.charData);
            msgTab[com.intData] = -1;
        }

    }
    
}




int main(){
    //czyszcze tablice klientow
    for(int i=0;i<MAX_N_OF_CLIENTS;i++){
        msgTab[i] = -1;
    }

    //obsluga SIGINT
    signal(SIGINT, handle);

    //otwieram plik do wpisywania logow
    fd = open("./logs.txt", O_WRONLY);
    if(fd <= 0){
        perror("Blad podczas otwierania pliku!");
        exit(1);
    }


    //tworze klucz
    int key = ftok(getenv("HOME"), SERVERCHAR);

    msg = msgget(key, IPC_CREAT | 0666);
    if(msg == -1){
        perror("Blad podczas tworzenia kolejki!");
        close(fd);
        exit(2);
    }

    

    
    //glowna petla servera
    struct msgbuf com;
    while(loop){
        //czekam na komunikat
        int result = msgrcv(msg, &com, sizeof(com.id)+sizeof(com.intData)+sizeof(com.charData), INT_MIN, 0);
        if(result != -1){
            //zapisuje komunikat do pliku
            saveLogs(com.mtype, com.id, com.intData, com.charData);

            //STOP
            if(com.mtype == STOP){
                msgTab[com.intData] = -1;
            }
            //LIST
            else if(com.mtype == LIST){
                //tworze str z lista
                char* myClients = (char*)calloc(MAXCOMLEN, sizeof(char));
                for(int i=0;i<MAX_N_OF_CLIENTS;i++){
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
                time_t t = time(NULL);
                struct tm tim = *localtime(&t);
                char* date = (char*)calloc(MAXCOMLEN, sizeof(char));
                sprintf(date, "%d:%d:%d", tim.tm_year+1900, tim.tm_mon+1, tim.tm_mday);
                struct msgbuf newMsg;
                newMsg.mtype = MESSAGE;
                newMsg.intData = com.id;
                strcpy(newMsg.charData, date);
                strcat(newMsg.charData, "-->");
                strcat(newMsg.charData, com.charData);

                free(date);

                //wysylam wiadomosc do wszystkich klientow
                for(int i=0;i<MAX_N_OF_CLIENTS;i++){
                    if(i != com.id && msgTab[i] != -1){
                        msgsnd(msgTab[i], &newMsg, sizeof(com.id)+sizeof(com.intData)+sizeof(com.charData), 0);
                    }
                }
            }
            //2ONE
            else if(com.mtype == _2ONE){
                //tworze wiadomosc
                time_t t = time(NULL);
                struct tm tim = *localtime(&t);
                char* date = (char*)calloc(MAXCOMLEN, sizeof(char));
                sprintf(date, "%d:%d:%d", tim.tm_year+1900, tim.tm_mon+1, tim.tm_mday);
                struct msgbuf newMsg;
                newMsg.mtype = MESSAGE;
                newMsg.intData = com.id;
                strcpy(newMsg.charData, date);
                strcat(newMsg.charData, "-->");
                strcat(newMsg.charData, com.charData);

                free(date);

                //wysylam wiadomosc do klienta
                msgsnd(msgTab[com.intData], &newMsg, sizeof(com.id)+sizeof(com.intData)+sizeof(com.charData), 0);   
            }
            //INIT
            else if(com.mtype == INIT){
                //dodaje nowego clienta
                int newID = -1;
                for(int i=0;i<MAX_N_OF_CLIENTS;i++){
                    if(msgTab[i] == -1){
                        newID = i;
                        msgTab[newID] = msgget(com.intData, 0666);
                        break;
                    }
                }
                //robie cos dalej tylko jezeli jest miejsce
                if(newID >= 0){
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
        
    }


    //usuwam kolejke
    msgctl(key, IPC_RMID, NULL);

    //zamykam plik
    close(fd);


    return 0;
}