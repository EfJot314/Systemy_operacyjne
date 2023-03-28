#include<stdio.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

//zmienna obslugujaca nieskonczona petle utrzymujaca program przy zyciu
int stoper = 1;
//zmienna przechowujaca PID procesu wyspisujacego czas
int timePID = 0;
//zmienna czasu
time_t t;
//zmienna przechowujaca liczbe zmian trybu pracy
int nOfChanges = 0;


//funkcja wykonuje dzialania ktore powinny byc wykonywane na poczatku kazdego trybu pracy
void tribeInit(){
    //inkrementuje licznik zmian trybu pracy
    nOfChanges++;
    //jesli jest wlaczony tryb 4 to go koncze wysylajac sygnal SIGINT (Ctrl+C)
    if(timePID != 0){
        kill(timePID, SIGINT);
        timePID = 0;
    }
}


//funkcja wysylajaca sygnal SIGUSR1 z wartocia value do procesu o podanym PID
void sendSignal(int PID, int value){
    union sigval ssv;
    ssv.sival_int = value;
    //wysylam sygnal i sprawdzam czy nie bylo z tym problemow
    int result = sigqueue(PID, SIGUSR1, ssv);
    if(result < 0){
        perror("Sygnal potwierdzajacy nie zostal wyslany!\n");
    }
}


//handler sygnalu SIGUSR1
void handle(int sig_no, siginfo_t *info, void *ucontext){
    int senderPID = info->si_pid;
    union sigval sv = info->si_value;
    
    //odczytanie trybu
    int tribe = sv.sival_int;

    //wykonuje zadania okreslone dla kazdego trybu pracy

    //1 - wypisanie liczb od 1 do 100
    if(tribe == 1){
        tribeInit();

        printf("Liczby od 1 do 100: ");
        for(int i=1;i<=100;i++){
            printf("%d ", i);
        }
        printf("\n");
    }
    //2 - wypisanie aktualnego czasu
    else if(tribe == 2){
        tribeInit();

        time(&t);
        printf("Aktualny czas: %s", ctime(&t));
    }
    //3 - wypisanie liczby zmian trybu pracy
    else if(tribe == 3){
        tribeInit();

        printf("Liczba zmian trybu pracy: %d\n", nOfChanges);
    }
    //4 - wypisywanie aktualnego czasu co 1 s
    else if(tribe == 4){
        nOfChanges++;
        //jesli ten tryb juz pracuje to nic nie robie
        if(timePID == 0){
            //tworze nowy proces dla wypisywania czasu
            int newPID = fork();
            if(newPID == 0){
                //nieskonczona petla wypisujaca czas co 1 s dopoki nie zmienie trybu
                while(1){
                    time(&t);
                    printf("Aktualny czas: %s", ctime(&t));
                    sleep(1);
                }
            }
            else{
                timePID = newPID;
            }
        }
        
        
    }
    //5 - koniec pracy programu catcher
    else if(tribe == 5){
        tribeInit();

        //po prostu wychodze z nieskonczonej petli
        stoper = 0;
    }
    //jesli nie wybrano zadnego trybu
    else{
        perror("Nie rozpoznano trybu!\n");
    }
    
    //wysylam potwierdzenie
    sendSignal(senderPID, 0);
    
}



int main(){

    //wypisanie PIDu
    printf("PID: %d\n", getpid());


    //ustawiam obsluge sygnalu SIGUSR1
    struct sigaction act; 
    act.sa_handler = handle;
    sigemptyset(&act.sa_mask); 
    act.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &act, NULL);


    //czekam na sygnaly
    while(stoper){
        
    }


    return 0;
}