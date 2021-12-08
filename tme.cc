// tme.cc
// Treemap Explorer
// Samuel Bauman 2021

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <unistd.h>
#include "stm.h"
#include "scan_dir.h"

#define dir_padding 4
#define item_padding 1

using namespace std;
namespace fs = std::filesystem;


void draw_rect(SDL_Renderer *r, SDL_Rect rect) {
	
	SDL_RenderFillRect(r, &rect);
	
	if (rect.w > 1 || rect.h > 1) {
		SDL_Rect rect2 = {rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2};
		SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
		SDL_RenderFillRect(r, &rect2);
	}
	
	/*
	SDL_Color color = { 0, 0, 0 };
	SDL_Surface * surface = TTF_RenderText_Blended(font, text, color);
	SDL_Texture * texture = SDL_CreateTextureFromSurface(r, surface);
	
	int texW = 0;
	int texH = 0;
	SDL_QueryTexture(texture, NULL, NULL, &texW, &texH);
	SDL_Rect dstrect = { rect.x + 2, rect.y, texW, texH };
	
	SDL_RenderCopy(r, texture, NULL, &dstrect);
	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surface);
	*/
}

void fs_treemap(SDL_Renderer *r, fs_item dir, int bounds[4], 
		int depth = 0) {
	vector<int> sizes = dir.get_sizes();
	vector<int> row {};
	
	vector<int> rects = squarify(sizes, row, bounds);
	
	for (int i = 0; i < rects.size(); i += 4) {
		int index = i / 4;
		int new_bounds[4] = 
		{rects.at(i) + dir_padding, rects.at(i+1) + dir_padding, 
			rects.at(i+2) - (dir_padding * 2), 
			rects.at(i+3) - (dir_padding * 2)};
		SDL_Rect rect_bounds = 
		{rects.at(i), rects.at(i+1), max(rects.at(i+2), 1), max(rects.at(i+3), 1)};

		//cout << "Item: " << dir.children.at(index).name << endl;
		//if (dir.children.at(index))
		if (depth%4 == 0)
			SDL_SetRenderDrawColor(r, 255, 50, 50, 255);
		else if (depth%4 == 1)
			SDL_SetRenderDrawColor(r, 50, 255, 50, 255);
		else if (depth%4 == 2)
			SDL_SetRenderDrawColor(r, 255, 50, 255, 255);
		else if (depth%4 == 3)
			SDL_SetRenderDrawColor(r, 50, 150, 255, 255);

		draw_rect(r, rect_bounds);

		if (dir.children.at(index).is_dir && dir.children.at(index).size > 0 &&
				new_bounds[2] > dir_padding && new_bounds[3] > dir_padding) {
			//cout << "Is dir." << endl;
			fs_treemap(r, dir.children.at(index), new_bounds, depth + 1);
		}
	}
}


int main() {
	SDL_Init(SDL_INIT_EVERYTHING);
	//TTF_Init();
	SDL_Window *window = SDL_CreateWindow(
			"Directory Map",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			680, 480,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
	//TTF_Font * font = 
	//	TTF_OpenFont("font/victor_mono/VictorMono-Medium.ttf", 14);
	SDL_Event event;
	
	fs_item working_dir = scan_dir(get_current_dir_name());
	
	bool redraw = true;
	bool running = true;
	while (running) {
		while(SDL_PollEvent(&event) > 0) {
			switch(event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_WINDOWEVENT:
					if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
						redraw = true;
					}
			}
		}
	
		if (redraw) {
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			SDL_RenderClear(renderer);
		
			int w, h; SDL_GetWindowSize(window, &w, &h);
			int bounds[4] = {0, 0, w, h};
		
			fs_treemap(renderer, working_dir, bounds);
		
			SDL_RenderPresent(renderer);
			redraw = false;
		}
		SDL_Delay(33);
	}
	
	SDL_DestroyWindow(window);
	//TTF_Quit();
	SDL_Quit();
	
	return 0;
}

