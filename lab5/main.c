#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <fcntl.h>








int main(){

    //otwieram plik
    int file = open("./out.txt", O_RDONLY);

    //przekierowuje input z stdin na plik
    dup2(file, 0);

    int wynik;
    scanf("%d", &wynik);

    //zamykam plik
    close(file);


    printf("%d\n", wynik);

    return 0;

}