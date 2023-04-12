#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<time.h>


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


//dana w zadaniu funkcja f(x)
double f(double x){
    return 4/(x*x+1);
}

//funkcja calkujaca metoda prostokatow
double integrate(double xp, double xk, double h){
    //zmienne pomocnicze
    double result = 0;
    double x = xp;
    //petla iterujaca po x
    while(x < xk){
        //skracam ostatni przedzial
        if(x+h > xk){
            h = xk-x;
        }
        //dodaje prostokat do wyniku
        result += h*f(x);

        //zwiekszam x
        x += h;
    }
    //zwrot wyniku
    return result;
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

        //tablica deskryptorow dla rodzica
        int pd[n];

        //wlaczam zegar
        startTime();

        //tworze potomkow i przypisuje im zadania
        for(int i=0;i<n;i++){
            //tworze pipe
            int fd[2];
            pipe(fd);

            //tworze potomka
            int newPID = fork();

            //jesli jestem w dziecku
            if(newPID == 0){
                //zamykam wyjscie dla dziecka
                close(fd[0]);

                //licze caleczke na dedykowanym dla mnie przedziale
                double resultForParent = integrate(x_min+i*dx, x_min+(i+1)*dx, h);

                //wysylam wynik do rodzica
                write(fd[1], &resultForParent, sizeof(double));

                //wychodze z dziecka
                return 0;
            }
            //jesli jestem w rodzicu
            else{
                //zamykam wejscie dla rodzica
                close(fd[1]);

                //daje rodzicowi namiary na dzieciaka
                pd[i] = fd[0];
            }
        }

        //sumuje wyniki od potomkow
        double result = 0;
        for(int i=0;i<n;i++){
            double singleResult = 0;
            read(pd[i], &singleResult, sizeof(double));
            result += singleResult;
        }

        //wypisanie wyniku
        printf("%f\n", result);

        //czas stop
        stopTime();

    }
    else{
        perror("Niepoprawna liczba argumentow!");
    }


    return 0;
}