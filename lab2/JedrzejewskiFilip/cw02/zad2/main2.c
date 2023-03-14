#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#include<string.h>

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


char* reverseString(char* original, int n){
    char* reversedString = (char*)malloc(n*sizeof(char));
    for(int i=0;i<n;i++){
        reversedString[i] = original[n-i-1];
    }
    return reversedString;
}


int main(int argc, char* argv[]){
    //czas start
    startTime();

    if(argc == 3){
        //pobieram dane z wejscia
        char* sourceFile = argv[1];
        char* exitFile = argv[2];

        //otwieram plik zrodlowy i sprawdzam czy wszystko poszlo ok
        FILE *source = fopen(sourceFile, "r");

        if(source != NULL){
            //otwieram/tworze plik wyjsciowy i sprawdzam czy wszystko poszlo ok
            FILE *exit = fopen(exitFile, "w");

            if(exit != NULL){
                
                //liczba bajtow ktore pobieram na raz
                int N = 1024;

                int counter = 0;
                char ch[N];
                if(ch != NULL){
                    while(1){
                        //czytanie po 1024 znakach z pliku
                        counter++;
                        int wsk1 = fseek(source, -counter*N, SEEK_END);
                        //magia ktora po prostu pobiera tyle znakow ile sie da, a nie wiecej
                        int i = N;
                        while(wsk1 != 0){
                            i--;
                            if(i <= 0){
                                break;
                            }
                            wsk1 = fseek(source, -(counter-1)*N-i, SEEK_END);
                        }
                        int wsk2 = fread((void*)ch, sizeof(char), i, source);
                        
                        //jezeli nic nie przeczytalem to wychodze
                        if(wsk2 == 0 || wsk1 != 0){
                            break;
                        }

                        //obracam stringa
                        char* reversed = reverseString(ch, i);
                        strcpy(ch, reversed);
                        free(reversed);

                        //zapis do pliku wyjsciowego
                        fwrite((void*)ch, sizeof(char), i, exit);
                        
                    }
                }
                else{
                    printf("Blad alokacji pamieci!\n");
                }

                //zamykam wyjscie
                fclose(exit);
                
            }
            else{
                printf("Blad otwierania/tworzenia pliku wyjsciowego\n");
            }

            //zamykam zrodlo
            fclose(source);


        }
        else{
            printf("Blad otwierania pliku wejsciowego\n");
        }   

    }
    else{
        printf("Niepoprawna liczba argumentow!\n");
    }

    //czas stop
    stopTime();



    return 0;
}
