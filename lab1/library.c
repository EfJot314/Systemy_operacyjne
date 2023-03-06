#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct parray
{
    int size;
    int actSize;
    void* tab[];
};


struct parray* createStructure(int maxSize){
    void* tab[maxSize];
    struct parray *p1;
    p1 = malloc(sizeof(struct parray));
    *p1->tab = (void*)calloc(maxSize, sizeof(void*));
    p1->size = maxSize;
    p1->actSize = 0;

    printf("struktura zostala stworzona\n");
    
    return p1;
}


void countFile(struct parray* pStruct, char *fileName){

    fileName = "./testowe";

    char* tempName = NULL;

    tempName = tmpnam(NULL);

    printf(tempName);

    printf("\n");

    

    if(tempName != NULL){
        char* command;

        int leng = strlen("wc ")+strlen(tempName)+strlen(" > ")+strlen(fileName);
        printf("%d\n", leng);

        command = malloc(leng);

        strcat(command, "wc ");
        strcat(command, fileName);
        strcat(command, " > ");
        strcat(command, tempName);

        printf("%s", command);
        printf("\n");

        
        system(command);


        FILE* file = fopen(tempName, "r");


        char* block = calloc(1, sizeof("kurwaaa"));

        // strcpy(block, file);
        block = "kurwaaa";

        int size = pStruct->actSize;
        *(pStruct->tab+size) = (void*)block;

        //inkrementacja licznika
        pStruct->actSize = size+1;
        

        fclose(file);


        //wypisywanie zawartosci pliku
        // char ch = "";
        // while(1){
        //     ch = fgetc(file);
        //     if(ch == EOF){
        //         break;
        //     }
        //     printf("%c", ch);
        // }


    }



}

char* showBlock(struct parray *pStruct, int ind){
    char* result;
    result = (char*)*(pStruct->tab+ind);
    char* toReturn = malloc(strlen(result)*sizeof(char));
    strcpy(toReturn, result);
    return toReturn;
}



