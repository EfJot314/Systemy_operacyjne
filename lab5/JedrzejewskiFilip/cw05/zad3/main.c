#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>


struct timespec realStart, realEnd;

//funkcja obliczajaca roznice czasu w us
double deltaTime(struct timespec t1, struct timespec t2){
    return (double)(t2.tv_sec-t1.tv_sec)*1000000000.0f+(t2.tv_nsec-t1.tv_nsec);
}


//funkcje do zegara
void startTime(){
    //mierzenie czasu poczatkowego
    clock_gettime(CLOCK_REALTIME, &realStart);

}

void stopTime(){
    //mierzenie czasu koncowego
    clock_gettime(CLOCK_REALTIME, &realEnd);

    // obliaczanie roznic czasu
    double realTime = deltaTime(realStart, realEnd);
    
    //wyspisywanie czasu (dziele przez 10^9, aby miec czas w sekundach)
    printf("time: %f\n", realTime/1000000000.0f);
}





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
        startTime();

        //tworze potok nazwany
        char* path = "./potoczek";
        //S_IRWXU - czytanie, pisanie, wykonywanie
        int info = mkfifo(path, 0666);

        if(info != 0){
            perror("Blad podczas tworzenia potoku!");
            exit(1);
        }

        //zmienne pomocnicze
        char *xp = (char*)calloc(22, sizeof(char));
        char *xk = (char*)calloc(22, sizeof(char));
        char *H = (char*)calloc(22, sizeof(char));

        //glowna petla tworzaca potomkow i rozdzielajaca calke na n programow
        double result = 0;
        for(int i=0;i<n;i++){
            //nwm czm musze to tu pisac, ale po setkach prob tylko tak dzialalo, chetnie sie dowiem czm
            char *xp = (char*)calloc(22, sizeof(char));
            char *xk = (char*)calloc(22, sizeof(char));
            char *H = (char*)calloc(22, sizeof(char));

            //tworze potomka
            int newPID = fork();
            //dziecko idzie do programu helper
            if(newPID == 0){
                snprintf(xp, 22, "%1.20f", x_min);
                snprintf(xk, 22, "%1.20f", x_min+dx);
                snprintf(H, 22, "%1.20f", h);

                char* argv[] = {path, xp, xk, H};

                execv("./helper", argv);
                perror("Blad podczas uruchamiania programu helper!");
            }

            //rodzic slucha pipe i czeka na komunikat od dzieciaka
            else{
                //otwieram pipe
                int res = open(path, O_RDONLY);
                if(res > 0){
                    
                    //czytam pojedynczy odczyt od dzieciaka
                    double single_result;
                    read(res, &single_result, sizeof(double));

                    //dodaje do wyniku calkowitego
                    result += single_result;

                    //zamykam pipe
                    close(res);

                }
                else{
                    perror("Blad podczas otwierania potoku - main!");
                    exit(2);
                }
            }

            //zwalniam
            free(xp);
            free(xk);
            free(H);

            //przechodze do kolejnego przedzialu
            x_min += dx;

            
            
        }
        
        //usuwam potok
        remove(path);

        //print wyniku
        printf("%f\n", result);

        //czas stop
        stopTime();

    }
    else{
        perror("Niepoprawna liczba argumentow - main!");
    }


    return 0;
}





