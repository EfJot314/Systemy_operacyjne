#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int main(int argc, char* argv[])
{
	if(argc != 2){
		perror("Bledna liczba argumentow wywolania programu!");
		exit(1);
	}

	int n = atoi(argv[1]);
	if(n <= 0){
		perror("Bledny argument!");
		exit(1);
	}


	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();

	pthread_t** threads = init_grid(foreground, background, n);

	while (true)
	{
		draw_grid(foreground);
		
		usleep(500 * 1000);

		// Step simulation
		update_grid(threads, n);

	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);
	destroy_threads(threads, n);

	return 0;
}
