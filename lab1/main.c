#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <sys/times.h>



//zmienne do mierzenia czasu
static struct tms startTms;
static struct tms endTms;
static clock_t realStart;
static clock_t realStop;

//wskaznik na parray
struct parray *pStruct = NULL;


//funkcje do zegara
void startTime(){
    //mierzenie czasu poczatkowego
    realStart = times(&startTms);
}

void stopTime(){
    //mierzenie czasu koncowego
    realStop = times(&endTms);

    //obliaczanie roznic czasu
    double realTime = (double)(realStop-realStart);
    double usrTime = (double)(endTms.tms_utime-startTms.tms_utime);
    double sysTime = (double)(endTms.tms_stime-startTms.tms_stime);
    printf("real: %f, usr: %f, sys: %f \n", realTime, usrTime, sysTime);
}


int parseInput(char* command){
    char white[] = " \t\n";
    char* curWord = strtok(command, white);
    //wyjscie z programu
    if(strcmp(curWord, "quit") == 0){
        return 0;
    }
    //init size
    if(strcmp(curWord, "init") == 0){
        curWord = strtok(NULL, white);
        int maxSize = atoi(curWord);
        //wywolanie funkcji
        pStruct = createStructure(maxSize);
        return 1;
    }
    //count file
    if(strcmp(curWord, "count") == 0){
        curWord = strtok(NULL, white);
        if(pStruct != NULL){
            //wywolanie funkcji
            countFile(pStruct, curWord);
        }
        else{
            printf("Nie zainicjowano struktury!\n");
        }
        return 2;
    }
    //show index
    if(strcmp(curWord, "show") == 0){
        curWord = strtok(NULL, white);
        int ind = atoi(curWord);
        if(pStruct != NULL){
            //wywolanie funkcji
            char* result = getBlock(pStruct, ind);

            if(result != NULL){
                printf("%s", result);
                free(result);
            }
            else{
                printf("Podany indeks nie jest zapisany!\n");
            }
        }
        else{
            printf("Nie zainicjowano struktury!\n");
        }
        return 3;
    }
    //delete index index
    if(strcmp(curWord, "delete") == 0){
        curWord = strtok(NULL, white);
        if(strcmp(curWord, "index") == 0){
            curWord = strtok(NULL, white);
            int ind = atoi(curWord);
            if(pStruct != NULL){
                freeBlock(pStruct, ind);
            }
            else{
                printf("Nie zainicjowano struktury!\n");
            }
        }
        
        return 4;
    }
    //destroy
    if(strcmp(curWord, "destroy") == 0){
        if(pStruct != NULL){
            freeAllArray(pStruct);
            free(pStruct);
            pStruct = NULL;
        }
        else{
            printf("Nie zainicjowano struktury!\n");
        }
        return 5;
    }

    //jezeli polecenie nie zostalo rozpoznane / jest niepoprawne
    printf("Nie rozpoznano polecenia: ");
    printf("%s\n", command);
    return -1;
    
}

int main(){
    int maxSizeOfCommand = 200;
    char currCom[maxSizeOfCommand];
    while(1){
        printf(">>> ");
        fgets(currCom, maxSizeOfCommand, stdin);

        
        startTime();

        //wywolanie funkcji
        int result =  parseInput(currCom);

        stopTime();


        if(result == 0){
            return 0;
        }
    }


    return 0;
}   

