#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "stm.h"

using namespace std;


void draw_rect(SDL_Renderer *r, SDL_Rect rect) {
	SDL_SetRenderDrawColor(r, 50, 155, 255, 255);
	SDL_RenderFillRect(r, &rect);

	SDL_Rect rect2 = {rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2};
	SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
	SDL_RenderFillRect(r, &rect2);
}


int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window *window = SDL_CreateWindow(
			"Directory Map",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			680, 480,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	SDL_Event event;
	
	bool running = true;
	while (running) {
		while(SDL_PollEvent(&event) > 0) {
			switch(event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
						// TODO: add code here that handles a window resizing event
						//printf("%d, %d\n", event.window.data1, event.window.data2);
					}
			}
		}
		
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		
		vector<int> items {6, 6, 4, 3, 2, 2, 1};
		vector<int> row {};

		int w, h; SDL_GetWindowSize(window, &w, &h);
		int bounds[4] = {0, 0, w, h};

		vector<int> rects = squarify(items, row, bounds);

		for (int i = 0; i < rects.size(); i += 4) {
			//printf("%d, %d, %d, %d\n", 
			SDL_Rect rect = {rects.at(i),rects.at(i+1),rects.at(i+2),rects.at(i+3)};
			draw_rect(renderer, rect);
		}
		
		SDL_RenderPresent(renderer);
		SDL_Delay(33);
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

