#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>



//dana w zadaniu funkcja f(x)
double f(double x){
    return 4/(x*x+1);
}




int main(int argc, char* argv[]){
    if(argc == 4){
        //pobieram dane z wejscia, nie sprawdzam czy sa poprawne, bo po co, skoro bede ten program wywolywal jedynie z main
        char* path = argv[0];
        double xp = strtod(argv[1], NULL);
        double xk = strtod(argv[2], NULL);
        double h = strtod(argv[3], NULL);

        //glowna petla liczaca calke
        double result = 0;
        while(xp < xk){
            //poprawka na ostatni prostakat
            if(xp+h > xk){
                h = xk - xp;
            }

            //dodaje prostokat do wyniku
            result += h*f(xp);

            //zwiekszenie xp
            xp += h;
        }

        //zapis do potoku
        int res = open(path, O_WRONLY);
        if(res > 0){
            write(res, &result, sizeof(double));

            close(res);
        }
        else{
            perror("Blad podczas otwierania potoku - helper!");
            exit(2);
        }


    }
    else{
        perror("Niepoprawna liczba argumentow - helper!");
    }
    



    return 0;
}


