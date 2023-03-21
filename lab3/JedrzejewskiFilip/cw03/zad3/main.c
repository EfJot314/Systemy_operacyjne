#include<stdio.h>
#include<dirent.h>
#include<sys/stat.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include <fcntl.h>


int browseDirectory(char* directoryPath, char* sample){

    //kopuje directoryPath, bym mogl te pamiec bezstresowo zwolnic w rodzicu
    char* dirPath = (char*)calloc(PATH_MAX, sizeof(char));
    if(dirPath == NULL){
        perror("Blad alokacji pamieci!");
        exit(0);
    }
    strcpy(dirPath, directoryPath);

    int newPID = fork();

    if(newPID < 0){
        perror("Blad podczas tworzenia nowego procesu!");
        exit(0);
    }

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
                    if(newPath == NULL){
                        perror("Blad alokacji pamieci!");
                        free(dirPath);
                        exit(0);
                    }
                    strcpy(newPath, dirPath);
                    strcat(newPath, "/");
                    strcat(newPath, curDirEl->d_name);

                    //jesli jest katalogiem to rekurencyjnie wywoluje te funkcje w nowym procesie
                    if(S_ISDIR(stats.st_mode)){
                        //pomijam katalogi "." i ".."
                        if(strcmp(curDirEl->d_name, ".") != 0 && strcmp(curDirEl->d_name, "..") != 0){
                            
                            //wywolanie rekurencyjne
                            if(browseDirectory(newPath, sample)){
                                //jesli jestem w dziecku to chce wyjsc, ale wczesniej sprzatam po newPath i dirPath
                                free(newPath);
                                free(dirPath);
                                return 1;
                            }

                        }
                    }
                    //jesli nie jest to sprawdzam czy plik rozpoczyna sie od lancucha sample
                    else{
                        //sprawdzam czy moge czytac dany plik
                        if(access(newPath, R_OK) == 0){
                            int open_result = open(newPath, O_RDONLY);
                            if(open_result >= 0){
                                char* readedSample = (char*)calloc(255, sizeof(char));
                                if(readedSample != NULL){
                                    int read_result = read(open_result, readedSample, strlen(sample));
                                    if(read_result >= 0){
                                        //jesli probka dana do programu jest rowna probce pobranej z pliku, to printuje sciezke i PID aktualnego procesu
                                        if(strcmp(readedSample, sample) == 0){
                                            printf("Sciezka: %s, PID: %d\n", newPath, getpid());
                                        }
                                    }
                                    else{
                                        
                                        perror("Blad podczas czytania z pliku!");
                                    }
                                }
                                else{
                                    perror("Blad alokacji pamieci!");
                                    exit(0);
                                }
                                
                                
                            }
                            else{
                                perror("Blad otwierania pliku!");
                            }
                            
                        }
                    }

                    //czyszcze miejsce po newPath
                    free(newPath);
                }
                else{
                    perror("Blad pobierania danych o pliku!");
                }

                //pobieram kolejny element katalogu
                curDirEl = readdir(directory);
            }

            //zamykam katalog
            closedir(directory);
        }
        else{
            perror("Blad otwierania katalogu!");
        }

        //jako dziecko koncze funkcje z 1 (ale wczesniej sprzatam pamiec po dirPath)
        free(dirPath);
        return 1;
    }

    //jako rodzic koncze funkcje z 0 (ale wczesniej sprzatam pamiec po dirPath)
    free(dirPath);
    return 0;
}



int main(int argc, char* argv[]){

    //pobieram dane z wejscia i sprawdzam czy wszystko ok
    if(argc == 3){
        //rezerwuje miejsce
        char* dirPath = (char*)calloc(PATH_MAX, sizeof(char));
        char* sample = (char*)calloc(255, sizeof(char));
        if(dirPath == NULL || sample == NULL){
            perror("Blad alokacji pamieci!");
            exit(0);
        }

        //kopiuje z wejscia
        strcpy(dirPath, argv[1]);
        strcpy(sample, argv[2]);


        //pierwsze wywolanie
        browseDirectory(dirPath, sample);

    }
    else{
        perror("Niepoprawna liczba argumentow!");
    }

    return 0;
}