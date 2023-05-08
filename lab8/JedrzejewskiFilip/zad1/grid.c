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
    int x = *(int*)args[0];
    int y = *(int*)args[1];
    char* foreground = (char*)args[2];
    char* background = (char*)args[3];
    free(args[0]);
    free(args[1]);
    free(arghs);
    


    //glowna petla
    while(1){
        //czekam na sygnal
        pause();

        background = foreground;
        
        if(is_alive(x, y, background)){
            foreground[grid_width*y+x] = true;
        }
        else{
            foreground[grid_width*y+x] = false;
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

void destroy_threads(pthread_t** threads){
    for(int i=0;i<grid_height*grid_width;i++){
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

pthread_t** init_grid(char *grid1, char* grid2)
{
    //obsluga SIGUSR1
    signal(SIGUSR1, handler);

    //wypelnianie grida
    for (int i = 0; i < grid_width * grid_height; ++i)
        grid1[i] = rand() % 2 == 0;

    //tworzenie watkow
    pthread_t** toReturn = calloc(grid_width*grid_height, sizeof(pthread_t*));
    for(int x=0; x<grid_width; x++){
        for(int y=0; y<grid_height; y++){
            //tworze argumenty
            void ** args = calloc(4, sizeof(void*));
            int* i = (int*)calloc(1, sizeof(int));
            int* j = (int*)calloc(1, sizeof(int));
            *i = x;
            *j = y;
            args[0] = (void*)i;
            args[1] = (void*)j;
            args[2] = (void*)grid1;
            args[3] = (void*)grid2;


            //tworze nowy watek
            pthread_t* newThread = (pthread_t*)calloc(1, sizeof(pthread_t));
            pthread_create(newThread, NULL, cellLive, (void*)args);
            toReturn[y*grid_width+x] = newThread;

        }
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

void update_grid(pthread_t** threads)
{
    for(int i=0;i<grid_width*grid_height;i++){
        pthread_kill(*threads[i], SIGUSR1);
    }
    
}