#pragma once
#include <stdbool.h>
#include <pthread.h>

char *create_grid();
void destroy_grid(char *grid);
void destroy_threads(pthread_t** threads, int n);
void draw_grid(char *grid);
pthread_t** init_grid(char *grid1, char *grid2, int n);
bool is_alive(int row, int col, char *grid);
void update_grid(pthread_t** threads, int n);