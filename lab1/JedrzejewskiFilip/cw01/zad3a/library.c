#include<stdio.h>
#include<stdlib.h>
#include <string.h>

struct parray
{
    int size;
    int actSize;
    void** tab;
};


struct parray* createStructure(int maxSize){
    struct parray *p1;
    p1 = (struct parray*)malloc(sizeof(struct parray));
    if(p1 != NULL){
        p1->tab = (void*)calloc(maxSize, sizeof(void*));
        p1->size = maxSize;
        p1->actSize = 0;

        printf("Struktura zostala pomyslnie zainicjowana.\n");

        return p1;
    }

    printf("Blad alokacji pamieci.\n");

    return NULL;
    
}


void countFile(struct parray* pStruct, char *fileName){


    //zczytywanie aktualnej dlugosci ze struktury
    int size = pStruct->actSize;
    int maxSize = pStruct->size;

    if(size < maxSize){
        //tworzenie pliku tmp
        char* tempName = tmpnam(NULL);

        //jesli plik zostal stworzony
        if(tempName != NULL){
            
            //wyznaczanie dlugosci polecenia
            int leng = strlen("wc ")+strlen(tempName)+strlen(" > ")+strlen(fileName);

            //tworzenie polecenia
            char* command = calloc(leng, sizeof(char));
            if(command != NULL){
                strcat(command, "wc ");
                strcat(command, fileName);
                strcat(command, " > ");
                strcat(command, tempName);
                
                //wywolywanie polecenia
                int result = system(command);

                //usuwanie polecenia z pamieci
                free(command);

                //jesli polecenie wykonalo sie poprawnie
                if(result == 0){

                    //otwieranie pliku tmp
                    FILE* file = fopen(tempName, "r");
                    if(file != NULL){
                        //przesuwa kursor na koniec pliku fseek(wskaznik na plik, o ile przesunac, wzgledem czego przesunac)
                        fseek(file, 0, SEEK_END);
                        //zwraca aktualna pozycje kursora ftell(wskaznik na plik)
                        int fileSize = ftell(file);
                        //powrot kursorem na poczatek pliku rewind(wskaznik na plik) -> praktycznie rownowazne fseek(file, 0, SEEK_SET)
                        rewind(file);


                        //wpisywanie zawartosci pliku do bloku pamieci
                        char* block = calloc(1, fileSize*sizeof(char));
                        char ch = "";
                        for(int i=0;i<fileSize;i++){
                            ch = fgetc(file);
                            *(block+i) = ch;
                        }

                        //ustawianie wskaznika w tablicy na wlasnie stworzony blok pamieci
                        pStruct->tab[size] = (void*)block;

                        //inkrementacja licznika
                        pStruct->actSize = size+1;
                        
                        //zamykanie pliku
                        fclose(file);
                    }
                    else{
                        printf("Blad otwierania pliku.\n");
                    }
                }
                else{
                    printf("Niepoprawne polecenie!\n");
                }

                

                
            }
            else{
                printf("Blad alokacji pamieci.\n");
            }

        
            //usuwanie pliku
            remove(tempName);

        }
        else{
            printf("Stworznie pliku tymczasowego sie nie powiodlo.\n");
        }
    }
    else{
        printf("Brak miejsca w tablicy!\n");
    }

    
}

char* getBlock(struct parray *pStruct, int ind){
    //pobieranie aktualnej dlugosci tablicy
    int size = pStruct->actSize;
    //jezeli indeks znajduje sie we wlasciwym przedziale to zwracam wartosc bloku
    if(ind >= 0 && ind < size){
        char* result = (char*)pStruct->tab[ind];
        char* toReturn = calloc(1,strlen(result)*sizeof(char));
        strcpy(toReturn, result);
        return toReturn;
    }
    //jezeli jest cos nie tak, to zwracam NULL
    else{
        return NULL;
    }
    
}


void freeBlock(struct parray *pStruct, int ind){
    //pobranie aktualnej dlugosci tablicy
    int size = pStruct->actSize;
    if(ind >= 0 && ind < size){
        //pobranie wskaznika
        void* ptr = pStruct->tab[ind];
        //zwolnienie pamieci
        free(ptr);
        //przesuniecie kolejnych wskaznikow jesli nie jestem na koncy tablicy
        if(size > ind+1){
            pStruct->tab[ind] = (void*)getBlock(pStruct, ind+1);
            freeBlock(pStruct, ind+1);
        }
        else{
            //jezeli jestem na koncu tablicy i juz nie ma nic do przepisywania, to zmniejsza licznik aktualnie zajetych blokow
            pStruct->actSize = size-1;
            printf("Pomyslnie usunieto blok pamieci.\n");
        }
    }
    else{
        printf("Niepoprawna wartosc indeksu do usuniecia.\n");
    }
    
}


void freeAllArray(struct parray *pStruct){
    int size = pStruct->actSize;
    //ide od tylu zeby nie wywolywac rekurencji z funkcji freeBlock()
    for(int i=size-1; i>=0; i--){
        freeBlock(pStruct, i);
    }
    //zwalnianie pamieci juz pustej tablicy
    free(pStruct->tab);
}



