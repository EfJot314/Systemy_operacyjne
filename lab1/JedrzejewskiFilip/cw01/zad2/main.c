#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include <sys/times.h>
#include <sys/resource.h>


//zmienne do mierzenia czasu
struct timeval userValStart, userValEnd;
struct timeval systemValStart, systemValEnd;
struct timespec userStart, userEnd;
struct timespec systemStart, systemEnd;
struct timespec realStart, realEnd;
struct rusage usage;

// static struct tms startTms;
// static struct tms endTms;
// static clock_t realStart;
// static clock_t realStop;


//wskaznik na parray
struct parray *pStruct = NULL;


//funkcja obliczajaca roznice czasu w us
double deltaTime(struct timespec t1, struct timespec t2){
    return (double)(t2.tv_sec-t1.tv_sec)*1000000000.0f+(t2.tv_nsec-t1.tv_nsec);
}


//funkcje do zegara
void startTime(){
    //mierzenie czasu poczatkowego
    clock_gettime(CLOCK_REALTIME, &realStart);
    getrusage(RUSAGE_SELF, &usage);
    userValStart = usage.ru_utime;
    systemValStart = usage.ru_stime;

    // realStart = times(&startTms);

    
}

void stopTime(){
    //mierzenie czasu koncowego
    getrusage(RUSAGE_SELF, &usage);
    userValEnd = usage.ru_utime;
    systemValEnd = usage.ru_stime;
    clock_gettime(CLOCK_REALTIME, &realEnd);



    systemStart.tv_sec = systemValStart.tv_sec;
    systemStart.tv_nsec = systemValStart.tv_usec*1000;
    systemEnd.tv_sec = systemValEnd.tv_sec;
    systemEnd.tv_nsec = systemValEnd.tv_usec*1000;

    userStart.tv_sec = userValStart.tv_sec;
    userStart.tv_nsec = userValStart.tv_usec*1000;
    userEnd.tv_sec = userValEnd.tv_sec;
    userEnd.tv_nsec = userValEnd.tv_usec*1000;

    // obliaczanie roznic czasu
    double realTime = deltaTime(realStart, realEnd);
    double usrTime = deltaTime(userStart, userEnd);
    double sysTime = deltaTime(systemStart, systemEnd);
    


    // realStop = times(&endTms);
    // double realTime = (double)(realStop-realStart);
    // double usrTime = (double)(endTms.tms_utime-startTms.tms_utime);
    // double sysTime = (double)(endTms.tms_stime-startTms.tms_stime);

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
        if(pStruct == NULL){
            int maxSize = atoi(curWord);
            //wywolanie funkcji
            pStruct = createStructure(maxSize);
            return 1;
        }
        printf("Struktura zostala juz zainicjalizowana!\n");
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
            break;
        }
    }

    return 0;
}   

