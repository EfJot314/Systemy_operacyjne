#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>



int main(int argc, char* argv[]){

    //sprawdzam czy liczba argumentow sie zgadza
    if(argc == 3){
        //zbieram dane z wejscia i sprawdzam czy sa poprawne
        double h = strtod(argv[1], NULL);
        if(h == 0){
            perror("Niepoprany pierwszy argument!");
            exit(1);
        }
        int n = atoi(argv[2]);
        if(n == 0){
            perror("Niepoprany drugi argument!");
            exit(2);
        }

        //granice przedzialu
        double x_min = 0;
        double x_max = 1;

        //obliczam szerokosc przedzialu ktory otrzyma do obliczenia kazdy z procesow potomnych
        double dx = (x_max - x_min) / n;

        //wlaczam zegar
        // startTime();

        //tworze potok nazwany
        const char* path = "potoczek";
        //S_IRWXU - czytanie, pisanie, wykonywanie
        int kkk = mkfifo(path, S_IRWXU);

        for(int i=0;i<n;i++){
            int res = open(path, O_RDONLY);
            if(res > 0){
                printf("sup\n");
            }
            else{
                printf("ups\n");
            }
        }

        //usuwam potok
        remove(path);

        //czas stop
        // stopTime();

    }
    else{
        perror("Niepoprawna liczba argumentow!");
    }


    return 0;
}





