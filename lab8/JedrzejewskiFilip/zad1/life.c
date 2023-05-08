#include <ncurses.h>
#include <locale.h>
#include <unistd.h>
#include <stdbool.h>
#include "grid.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int main()
{
	srand(time(NULL));
	setlocale(LC_CTYPE, "");
	initscr(); // Start curses mode

	char *foreground = create_grid();
	char *background = create_grid();

	pthread_t** threads = init_grid(foreground, background);

	while (true)
	{
		draw_grid(foreground);
		
		usleep(500 * 1000);

		// Step simulation
		update_grid(threads);

	}

	endwin(); // End curses mode
	destroy_grid(foreground);
	destroy_grid(background);
	destroy_threads(threads);

	return 0;
}
