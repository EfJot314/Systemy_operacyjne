#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>



char* path = "./mails.txt";

int main(int argc, char* argv[]){

    //wywolanie z jednym argumentem
    if(argc == 2){

    }
    //wywolanie z trzema argumentami
    else if(argc == 4){
        //pobieram argumenty
        char* email = argv[1];
        char* title = argv[2];
        char* content = argv[3];

        //otwieram plik
        int d = open(path, O_WRONLY);
        if(d <= 0){
            perror("Blad otwierania pliku!");
            exit(1);
        }

        int sum_len = strlen(email)+strlen(title)+strlen(content)+2;

        char* email_to_save = (char*)calloc(sum_len, sizeof(char));

        strcpy(email_to_save, email);
        strcpy(email_to_save, "\t");
        strcpy(email_to_save, title);
        strcpy(email_to_save, "\t");
        strcpy(email_to_save, content);


        write(d, email_to_save, sum_len);

        close(d);
        
    }
    else{
        perror("Niepoprawna liczba argumentow!");
    }

    return 0;
}