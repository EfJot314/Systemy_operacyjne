#include<stdio.h>



int main(){
    struct parray *pStruct = createStructure(10);
    countFile(pStruct, "./testowe");
    printf("%s\n", showBlock(pStruct,0));

    return 0;
}   