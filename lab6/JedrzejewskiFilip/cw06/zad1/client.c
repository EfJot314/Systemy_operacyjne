#include<stdio.h>
#include<unistd.h>
#include<limits.h>
#include<sys/msg.h>
#include<stdlib.h>
#include<string.h>

#include"constData.h"



int loop = 1;


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
}


int main(){
    //kolejka serwera
    int key = ftok(getenv("HOME"), SERVERCHAR);
    int msg = msgget(key, 0666);
    if(msg == -1){
        perror("Blad podczas otwierania kolejki!");
        exit(1);
    }

    
    //kolejka klienta
    int myKey = ftok(getenv("HOME"), getpid()%100);
    int myMsg = msgget(myKey, IPC_CREAT | 0666);

    //id klienta
    int myId = 0;

    printf("%d\n", myKey);

    //init do servera
    sendMsg(msg, myId, INIT, myKey, "");

    //zbieram odeslana wiadomosc z id
    struct msgbuf mes;
    msgrcv(myMsg, &mes, sizeof(mes.id)+sizeof(mes.intData)+sizeof(mes.charData), INIT, 0);
    myId = mes.intData;

    

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
    
    


    //usuwam kolejke
    msgctl(myKey, IPC_RMID, NULL);




    return 0;
}