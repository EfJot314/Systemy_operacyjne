#include<stdio.h>
#include<stdlib.h>
#include <string.h>


struct parray *pStruct = NULL;


int whatIsIt(char* command){
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
    
}

int main(){
    int maxSizeOfCommand = 200;
    char currCom[maxSizeOfCommand];
    while(1){
        printf(">>> ");
        fgets(currCom, maxSizeOfCommand, stdin);

        int result =  whatIsIt(currCom);

        if(result == 0){
            return 0;
        }
    }


    return 0;
}   

