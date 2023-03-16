#include<stdio.h>





int main(int argn, char* argv[]){
    //pobieram dane wejsciowe i sprawdzam czy wszystko z nimi ok
    if(argn == 2){
        char* path = argv[1];

        //print nazwy programu
        //TODO
        printf("XD ");

        //wymuszam brak buforowania danych
        setbuf(stdout, NULL);

        //wywoluje ls dla danej sciezki
        execl("/bin/ls", "ls", path, NULL);


    }
    else{
        printf("Niepoprawna liczba argumentow!\n");
    }


    return 0;
}