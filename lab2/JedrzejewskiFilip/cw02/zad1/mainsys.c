#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
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
    

    printf("time: %f\n", realTime);
}




void change(char wanted, char toSwap, char* sourceFile, char* exitFile){
    //otwieram plik source
    int source = open(sourceFile, O_RDONLY);

    if(source > 0){
        //otwieram plik exit
        int exit = open(exitFile, O_WRONLY | O_CREAT);

        if(exit > 0){

            //przepisywanie pliku ze zmianami
            char ch;
            while(1){
                int wsk = read(source, (void*)&ch, 1);
                //wyjscie z petli jak jestem na koncu pliku
                if(wsk == 0){
                    break;
                }
                //zamiana wanted na toSwap
                if(ch == wanted){
                    ch = toSwap;
                }
                //zapis do pliku
                write(exit, (void*)&ch, 1);
            }

            //zamkniecie wpliku wyjsciowego
            close(exit);
        }
        else{
            printf("Blad podczas otwierania/tworzenia pliku wyjsciowego!\n");
        }

        //zamkniecie pliku wejsciowego
        close(source);
    }
    else{
        printf("Blad podczas otwierania pliku wejsciowego!\n");
    }


}


int main(int argc, char* argv[]){

    if(argc == 5){
        //inputy
        char* wanted = argv[1];
        char* toSwap = argv[2];

        char* sourceFile = argv[3];
        char* exitFile = argv[4];

        //czas start
        startTime();

        //wywolanie funkcji, ktora przepisuje pliki ze zmianami
        change(wanted[0], toSwap[0], sourceFile, exitFile);

        //czas stop
        stopTime();
    }
    else{
        printf("Niepoprawna liczba argumentow!\n");
    }
    

    return 0;
}