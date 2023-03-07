#include<stdio.h>
#include<stdlib.h>
#include <string.h>


struct parray *pStruct = NULL;


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
        pStruct = createStructure(maxSize);
        return 1;
    }
    //count file
    if(strcmp(curWord, "count") == 0){
        curWord = strtok(NULL, white);
        if(pStruct != NULL){
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

        int result =  parseInput(currCom);

        if(result == 0){
            return 0;
        }
    }


    return 0;
}   

