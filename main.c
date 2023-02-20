#include <stdio.h> 
#include <stdlib.h>
#include <SDL2/SDL.h>

//Global Variables
const int WIDTH = 620;
const int HEIGHT = 480;

/* Function to sum living neighbors per the Moore Neighborhood definition */
int count_living_neighbors(int curbuffer[WIDTH][HEIGHT], int x, int y) {
	int sum = 0;
	for(int i = x-1; i <= x+1; i++) {
		for(int j = y-1; j <= y+1; j++) {
			if ((i == x && j == y) || (i < 0 || j < 0) || (i >= WIDTH || j >= HEIGHT)) {
				continue;
			}
			if (curbuffer[i][j] > 0) {
				sum++;
			}
		}
	}
	return sum;
}

/* Populates the next buffer with the cells that survive into the next generation. */
void populate_buffer(int initbuffer[WIDTH][HEIGHT], int nextbuffer[WIDTH][HEIGHT]){
	int num_live_neighbors = 0;
	int cellvalue;

	for (int i = 0; i < WIDTH; i++){
		for (int j = 0; j < HEIGHT; j++){
			num_live_neighbors = count_living_neighbors(initbuffer, i, j);	// Get the count of all neighbors of specific cell
			cellvalue = initbuffer[i][j];	// Current cell dead or alive
			// If the cell is alive and has 2 or 3 neighbors it lives
			if(cellvalue == 1 && (num_live_neighbors > 1 && num_live_neighbors < 4)) {
				nextbuffer[i][j] = 1;
			}
			// If the cell is dead and it has 3 neighbors it lives 
			else if(cellvalue == 0 && num_live_neighbors == 3) {
				nextbuffer[i][j] = 1;
			}
			// Otherwise the cell dies
			else {
				nextbuffer[i][j] = 0;
			}
		}
	}
}

/* Randomly generate an initial buffer of living cells */
void init_buffer(int initbuffer[WIDTH][HEIGHT]){
    for(int i = 0; i < WIDTH; i++){
        for(int j = 0; j < HEIGHT; j++){
            initbuffer[i][j] = (rand()-1)%2;
        }
    }
}

int main(int argc, char *argv[]) {
	
	// Buffers (2d array of 0's & 1's)
	int cbuffer[WIDTH][HEIGHT];
	int nbuffer[WIDTH][HEIGHT];

	int MAX_GEN = atoi(argv[1]);
	int curgen = 0;
	char title[10];
   
	// SDL pointers
	SDL_Window* window;
	SDL_Renderer* renderer;

	// Initialize SDL and window
	SDL_Init(SDL_INIT_VIDEO);
	SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);

	// Populate the initial buffer
    init_buffer(cbuffer);
	
	// Main simulation loop
	while(curgen < MAX_GEN) {

		// Update the title of the window to display the cell generation
		sprintf(title, "Gen: %d", curgen);
		SDL_SetWindowTitle(window, title);

		// Clear the screen by drawing it all black
		SDL_SetRenderDrawColor(renderer,0,0,0,255);
		SDL_RenderClear(renderer);

		// Alternate initial buffers based on the generation
		if(curgen % 2 == 0) {
			// populate
			populate_buffer(cbuffer, nbuffer);

			// Draw the current buffer to the renderer
			SDL_SetRenderDrawColor(renderer,255,255,255,255);
			for(int i = 0; i < WIDTH; i++){
				for(int j = 0; j < HEIGHT; j++){
					if(nbuffer[i][j] == 1){
						SDL_RenderDrawPoint(renderer, i, j);
					}
				}
			}
		}
		else{
			//populate
			populate_buffer(nbuffer, cbuffer);

			// Draw the current buffer to the renderer
			SDL_SetRenderDrawColor(renderer,255,255,255,255);
			for(int i = 0; i < WIDTH; i++){
				for(int j = 0; j < HEIGHT; j++){
					if(cbuffer[i][j] == 1){
						SDL_RenderDrawPoint(renderer, i, j);
					}
				}
			}
		}
		curgen++;
		// Present the rendered buffer
		SDL_RenderPresent(renderer);
		SDL_Delay(50);
	}

	SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
	return 0;
}
