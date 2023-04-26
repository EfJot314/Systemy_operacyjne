#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<string.h>
#include<signal.h>

#include"constData.h"



int loop = 1;
int myId = 0;
int msg;


void sendMsg(int rec, int id, int type, int intData, char* charData){
    //tworze wiadomosc
    struct msgbuf newMsg;
    newMsg.mtype = type;
    newMsg.id = id;
    newMsg.intData = intData;
    strcpy(newMsg.charData, charData);

    //wysylam wiadomosc
    msgsnd(rec, &newMsg, sizeof(newMsg.id)+sizeof(newMsg.intData)+sizeof(newMsg.charData), 0);
}


void parseAndSend(char* com, int id, int rec){
    //biale znaki
    const char delim[]=" \t\n\r";
    //tne
    char* curr = strtok(com, delim);
    //STOP
    if(strcmp(curr, "STOP") == 0){
        curr = strtok(NULL, delim);
        //sprawdzam czy tylko to slowo zostalo wprowadzone
        if(curr == NULL){
            loop = 0;
            sendMsg(rec, id, STOP, id, "");
        }
        else{
            perror("Nie rozpoznano polecenia.");
        }
    }
    //LIST
    else if(strcmp(curr, "LIST") == 0){
        curr = strtok(NULL, delim);
        //sprawdzam czy tylko to slowo zostalo wprowadzone
        if(curr == NULL){
            sendMsg(rec, id, LIST, 0, "");
        }
        else{
            perror("Nie rozpoznano polecenia.");
        }
    }
    //2ALL
    else if(strcmp(curr, "2ALL") == 0){
        curr = strtok(NULL, "");
        //sprawdzam czy tylko to slowo zostalo wprowadzone
        if(curr != NULL){
            sendMsg(rec, id, _2ALL, 0, curr);
        }
        else{
            perror("Nie rozpoznano polecenia.");
        }
    }
    //2ONE
    else if(strcmp(curr, "2ONE") == 0){
        curr = strtok(NULL, delim);
        int recID = atoi(curr);
        curr = strtok(NULL, "");
        //sprawdzam czy tylko to slowo zostalo wprowadzone
        if(curr != NULL){
            sendMsg(rec, id, _2ONE, recID, curr);
        }
        else{
            perror("Nie rozpoznano polecenia.");
        }
    }
}

void handle(int sig_no){
    //tak jak STOP
    loop = 0;
    sendMsg(msg, myId, STOP, myId, "");
    //zabijam czytacza
    kill(0, SIGKILL);
}



int main(){
    
    //obluga sygnalu SIGINT
    signal(SIGINT, handle);

    //kolejka serwera
    int key = ftok(getenv("HOME"), SERVERCHAR);
    msg = msgget(key, 0666);
    if(msg == -1){
        perror("Blad podczas otwierania kolejki!");
        exit(1);
    }

    
    //kolejka klienta
    int myKey = ftok(getenv("HOME"), getpid()%100);
    int myMsg = msgget(myKey, IPC_CREAT | 0666);

    //init do servera
    sendMsg(msg, myId, INIT, myKey, "");

    //zbieram odeslana wiadomosc z id
    struct msgbuf mes;
    msgrcv(myMsg, &mes, sizeof(mes.id)+sizeof(mes.intData)+sizeof(mes.charData), INIT, 0);
    myId = mes.intData;


    int newPID = fork();
    //dziecko - odbieranie komunikatow
    if(newPID == 0){
        while(loop){
            //odbieram ew wiadomosci
            struct msgbuf mesg;
            msgrcv(myMsg, &mesg, sizeof(mesg.id)+sizeof(mesg.intData)+sizeof(mesg.charData), 0, 0);
            //jesli odebralem wiadomosc to ja printuje
            if(mesg.mtype == MESSAGE){
                printf("%d-->%s", mesg.intData, mesg.charData);
            }
            else if(mesg.mtype == LIST){
                printf("%s\n", mesg.charData);
            }
        }
        
    }
    //rodzic - wysylanie komunikatow
    else{
        
        //glowna petla
        char* order = (char*)calloc(MAXCOMLEN, sizeof(char));
        size_t size;
        while(loop)
        {
            //czytam linie
            getline(&order, &size, stdin);

            //robie to co mam robic
            parseAndSend(order, myId, msg);

        }

        free(order);

        //zabijam czytacza
        kill(newPID, SIGKILL);

        //usuwam kolejke
        msgctl(myKey, IPC_RMID, NULL);
    }


    

    return 0;
}