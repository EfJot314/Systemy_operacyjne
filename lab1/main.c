#include<stdio.h>
#include<stdlib.h>
#include <string.h>


struct parray *pStruct;


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

