#include<stdio.h>



int main(){
    struct parray *pStruct = createStructure(10);
    countFile(pStruct, "./testowe");
    countFile(pStruct, "./wynik");
    countFile(pStruct, "./wynik");
    countFile(pStruct, "./testowe");
    printf("%s\n", getBlock(pStruct,0));
    printf("%s\n", getBlock(pStruct,1));
    printf("%s\n", getBlock(pStruct,2));
    printf("%s\n", getBlock(pStruct,3));

    freeBlock(pStruct, 2);

    printf("Po usunieciu elementu o indekcie 2:\n");


    printf("%s\n", getBlock(pStruct,0));
    printf("%s\n", getBlock(pStruct,1));
    printf("%s\n", getBlock(pStruct,2));
    printf("%s\n", getBlock(pStruct,3));

    printf("Dodaje kolejne 4 elementy:\n");

    countFile(pStruct, "./testowe");
    countFile(pStruct, "./wynik");
    countFile(pStruct, "./wynik");
    countFile(pStruct, "./testowe");

    printf("%s\n", getBlock(pStruct,0));
    printf("%s\n", getBlock(pStruct,1));
    printf("%s\n", getBlock(pStruct,2));
    printf("%s\n", getBlock(pStruct,3));
    printf("%s\n", getBlock(pStruct,4));
    printf("%s\n", getBlock(pStruct,5));
    printf("%s\n", getBlock(pStruct,6));

    printf("Usuwam calosc tablicy: \n");

    freeAllArray(pStruct);

    printf("%s\n", getBlock(pStruct,0));
    printf("%s\n", getBlock(pStruct,1));
    printf("%s\n", getBlock(pStruct,2));
    printf("%s\n", getBlock(pStruct,3));
    printf("%s\n", getBlock(pStruct,4));
    printf("%s\n", getBlock(pStruct,5));
    printf("%s\n", getBlock(pStruct,6));




    return 0;
}   

