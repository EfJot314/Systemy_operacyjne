#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<ftw.h>
#include<libgen.h>

long long sumSize = 0;

int collectData(const char * fpath , const struct stat * stats , int typeflag ){
    //sprawdzam czy jest katalogiem czy nie
    if(!S_ISDIR(stats->st_mode)){
        //jesli nie jest to zwiekszam calkowity rozmiar wszytskich plikow i wypisuje dane aktualnego
        sumSize += stats->st_size;
        printf("Nazwa: %s\t, rozmiar: %lld\n", basename(fpath), stats->st_size);
    }

    return 0;
}



int main(int argc, char* argv[]){
    
    if(argc == 2){
        //pobieranie danych z wejscia
        char* dirPath = argv[1];

        //przegladanie plikow w danym katalogu i jego podkatalogach i ew zglaszanie problemow
        int result = ftw(dirPath, collectData, 100);
        if(result == 0){
            //wypisywanie calkowitego rozmiaru plikow w tym katalogu i jego podkatalogach
            printf("Calkowity rozmiar plikow w tym katalogu i jego podkatalogach: %lld\n\n", sumSize);
        }
        else{
            printf("Blad podczas przegladania plikow!\n");
        }

    }   
    else{
        printf("Niepoprawna liczba argumentow!\n");
    }



    return 0;
}