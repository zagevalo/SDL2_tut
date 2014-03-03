#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "logging.h"
#include "render.h"


const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
const int TILE_SIZE = 40;


int main(int argc, char **argv) {

	// Init SDL

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		logSDLError(std::cout, "SDL_Init");
		return 1;
	}


	// Init SDL_image

	int img_init_flags = IMG_INIT_JPG | IMG_INIT_PNG;
	if ((IMG_Init(img_init_flags) & img_init_flags) != img_init_flags) {
		logSDLError(std::cout, "IMG_Init");
		return 1;
	}

	// Create SDL_Window

	SDL_Window *win = SDL_CreateWindow("FCUK", 100, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	if (win == nullptr) {
		logSDLError(std::cout, "CreateWindow");
		return 1;
	}

	// Create SDL_Renderer

	SDL_Renderer *renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (renderer == nullptr) {
		logSDLError(std::cout, "CreateRenderer");
		return 1;
	}


	SDL_Texture *bg = loadTexture("res/bg.png", renderer);
	SDL_Texture *character = loadTexture("res/Alice.png", renderer);
	if (bg == nullptr || character == nullptr)
		return 4;


	// Main Loop

	SDL_Rect char_box;
	int iW, iH;
	SDL_QueryTexture(character, NULL, NULL, &iW, &iH);
	char_box.x = SCREEN_WIDTH / 2 - iW / 2;
	char_box.y = SCREEN_HEIGHT / 2 - iH / 2;
	char_box.w = iW + 20;
	char_box.h = iH + 20;

	SDL_Event e;
	bool quit = false;

	while (!quit) {
		while (SDL_PollEvent(&e)) {
			if (e.type == SDL_QUIT)
				quit = true;
			if (e.type == SDL_KEYDOWN)
				switch(e.key.keysym.sym) {
					case SDLK_LEFT: char_box.x -= 20; break;
					case SDLK_RIGHT: char_box.x += 20; break;
					case SDLK_UP: char_box.y -= 20; break;
					case SDLK_DOWN: char_box.y += 20; break;
					case SDLK_ESCAPE: quit = true; break;
				}
			if (e.type == SDL_MOUSEBUTTONDOWN)
				quit = true;
		}

		// Render images

		SDL_RenderClear(renderer);

		int bW, bH;
		SDL_QueryTexture(bg, NULL, NULL, &bW, &bH);

		SDL_Rect bg_box;
		bg_box.x = 0;
		bg_box.y = 0;
		bg_box.w = SCREEN_WIDTH;
		bg_box.h = SCREEN_HEIGHT;
		renderTexture(bg, renderer, bg_box, nullptr);

		renderTexture(character, renderer, char_box.x, char_box.y, nullptr);

		SDL_RenderPresent(renderer);
	}


	// Cleanup

	SDL_DestroyTexture(bg);
	SDL_DestroyTexture(character);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(win);

	SDL_Quit();
	return 0;
}
