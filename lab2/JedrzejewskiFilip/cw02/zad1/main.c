#include<stdio.h>
#include<stdlib.h>


void change(char wanted, char toSwap, char* sourceFile, char* exitFile){
    //otwieram plik source
    FILE *source = fopen(sourceFile, "r");

    if(source != NULL){
        //otwieram plik exit
        FILE *exit = fopen(exitFile, "w");

        if(exit != NULL){

            // //przesuwa kursor na koniec pliku fseek(wskaznik na plik, o ile przesunac, wzgledem czego przesunac)
            // fseek(source, 0, SEEK_END);
            // //zwraca aktualna pozycje kursora ftell(wskaznik na plik)
            // int fileSize = ftell(source);
            // //powrot kursorem na poczatek pliku rewind(wskaznik na plik) -> praktycznie rownowazne fseek(file, 0, SEEK_SET)
            // rewind(source);


            //przepisywanie pliku ze zmianami
            char ch;
            while(1){
                ch = fgetc(source);
                //wyjscie z petli jak jestem na koncu pliku
                if(ch == EOF){
                    break;
                }
                //zamiana wanted na toSwap
                if(ch == wanted){
                    ch = toSwap;
                }
                //zapis do pliku
                fprintf(exit, "%c", ch);
            }

            //zamkniecie wpliku wyjsciowego
            fclose(exit);
        }
        else{
            printf("Blad podczas otwierania/tworzenia pliku wyjsciowego!\n");
        }

        //zamkniecie pliku wejsciowego
        fclose(source);
    }
    else{
        printf("Blad podczas otwierania pliku wejsciowego!\n");
    }


}


int main(){
    //maxymalna dlugosc nazwy pliku
    int maxFileNameLen = 100;

    //inputy
    char* wanted = calloc(1, sizeof(char));
    char* toSwap = calloc(1, sizeof(char));

    char* sourceFile = calloc(maxFileNameLen, sizeof(char));
    char* exitFile = calloc(maxFileNameLen, sizeof(char));

    scanf("%s", wanted);
    scanf("%s", toSwap);

    scanf("%s", sourceFile);
    scanf("%s", exitFile);


    change(wanted[0], toSwap[0], sourceFile, exitFile);
    

    return 0;
}