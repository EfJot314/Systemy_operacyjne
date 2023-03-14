#include<stdio.h>
#include<dirent.h>
#include <sys/stat.h>




int main(){

    //otwieram i sprawdzma czy wszystko dobrze poszlo
    DIR* directory = opendir("./");

    if(directory != NULL){
        //potrzebne zmienne
        long long sumSize = 0;
        struct dirent *curDirEl = readdir(directory);

        //iterowanie po plikach
        while(curDirEl != NULL){
            //pobieranie statystyk i sprawdzanie czy wszystko dobrze poszlo
            struct stat stats;
            int result = stat(curDirEl->d_name, &stats);
            if(result == 0){
                //uwzgledniam tylko gdy nie jest katalogiem
                if( !S_ISDIR(stats.st_mode)){
                    //dodaje rozmiar aktulanego pliku do sumy rozmiarow
                    sumSize += stats.st_size;
                    //wypisuje nazwe i rozmiar
                    printf("Nazwa: %s\t, rozmiar: %lld\n", curDirEl->d_name, stats.st_size);
                }
            }
            else{
                printf("Blad pobierania danych o pliku!\n");
            }

            curDirEl = readdir(directory);
        }

        //wypisanie wartosci rozmiaru wszytskich plikow
        printf("Sumaryczny rozmiar plikow w tym katalogu: %lld\n\n", sumSize);

        //zamykam
        closedir(directory);
    }
    else{
        printf("Blad otwierania katalogu!\n");
    }


    return 0;
}