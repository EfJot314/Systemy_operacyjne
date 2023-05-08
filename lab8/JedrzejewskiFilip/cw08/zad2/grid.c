#include "grid.h"
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

const int grid_width = 30;
const int grid_height = 30;


void handler(int signum){
}



void * cellLive(void *arghs){
    //pobieram argumenty
    void** args = (void**)arghs;
    int* Xtmp = (int*)args[0];
    int* Ytmp = (int*)args[1];
    int count = *(int*)args[2];
    char* foreground = (char*)args[3];
    char* background = (char*)args[4];

    //przepisuje aby potem zwolnic
    int X[count];
    int Y[count];
    for(int i=0;i<count;i++){
        X[i] = Xtmp[i];
        Y[i] = Ytmp[i];
    }
    
    //uwalniam wszystko co moge
    free(args[0]);
    free(args[1]);
    free(args[2]);
    free(arghs);


    //glowna petla
    while(true){
        //czekam na sygnal
        pause();

        background = foreground;

        for(int i=0;i<count;i++){
            int x = X[i];
            int y = Y[i];
            if(is_alive(x, y, background)){
                foreground[grid_width*y+x] = true;
            }
            else{
                foreground[grid_width*y+x] = false;
            }
        }
        
    }



}



char *create_grid()
{
    return malloc(sizeof(char) * grid_width * grid_height);
}

void destroy_grid(char *grid)
{
    free(grid);
}

void destroy_threads(pthread_t** threads, int n){
    for(int i=0;i<n;i++){
        pthread_cancel(*threads[i]);
        free(threads[i]);
    }
    free(threads);
}

void draw_grid(char *grid)
{
    for (int i = 0; i < grid_height; ++i)
    {
        // Two characters for more uniform spaces (vertical vs horizontal)
        for (int j = 0; j < grid_width; ++j)
        {
            if (grid[i * grid_width + j])
            {
                mvprintw(i, j * 2, "■");
                mvprintw(i, j * 2 + 1, " ");
            }
            else
            {
                mvprintw(i, j * 2, " ");
                mvprintw(i, j * 2 + 1, " ");
            }
        }
    }

    refresh();
}

pthread_t** init_grid(char *grid1, char* grid2, int n)
{
    //obsluga SIGUSR1
    signal(SIGUSR1, handler);

    //wypelnianie grida
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid1[i] = rand() % 2 == 0;

    
    //przydzielam komorki watkom
    int xTab[n][(int)(grid_width*grid_height/n)+1];
    int yTab[n][(int)(grid_width*grid_height/n)+1];
    int countTab[n];
    for(int i=0;i<n;i++){
        countTab[i] = 0;
    }
    int iterator = 0;
    for(int x=0; x<grid_width; x++){
        for(int y=0; y<grid_height; y++){
            xTab[iterator][countTab[iterator]] = x;
            yTab[iterator][countTab[iterator]] = y;
            countTab[iterator] += 1;

            //zwiekszanie iteratora
            iterator = (iterator + 1) % n;
        }
    }

    //tworze tablice watkow i ja wypelniam
    pthread_t** toReturn = calloc(n, sizeof(pthread_t*));
    for(int i=0;i<n;i++){
        //tworze argumenty
        void ** args = calloc(5, sizeof(void*));
        
        int* X = (int*)calloc(countTab[i], sizeof(int));
        int* Y = (int*)calloc(countTab[i], sizeof(int));

        for(int j=0;j<countTab[i];j++){
            X[j] = xTab[i][j];
            Y[j] = yTab[i][j];
        }

        int* c = (int*)calloc(1, sizeof(int));
        *c = countTab[i];
        
        args[0] = (void*)X;
        args[1] = (void*)Y;
        args[2] = (void*)c;
        args[3] = (void*)grid1;
        args[4] = (void*)grid2;

        //tworze nowy watek
        pthread_t* newThread = (pthread_t*)calloc(1, sizeof(pthread_t));
        pthread_create(newThread, NULL, cellLive, (void*)args);
        toReturn[i] = newThread;
    }

    return toReturn;
}

bool is_alive(int row, int col, char *grid)
{

    int count = 0;
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            if (i == 0 && j == 0)
            {
                continue;
            }
            int r = row + i;
            int c = col + j;
            if (r < 0 || r >= grid_height || c < 0 || c >= grid_width)
            {
                continue;
            }
            if (grid[grid_width * r + c])
            {
                count++;
            }
        }
    }

    if (grid[row * grid_width + col])
    {
        if (count == 2 || count == 3)
            return true;
        else
            return false;
    }
    else
    {
        if (count == 3)
            return true;
        else
            return false;
    }
}

void update_grid(pthread_t** threads, int n)
{
    for(int i=0;i<n;i++){
        pthread_kill(*threads[i], SIGUSR1);
    }
    
}