#include <SDL2/SDL.h>
#include <iostream>
#include <numeric>
#include <vector>

using namespace std;

void draw_row(SDL_Renderer *r, int items_sum, vector<int> row, 
		SDL_Rect bounds) 
{
	int pos = 0;
	int row_sum = accumulate(row.begin(), row.end(), 0);
	bool vertical = bounds.w > bounds.h;
	float row_width = (float)row_sum / (float)(row_sum + items_sum) *
	(float)(vertical ? bounds.w : bounds.h);
	for (int i = 0; i < row.size(); i ++) {
		SDL_Rect rect;
		if (vertical) {
			rect = { bounds.x, bounds.y + pos, rint(row_width),
				rint(((float)row.at(i) / (float)row_sum) * (float)(bounds.h - pos))
			};
		} else {
			rect = { bounds.x + pos, bounds.y, 
				rint(((float)row.at(i) / (float)row_sum) * (float)(bounds.w - pos)),
				rint(row_width)
			};
		}
		row_sum -= row.at(i);
		SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
		SDL_RenderFillRect(r, &rect);

		SDL_Rect rect2 = {rect.x + 1, rect.y + 1, rect.w - 2, rect.h - 2};
		SDL_SetRenderDrawColor(r, 0, 0, 0, 255);
		SDL_RenderFillRect(r, &rect2);

		pos += (vertical ? rect.h : rect.w);
	}
}

float worst(int item, int items_sum, int row_sum, SDL_Rect bounds) {
	bool vertical = bounds.w > bounds.h;
	row_sum += item;
	float w = ((float)row_sum / (float)(row_sum + items_sum)) * 
		(float)(vertical ? bounds.w : bounds.h);
	float h = ((float)item / (float)row_sum) * 
		(float)(vertical ? bounds.h : bounds.w);
	return fabs((w / h) - 1.0);
}

void squarify(SDL_Renderer *r, vector<int> items, vector<int> row, 
		SDL_Rect bounds) {
	
	int item, items_sum, row_sum;
	items_sum = accumulate(items.begin(), items.end(), 0);
	row_sum = accumulate(row.begin(), row.end(), 0);
	item = items.front();
	items.erase(items.begin());
	
	bool vertical = bounds.w > bounds.h;
	
	int shrink = rint(((float)row_sum / (float)(row_sum + items_sum)) *
		(float)(vertical ? bounds.w : bounds.h));
	
	SDL_Rect next_bounds = bounds;
	
	if (vertical) {
		next_bounds.x += shrink;
		next_bounds.w -= shrink;
	} else {
		next_bounds.y += shrink;
		next_bounds.h -= shrink;
	}

	float existing_row = worst(item, items_sum, row_sum, bounds);
	float next_row = worst(item, items_sum - row_sum, 0, next_bounds);
	bool add_to_next_row = existing_row > next_row;

	if (add_to_next_row) {
		draw_row(r, items_sum, row, bounds);
		row.clear();
	}

	row.push_back(item);
	
	if (items.size() > 0) {
		squarify(r, items, row, add_to_next_row ? next_bounds : bounds);
	} else {
		draw_row(r, 0, row, row.size() == 1 ? next_bounds : bounds);
	}
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
		SDL_Rect bounds = {0, 0, w, h};

		squarify(renderer, items, row, bounds);
		
		SDL_RenderPresent(renderer);
		SDL_Delay(33);
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}

