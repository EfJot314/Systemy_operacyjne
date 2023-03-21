#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>


int browseDirectory(char* dirPath, char* sample){

    printf("%s\n", dirPath);

    int newPID = fork();

    //robie cokolwiek tylko jesli jestem w dziecku
    if(newPID == 0){
        //otwieram i sprawdzma czy wszystko dobrze poszlo
        DIR* directory = opendir(dirPath);

        if(directory != NULL){
            //aktualny element w katalogu
            struct dirent *curDirEl = readdir(directory);

            //iterowanie po plikach
            while(curDirEl != NULL){
                //pobieranie danych i sprawdzanie czy wszystko dobrze poszlo
                struct stat stats;
                int result = stat(curDirEl->d_name, &stats);
                result = 0;
                if(result == 0){

                    //tworze sciezke do danego elementu katalogu
                    char* newPath = (char*)calloc(PATH_MAX, sizeof(char));
                    strcpy(newPath, dirPath);
                    strcat(newPath, "/");
                    strcat(newPath, curDirEl->d_name);

                    //jesli jest katalogiem to rekurencyjnie wywoluje te funkcje w nowym procesie
                    if(S_ISDIR(stats.st_mode)){
                        //pomijam katalogi "." i ".."
                        if(strcmp(curDirEl->d_name, ".") != 0 && strcmp(curDirEl->d_name, "..") != 0){
                            
                            
                            //wywolanie rekurencyjne
                            if(browseDirectory(newPath, sample)){
                                //jesli jestem w dziecku to chce wyjsc
                                return 1;
                            }

                        }
                    }
                    //jesli nie jest to sprawdzam czy plik rozpoczyna sie od lancucha sample
                    else{
                        if(access(newPath, R_OK) == 0){
                            printf("plik %s\n", curDirEl->d_name);
                        }
                    }
                }
                else{
                    printf("Blad pobierania danych o pliku!\n");
                }

                //pobieram kolejny element katalogu
                curDirEl = readdir(directory);
            }

            //zamykam
            closedir(directory);
        }
        else{
            printf("Blad otwierania katalogu %s !\n", dirPath);
        }

        //jako dziecko koncze funkcje z 1
        return 1;
    }

    //jako rodzic koncze funkcje z 0
    return 0;
}



int main(int argc, char* argv[]){

    //pobieram dane z wejscia i sprawdzam czy wszystko ok
    if(argc == 3){
        //rezerwuje miejsce
        char* dirPath = (char*)calloc(PATH_MAX, sizeof(char));
        char* sample = (char*)calloc(255, sizeof(char));

        //kopiuje z wejscia
        strcpy(dirPath, argv[1]);
        strcpy(sample, argv[2]);


        //pierwsze wywolanie
        browseDirectory(dirPath, sample);


    }
    else{
        printf("Niepoprawna liczba argumentow!\n");
    }


    return 0;
}